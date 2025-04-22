; Simple LLVM IR test file
; This contains a simple function that can be used to test the optimizer

; Module declaration
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function to compute factorial
define i64 @factorial(i64 %n) {
entry:
  %cmp = icmp sle i64 %n, 0
  br i1 %cmp, label %return, label %recurse

recurse:
  %sub = sub nsw i64 %n, 1
  %call = call i64 @factorial(i64 %sub)
  %mul = mul nsw i64 %n, %call
  br label %return

return:
  %result = phi i64 [ 1, %entry ], [ %mul, %recurse ]
  ret i64 %result
}

; Main function that calls factorial
define i32 @main() {
entry:
  %call = call i64 @factorial(i64 5)
  %conv = trunc i64 %call to i32
  ret i32 %conv
} 