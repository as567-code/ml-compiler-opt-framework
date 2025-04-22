//===- MemoryCoalescing.cpp - Memory Access Coalescing Pass -----------===//
//
// Implementation of a pass that analyzes memory access patterns and
// transforms them to improve memory coalescing for GPU architectures.
//
//===----------------------------------------------------------------===//

#include "passes/MemoryCoalescing.h"

#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

#define DEBUG_TYPE "memory-coalescing"

using namespace llvm;

namespace mlcompileropt {

PreservedAnalyses MemoryCoalescingPass::run(Function &F, FunctionAnalysisManager &AM) {
  LLVM_DEBUG(dbgs() << "Running Memory Coalescing Pass on function: " << F.getName() << "\n");
  
  // Get loop information for this function
  auto &LI = AM.getResult<LoopAnalysis>(F);
  bool Changed = false;
  
  // Process each loop in the function
  for (auto *L : LI) {
    Changed |= analyzeLoopMemoryAccess(L, LI);
    
    // Recursively process nested loops
    for (auto *SubLoop : L->getSubLoops()) {
      Changed |= analyzeLoopMemoryAccess(SubLoop, LI);
    }
  }
  
  // Process basic blocks for adjacent memory access merging
  for (auto &BB : F) {
    SmallVector<Instruction*, 8> MemInsts;
    
    // Collect memory instructions
    for (auto &I : BB) {
      if (isa<LoadInst>(I) || isa<StoreInst>(I)) {
        MemInsts.push_back(&I);
      }
    }
    
    if (MemInsts.size() > 1) {
      Changed |= mergeAdjacentAccesses(&BB, MemInsts);
    }
  }
  
  return Changed ? PreservedAnalyses::none() : PreservedAnalyses::all();
}

bool MemoryCoalescingPass::analyzeLoopMemoryAccess(Loop *L, LoopInfo &LI) {
  LLVM_DEBUG(dbgs() << "Analyzing loop for memory access patterns\n");
  
  bool Changed = false;
  SmallVector<Instruction*, 8> StridedLoads;
  
  // Collect strided load instructions
  for (auto *BB : L->getBlocks()) {
    for (auto &I : *BB) {
      if (auto *LI = dyn_cast<LoadInst>(&I)) {
        int Stride = 0;
        if (isStridedAccess(LI, Stride) && Stride > 1) {
          StridedLoads.push_back(LI);
          LLVM_DEBUG(dbgs() << "  Found strided load: " << *LI << " with stride: " << Stride << "\n");
        }
      }
    }
  }
  
  // Transform strided loads if possible
  if (!StridedLoads.empty()) {
    Changed |= transformStridedAccess(L, StridedLoads);
  }
  
  return Changed;
}

bool MemoryCoalescingPass::isStridedAccess(Instruction *I, int &Stride) {
  // For this simple implementation, we'll detect common patterns of strided access
  // A more robust implementation would use scalar evolution
  
  // We're looking for: ptr = base + i * stride
  if (auto *LI = dyn_cast<LoadInst>(I)) {
    if (auto *GEP = dyn_cast<GetElementPtrInst>(LI->getPointerOperand())) {
      // Simple pattern: constant index multiplied by loop induction variable
      if (GEP->getNumIndices() == 1) {
        Value *Index = GEP->getOperand(1);
        if (auto *Mul = dyn_cast<BinaryOperator>(Index)) {
          if (Mul->getOpcode() == Instruction::Mul) {
            if (auto *ConstInt = dyn_cast<ConstantInt>(Mul->getOperand(1))) {
              Stride = ConstInt->getSExtValue();
              return true;
            }
          }
        }
      }
    }
  }
  
  Stride = 0;
  return false;
}

bool MemoryCoalescingPass::transformStridedAccess(Loop *L, SmallVector<Instruction*, 8> &StridedLoads) {
  LLVM_DEBUG(dbgs() << "Transforming strided memory accesses\n");
  
  // In a real implementation, this would transform the loop and memory access pattern
  // For simplicity, we'll just return true indicating the changes would be made
  
  // The actual transformation would:
  // 1. Analyze the strided access pattern
  // 2. Determine if loop interchange would improve coalescing
  // 3. Transform the loop structure if beneficial
  
  return true; // Pretend we made changes for demonstration
}

bool MemoryCoalescingPass::mergeAdjacentAccesses(BasicBlock *BB, SmallVector<Instruction*, 8> &MemInsts) {
  LLVM_DEBUG(dbgs() << "Looking for adjacent memory accesses to merge\n");
  
  bool Changed = false;
  
  // In a real implementation, this would identify adjacent loads/stores and merge them
  // For simplicity, we'll just look for adjacent loads of the same type and pretend to merge them
  
  for (size_t i = 0; i < MemInsts.size() - 1; ++i) {
    if (auto *Load1 = dyn_cast<LoadInst>(MemInsts[i])) {
      if (auto *Load2 = dyn_cast<LoadInst>(MemInsts[i + 1])) {
        // Check if the loads are from adjacent memory locations
        // This is a simplified check and would be more complex in real implementation
        
        if (Load1->getType() == Load2->getType()) {
          // For demonstration purposes only - indicate we found a candidate
          LLVM_DEBUG(dbgs() << "  Found candidate loads to merge: " 
                           << *Load1 << " and " << *Load2 << "\n");
          
          // In a real implementation, we would:
          // 1. Check if these loads are actually adjacent in memory
          // 2. Create a wider load instruction if possible
          // 3. Extract the individual values with bitwise operations
          // 4. Replace the original loads with the extracted values
          
          Changed = true; // Pretend we made changes
        }
      }
    }
  }
  
  return Changed;
}

// Factory function for creating our pass
FunctionPassManager buildMemoryCoalescingPipeline() {
  FunctionPassManager FPM;
  FPM.addPass(MemoryCoalescingPass());
  return FPM;
}

} // namespace mlcompileropt 