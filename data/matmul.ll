; Simple 2x2 matrix multiplication in LLVM IR
; C = A * B where A, B, C are 2x2 matrices

target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function to perform 2x2 matrix multiplication
define void @matmul_2x2(float* %A, float* %B, float* %C) {
entry:
  ; C[0][0] = A[0][0] * B[0][0] + A[0][1] * B[1][0]
  %A00 = load float, float* %A, align 4
  %A_idx_1 = getelementptr float, float* %A, i64 1
  %A01 = load float, float* %A_idx_1, align 4
  
  %B00 = load float, float* %B, align 4
  %B_idx_2 = getelementptr float, float* %B, i64 2
  %B10 = load float, float* %B_idx_2, align 4
  
  %mul1 = fmul float %A00, %B00
  %mul2 = fmul float %A01, %B10
  %sum1 = fadd float %mul1, %mul2
  store float %sum1, float* %C, align 4
  
  ; C[0][1] = A[0][0] * B[0][1] + A[0][1] * B[1][1]
  %B_idx_1 = getelementptr float, float* %B, i64 1
  %B01 = load float, float* %B_idx_1, align 4
  %B_idx_3 = getelementptr float, float* %B, i64 3
  %B11 = load float, float* %B_idx_3, align 4
  
  %mul3 = fmul float %A00, %B01
  %mul4 = fmul float %A01, %B11
  %sum2 = fadd float %mul3, %mul4
  %C_idx_1 = getelementptr float, float* %C, i64 1
  store float %sum2, float* %C_idx_1, align 4
  
  ; C[1][0] = A[1][0] * B[0][0] + A[1][1] * B[1][0]
  %A_idx_2 = getelementptr float, float* %A, i64 2
  %A10 = load float, float* %A_idx_2, align 4
  %A_idx_3 = getelementptr float, float* %A, i64 3
  %A11 = load float, float* %A_idx_3, align 4
  
  %mul5 = fmul float %A10, %B00
  %mul6 = fmul float %A11, %B10
  %sum3 = fadd float %mul5, %mul6
  %C_idx_2 = getelementptr float, float* %C, i64 2
  store float %sum3, float* %C_idx_2, align 4
  
  ; C[1][1] = A[1][0] * B[0][1] + A[1][1] * B[1][1]
  %mul7 = fmul float %A10, %B01
  %mul8 = fmul float %A11, %B11
  %sum4 = fadd float %mul7, %mul8
  %C_idx_3 = getelementptr float, float* %C, i64 3
  store float %sum4, float* %C_idx_3, align 4
  
  ret void
}

; Main function that initializes matrices and calls matmul
define i32 @main() {
entry:
  ; Allocate memory for matrices
  %A = alloca [4 x float], align 16
  %B = alloca [4 x float], align 16
  %C = alloca [4 x float], align 16
  
  ; Initialize matrix A with values [1.0, 2.0, 3.0, 4.0]
  %A_ptr = getelementptr [4 x float], [4 x float]* %A, i64 0, i64 0
  store float 1.0, float* %A_ptr, align 4
  %A_ptr_1 = getelementptr [4 x float], [4 x float]* %A, i64 0, i64 1
  store float 2.0, float* %A_ptr_1, align 4
  %A_ptr_2 = getelementptr [4 x float], [4 x float]* %A, i64 0, i64 2
  store float 3.0, float* %A_ptr_2, align 4
  %A_ptr_3 = getelementptr [4 x float], [4 x float]* %A, i64 0, i64 3
  store float 4.0, float* %A_ptr_3, align 4
  
  ; Initialize matrix B with values [5.0, 6.0, 7.0, 8.0]
  %B_ptr = getelementptr [4 x float], [4 x float]* %B, i64 0, i64 0
  store float 5.0, float* %B_ptr, align 4
  %B_ptr_1 = getelementptr [4 x float], [4 x float]* %B, i64 0, i64 1
  store float 6.0, float* %B_ptr_1, align 4
  %B_ptr_2 = getelementptr [4 x float], [4 x float]* %B, i64 0, i64 2
  store float 7.0, float* %B_ptr_2, align 4
  %B_ptr_3 = getelementptr [4 x float], [4 x float]* %B, i64 0, i64 3
  store float 8.0, float* %B_ptr_3, align 4
  
  ; Call matrix multiplication function
  call void @matmul_2x2(float* %A_ptr, float* %B_ptr, float* getelementptr ([4 x float], [4 x float]* %C, i64 0, i64 0))
  
  ; For simplicity, just return 0
  ret i32 0
} 