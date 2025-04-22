#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <string>
#include <memory>
#include <iomanip>

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/Analysis/CGSCCPassManager.h"
#include "llvm/Passes/OptimizationLevel.h"

// Include our custom passes
#include "passes/MemoryCoalescing.h"

// Simple timer class for benchmarking
class Timer {
public:
    Timer() : start(std::chrono::high_resolution_clock::now()) {}
    
    double elapsed() const {
        auto now = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(now - start).count();
    }
    
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
};

// Benchmarking function for optimization passes
double runOptimizer(const std::string &InputFile, bool useCustomPasses = true) {
    // Create context and parse the module
    llvm::LLVMContext Context;
    llvm::SMDiagnostic Err;
    
    // Load the IR file
    std::unique_ptr<llvm::Module> Module = llvm::parseIRFile(InputFile, Err, Context);
    if (!Module) {
        llvm::errs() << "Error loading file: " << InputFile << "\n";
        Err.print("benchmark", llvm::errs());
        return -1.0;
    }
    
    // Setup the pass manager infrastructure
    llvm::LoopAnalysisManager LAM;
    llvm::FunctionAnalysisManager FAM;
    llvm::CGSCCAnalysisManager CGAM;
    llvm::ModuleAnalysisManager MAM;
    
    llvm::PassBuilder PB;
    PB.registerModuleAnalyses(MAM);
    PB.registerCGSCCAnalyses(CGAM);
    PB.registerFunctionAnalyses(FAM);
    PB.registerLoopAnalyses(LAM);
    PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);
    
    // Create the standard optimization pipeline
    llvm::ModulePassManager MPM = PB.buildPerModuleDefaultPipeline(
        llvm::OptimizationLevel::O3);
    
    // Start the timer
    Timer timer;
    
    // Apply our custom passes if requested
    if (useCustomPasses) {
        // Apply memory coalescing to each function
        for (auto &F : *Module) {
            if (!F.isDeclaration()) {
                mlcompileropt::MemoryCoalescingPass MemCoalesce;
                MemCoalesce.run(F, FAM);
            }
        }
    }
    
    // Run the standard optimization pipeline
    MPM.run(*Module, MAM);
    
    // Return the elapsed time
    return timer.elapsed();
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <ir-file> [<ir-file> ...]\n";
        return 1;
    }
    
    std::cout << "ML Compiler Optimization Framework - Benchmark\n";
    std::cout << "=============================================\n\n";
    
    // Print header
    std::cout << std::left << std::setw(30) << "File"
              << std::right << std::setw(15) << "Standard (ms)"
              << std::right << std::setw(15) << "Custom (ms)"
              << std::right << std::setw(15) << "Speedup (%)"
              << "\n";
    std::cout << std::string(75, '-') << "\n";
    
    // Process each input file
    for (int i = 1; i < argc; ++i) {
        std::string InputFile = argv[i];
        
        // Run with standard passes only
        double standardTime = runOptimizer(InputFile, false);
        
        // Run with our custom passes
        double customTime = runOptimizer(InputFile, true);
        
        // Calculate speedup
        double speedup = 0.0;
        if (standardTime > 0 && customTime > 0) {
            speedup = ((standardTime - customTime) / standardTime) * 100.0;
        }
        
        // Print results
        std::cout << std::left << std::setw(30) << InputFile
                  << std::right << std::setw(15) << std::fixed << std::setprecision(2) << standardTime
                  << std::right << std::setw(15) << std::fixed << std::setprecision(2) << customTime
                  << std::right << std::setw(15) << std::fixed << std::setprecision(2) << speedup
                  << "\n";
        
        // Save the optimized output (optional)
        std::string OutputFile = InputFile + ".opt.ll";
        // Code to save optimized IR would go here
    }
    
    return 0;
} 