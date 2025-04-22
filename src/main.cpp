// src/main.cpp

#include <iostream>
#include <memory>
#include <string>

// LLVM core headers
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"

// LLVM new pass manager
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/Analysis/CGSCCPassManager.h"
#include "llvm/Transforms/Scalar/LoopUnrollPass.h"
#include "llvm/Passes/OptimizationLevel.h"

// Custom passes
#include "passes/MemoryCoalescing.h"

int main(int argc, char** argv) {
    // Check command line arguments
    if (argc < 2) {
        std::cerr << "ML Compiler Optimization Framework\n"
                  << "--------------------------------\n"
                  << "Usage: " << argv[0] << " <input-IR-file>\n";
        return 1;
    }

    std::string InputFilename = argv[1];
    
    // 1. Setup LLVM context and parse the IR file
    llvm::LLVMContext Context;
    llvm::SMDiagnostic Err;
    std::unique_ptr<llvm::Module> Module = llvm::parseIRFile(InputFilename, Err, Context);
    
    if (!Module) {
        llvm::errs() << "Error loading file '" << InputFilename << "':\n";
        Err.print(argv[0], llvm::errs());
        return 1;
    }

    llvm::outs() << "Successfully loaded IR module '" << Module->getName().str() << "'\n";
    
    // 2. Setup the new pass manager
    llvm::LoopAnalysisManager LAM;
    llvm::FunctionAnalysisManager FAM;
    llvm::CGSCCAnalysisManager CGAM;
    llvm::ModuleAnalysisManager MAM;
    
    // Create pass builder and register analysis passes
    llvm::PassBuilder PB;
    PB.registerModuleAnalyses(MAM);
    PB.registerCGSCCAnalyses(CGAM);
    PB.registerFunctionAnalyses(FAM);
    PB.registerLoopAnalyses(LAM);
    PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);
    
    // Create module pass manager with standard optimization passes
    llvm::ModulePassManager MPM = PB.buildPerModuleDefaultPipeline(
        llvm::OptimizationLevel::O3);
    
    // 3. Add our custom memory coalescing pass
    llvm::outs() << "Adding custom memory coalescing pass...\n";
    for (auto &F : *Module) {
        if (!F.isDeclaration()) {
            // Create and run the custom memory coalescing pass
            mlcompileropt::MemoryCoalescingPass MemCoalesce;
            MemCoalesce.run(F, FAM);
        }
    }
    
    // 4. Run the standard optimization passes
    llvm::outs() << "Running optimization passes...\n";
    MPM.run(*Module, MAM);
    
    // 5. Output the optimized IR to stdout
    llvm::outs() << "Optimized IR:\n";
    llvm::outs() << "------------\n";
    Module->print(llvm::outs(), nullptr);
    
    return 0;
}