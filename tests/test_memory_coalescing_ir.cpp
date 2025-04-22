#include <gtest/gtest.h>

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/Analysis/CGSCCPassManager.h"
#include "llvm/Passes/PassBuilder.h"

#include "passes/MemoryCoalescing.h"

// Path to test files
const std::string TEST_FILES_DIR = "/Users/adityaswaroop/Desktop/ml-compiler-opt-framework/tests/test_files/";

// Test fixture for memory coalescing tests
class MemoryCoalescingIRTest : public ::testing::Test {
protected:
  void SetUp() override {
    // Create module and context for testing
    Context = std::make_unique<llvm::LLVMContext>();
  }

  // Helper to load an IR file into a module
  bool loadIRFile(const std::string &Filename) {
    llvm::SMDiagnostic Err;
    M = llvm::parseIRFile(Filename, Err, *Context);
    
    if (!M) {
      Err.print("test", llvm::errs());
      return false;
    }
    
    return true;
  }

  // Helper to run the memory coalescing pass on a function
  void runMemoryCoalescingPass(llvm::Function &F) {
    // Create analysis managers
    llvm::LoopAnalysisManager LAM;
    llvm::FunctionAnalysisManager FAM;
    llvm::CGSCCAnalysisManager CGAM;
    llvm::ModuleAnalysisManager MAM;
    
    // Register analyses
    llvm::PassBuilder PB;
    PB.registerModuleAnalyses(MAM);
    PB.registerCGSCCAnalyses(CGAM);
    PB.registerFunctionAnalyses(FAM);
    PB.registerLoopAnalyses(LAM);
    PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);
    
    // Run our pass
    mlcompileropt::MemoryCoalescingPass MemCoalesce;
    MemCoalesce.run(F, FAM);
  }

  std::unique_ptr<llvm::LLVMContext> Context;
  std::unique_ptr<llvm::Module> M;
};

// Test the Memory Coalescing pass on strided access pattern
TEST_F(MemoryCoalescingIRTest, StridedAccessPattern) {
  // Load the IR file with strided access
  std::string FilePath = TEST_FILES_DIR + "strided_access.ll";
  ASSERT_TRUE(loadIRFile(FilePath));
  
  // Get the function
  llvm::Function *F = M->getFunction("strided_access");
  ASSERT_NE(F, nullptr);
  
  // Run the memory coalescing pass
  runMemoryCoalescingPass(*F);
  
  // In a real implementation, we would check for specific transformations
  // For example, we might verify that the stride has been removed or adjusted
  // For now, we just verify that the function still exists and is well-formed
  
  // This provides a minimal verification that the pass doesn't crash
  // and preserves basic function structure 
  ASSERT_FALSE(F->empty());
  
  // You could add more verification here based on expected transformations
  // For example, checking if the stride pattern was recognized:
  // - Count the number of load instructions
  // - Verify memory access patterns after optimization
}

// Test the Memory Coalescing pass on adjacent memory accesses
TEST_F(MemoryCoalescingIRTest, AdjacentMemoryAccesses) {
  // Load the IR file with adjacent memory accesses
  std::string FilePath = TEST_FILES_DIR + "strided_access.ll";
  ASSERT_TRUE(loadIRFile(FilePath));
  
  // Get the function
  llvm::Function *F = M->getFunction("adjacent_access");
  ASSERT_NE(F, nullptr);
  
  // Run the memory coalescing pass
  runMemoryCoalescingPass(*F);
  
  // Similar to above, we just verify basic function integrity
  ASSERT_FALSE(F->empty());
  
  // In a full implementation, we would check if adjacent loads were merged
  // For example, we might count load instructions and check if there are fewer loads
}

// Test the Memory Coalescing pass on nested loops with strided access
TEST_F(MemoryCoalescingIRTest, NestedLoopStridedAccess) {
  // Load the IR file with nested loops
  std::string FilePath = TEST_FILES_DIR + "strided_access.ll";
  ASSERT_TRUE(loadIRFile(FilePath));
  
  // Get the function
  llvm::Function *F = M->getFunction("nested_loop_strided");
  ASSERT_NE(F, nullptr);
  
  // Run the memory coalescing pass
  runMemoryCoalescingPass(*F);
  
  // Verify basic function integrity
  ASSERT_FALSE(F->empty());
  
  // In a full implementation, we might check:
  // - Loop structure after transformation
  // - Memory access patterns in the transformed code
}

// Main function for the test
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
} 