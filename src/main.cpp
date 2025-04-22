// src/main.cpp

#include <iostream>
#include <memory>

// LLVM core headers
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"

// LLVM legacy pass manager and a couple of passes
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input-IR-file>\n";
        return 1;
    }

    // 1. Setup LLVM context and parse the IR file
    llvm::LLVMContext Context;
    llvm::SMDiagnostic Err;
    auto Module = llvm::parseIRFile(argv[1], Err, Context);
    if (!Module) {
        Err.print(argv[0], llvm::errs());
        return 1;
    }

    // 2. Configure the pass manager
    llvm::legacy::PassManager PassMgr;
    llvm::PassManagerBuilder Builder;
    Builder.OptLevel = 3;                    // Aggressive optimization
    Builder.Inliner = llvm::AlwaysInliner(); // Enable function inlining
    Builder.populateModulePassManager(PassMgr);

    // Add a couple of example scalar passes
    PassMgr.add(llvm::createPromoteMemoryToRegisterPass()); // Mem2Reg
    PassMgr.add(llvm::createLoopUnrollPass());              // Loop Unrolling

    // 3. Run the passes
    PassMgr.run(*Module);

    // 4. Output the optimized IR to stdout
    Module->print(llvm::outs(), nullptr);

    return 0;
}