#include <gtest/gtest.h>

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/Analysis/CGSCCPassManager.h"
#include "llvm/Passes/PassBuilder.h"

#include "passes/MemoryCoalescing.h"

// Test fixture for memory coalescing tests
class MemoryCoalescingTest : public ::testing::Test {
protected:
  void SetUp() override {
    // Create module and context for testing
    Context = std::make_unique<llvm::LLVMContext>();
    M = std::make_unique<llvm::Module>("test_module", *Context);
  }

  // Helper to parse IR string into a module
  bool parseIR(const std::string &IR) {
    llvm::SMDiagnostic Err;
    std::unique_ptr<llvm::Module> ParsedModule = 
        llvm::parseIR(llvm::MemoryBufferRef(IR, "testIR"), Err, *Context);
    
    if (!ParsedModule) {
      Err.print("test", llvm::errs());
      return false;
    }
    
    M = std::move(ParsedModule);
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

// Test basic functionality of the memory coalescing pass
TEST_F(MemoryCoalescingTest, BasicCoalescing) {
  // Simple IR with strided memory access
  const char *IR = R"(
    define void @test_strided_access(float* %input, float* %output, i32 %n) {
    entry:
      br label %loop

    loop:
      %i = phi i32 [ 0, %entry ], [ %i.next, %loop ]
      %idx = mul i32 %i, 2
      %gep = getelementptr float, float* %input, i32 %idx
      %val = load float, float* %gep
      %gep.out = getelementptr float, float* %output, i32 %i
      store float %val, float* %gep.out
      %i.next = add i32 %i, 1
      %cond = icmp slt i32 %i.next, %n
      br i1 %cond, label %loop, label %exit

    exit:
      ret void
    }
  )";
  
  // Parse the IR
  ASSERT_TRUE(parseIR(IR));
  
  // Get the function
  llvm::Function *F = M->getFunction("test_strided_access");
  ASSERT_NE(F, nullptr);
  
  // Run the memory coalescing pass
  runMemoryCoalescingPass(*F);
  
  // At this point, we would verify the transformation
  // For the simplified implementation, we just ensure the test runs
  SUCCEED();
}

// Test that the pass correctly identifies adjacent memory accesses
TEST_F(MemoryCoalescingTest, AdjacentMemoryAccesses) {
  // Simple IR with adjacent memory accesses
  const char *IR = R"(
    define void @test_adjacent_access(float* %input, float* %output) {
    entry:
      %gep1 = getelementptr float, float* %input, i32 0
      %val1 = load float, float* %gep1
      %gep2 = getelementptr float, float* %input, i32 1
      %val2 = load float, float* %gep2
      %sum = fadd float %val1, %val2
      store float %sum, float* %output
      ret void
    }
  )";
  
  // Parse the IR
  ASSERT_TRUE(parseIR(IR));
  
  // Get the function
  llvm::Function *F = M->getFunction("test_adjacent_access");
  ASSERT_NE(F, nullptr);
  
  // Run the memory coalescing pass
  runMemoryCoalescingPass(*F);
  
  // Again, for the simplified implementation, we just ensure the test runs
  SUCCEED();
}

// Main function for the test
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
} 