//===- MemoryCoalescing.h - Memory Access Coalescing Pass -----------===//
//
// This file defines a pass that analyzes memory access patterns and 
// transforms them to improve memory coalescing for GPU architectures.
//
//===----------------------------------------------------------------===//

#ifndef MLCOMPILEROPT_PASSES_MEMORY_COALESCING_H
#define MLCOMPILEROPT_PASSES_MEMORY_COALESCING_H

#include "llvm/IR/PassManager.h"
#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/Function.h"

namespace mlcompileropt {

class MemoryCoalescingPass : public llvm::PassInfoMixin<MemoryCoalescingPass> {
public:
  // Main entry point for the pass
  llvm::PreservedAnalyses run(llvm::Function &F, llvm::FunctionAnalysisManager &AM);

  // Required for LLVM pass usage
  static bool isRequired() { return true; }

private:
  // Analyzes memory access patterns in a loop
  bool analyzeLoopMemoryAccess(llvm::Loop *L, llvm::LoopInfo &LI);
  
  // Checks if a given instruction is a strided memory access
  bool isStridedAccess(llvm::Instruction *I, int &Stride);
  
  // Transforms strided memory accesses to coalesced access pattern
  bool transformStridedAccess(llvm::Loop *L, llvm::SmallVector<llvm::Instruction*, 8> &StridedLoads);
  
  // Merges adjacent memory accesses when beneficial
  bool mergeAdjacentAccesses(llvm::BasicBlock *BB, llvm::SmallVector<llvm::Instruction*, 8> &MemInsts);
};

// Factory function to create the pass for registration
llvm::FunctionPassManager buildMemoryCoalescingPipeline();

} // namespace mlcompileropt

#endif // MLCOMPILEROPT_PASSES_MEMORY_COALESCING_H 