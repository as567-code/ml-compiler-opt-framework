; ModuleID = 'strided_access.ll'
source_filename = "strided_access.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function for testing strided memory access patterns
; This represents a common pattern in GPU kernels where thread i
; accesses array elements with a stride
define void @strided_access(float* %input, float* %output, i32 %n) {
entry:
  br label %loop

loop:
  %i = phi i32 [ 0, %entry ], [ %i.next, %loop ]
  %idx = mul i32 %i, 4                        ; strided access with stride 4
  %gep.in = getelementptr float, float* %input, i32 %idx
  %val = load float, float* %gep.in, align 4
  %gep.out = getelementptr float, float* %output, i32 %i
  store float %val, float* %gep.out, align 4
  %i.next = add i32 %i, 1
  %cond = icmp slt i32 %i.next, %n
  br i1 %cond, label %loop, label %exit

exit:
  ret void
}

; Function for testing adjacent memory accesses that could be coalesced
define float @adjacent_access(float* %input) {
entry:
  %gep1 = getelementptr float, float* %input, i32 0
  %val1 = load float, float* %gep1, align 4
  %gep2 = getelementptr float, float* %input, i32 1
  %val2 = load float, float* %gep2, align 4
  %gep3 = getelementptr float, float* %input, i32 2
  %val3 = load float, float* %gep3, align 4
  %gep4 = getelementptr float, float* %input, i32 3
  %val4 = load float, float* %gep4, align 4
  
  %sum1 = fadd float %val1, %val2
  %sum2 = fadd float %val3, %val4
  %final = fadd float %sum1, %sum2
  
  ret float %final
}

; Function testing a nested loop with strided access in the inner loop
define void @nested_loop_strided(float* %input, float* %output, i32 %width, i32 %height) {
entry:
  br label %outer.loop

outer.loop:
  %i = phi i32 [ 0, %entry ], [ %i.next, %outer.end ]
  br label %inner.loop

inner.loop:
  %j = phi i32 [ 0, %outer.loop ], [ %j.next, %inner.loop ]
  
  ; Calculate 2D array index with strided access pattern
  %row_offset = mul i32 %i, %width
  %col_offset = mul i32 %j, 2            ; stride of 2 in inner dimension
  %idx = add i32 %row_offset, %col_offset
  
  %gep.in = getelementptr float, float* %input, i32 %idx
  %val = load float, float* %gep.in, align 4
  
  ; Store to output array with linear access
  %lin_idx = mul i32 %i, %width
  %lin_idx2 = add i32 %lin_idx, %j
  %gep.out = getelementptr float, float* %output, i32 %lin_idx2
  store float %val, float* %gep.out, align 4
  
  %j.next = add i32 %j, 1
  %j.cond = icmp slt i32 %j.next, %width
  br i1 %j.cond, label %inner.loop, label %outer.end

outer.end:
  %i.next = add i32 %i, 1
  %i.cond = icmp slt i32 %i.next, %height
  br i1 %i.cond, label %outer.loop, label %exit

exit:
  ret void
} 