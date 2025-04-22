; Simple 2x2 matrix multiplication in LLVM IR
; C[i,j] = sum_k A[i,k] * B[k,j]

target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function to multiply two 2x2 matrices
; A: 2x2 matrix (float*)
; B: 2x2 matrix (float*)
; C: 2x2 matrix (float*) - result of multiplication
define void @matmul_2x2(float* %A, float* %B, float* %C) {
entry:
  ; C[0,0] = A[0,0]*B[0,0] + A[0,1]*B[1,0]
  
  ; Load A[0,0]
  %A00_ptr = getelementptr float, float* %A, i32 0
  %A00 = load float, float* %A00_ptr, align 4
  
  ; Load B[0,0]
  %B00_ptr = getelementptr float, float* %B, i32 0
  %B00 = load float, float* %B00_ptr, align 4
  
  ; Load A[0,1]
  %A01_ptr = getelementptr float, float* %A, i32 1
  %A01 = load float, float* %A01_ptr, align 4
  
  ; Load B[1,0]
  %B10_ptr = getelementptr float, float* %B, i32 2
  %B10 = load float, float* %B10_ptr, align 4
  
  ; Compute A[0,0]*B[0,0]
  %mul1 = fmul float %A00, %B00
  
  ; Compute A[0,1]*B[1,0]
  %mul2 = fmul float %A01, %B10
  
  ; Compute C[0,0] = A[0,0]*B[0,0] + A[0,1]*B[1,0]
  %sum1 = fadd float %mul1, %mul2
  
  ; Store C[0,0]
  %C00_ptr = getelementptr float, float* %C, i32 0
  store float %sum1, float* %C00_ptr, align 4
  
  ; C[0,1] = A[0,0]*B[0,1] + A[0,1]*B[1,1]
  
  ; Load B[0,1]
  %B01_ptr = getelementptr float, float* %B, i32 1
  %B01 = load float, float* %B01_ptr, align 4
  
  ; Load B[1,1]
  %B11_ptr = getelementptr float, float* %B, i32 3
  %B11 = load float, float* %B11_ptr, align 4
  
  ; Compute A[0,0]*B[0,1]
  %mul3 = fmul float %A00, %B01
  
  ; Compute A[0,1]*B[1,1]
  %mul4 = fmul float %A01, %B11
  
  ; Compute C[0,1] = A[0,0]*B[0,1] + A[0,1]*B[1,1]
  %sum2 = fadd float %mul3, %mul4
  
  ; Store C[0,1]
  %C01_ptr = getelementptr float, float* %C, i32 1
  store float %sum2, float* %C01_ptr, align 4
  
  ; C[1,0] = A[1,0]*B[0,0] + A[1,1]*B[1,0]
  
  ; Load A[1,0]
  %A10_ptr = getelementptr float, float* %A, i32 2
  %A10 = load float, float* %A10_ptr, align 4
  
  ; Load A[1,1]
  %A11_ptr = getelementptr float, float* %A, i32 3
  %A11 = load float, float* %A11_ptr, align 4
  
  ; Compute A[1,0]*B[0,0]
  %mul5 = fmul float %A10, %B00
  
  ; Compute A[1,1]*B[1,0]
  %mul6 = fmul float %A11, %B10
  
  ; Compute C[1,0] = A[1,0]*B[0,0] + A[1,1]*B[1,0]
  %sum3 = fadd float %mul5, %mul6
  
  ; Store C[1,0]
  %C10_ptr = getelementptr float, float* %C, i32 2
  store float %sum3, float* %C10_ptr, align 4
  
  ; C[1,1] = A[1,0]*B[0,1] + A[1,1]*B[1,1]
  
  ; Compute A[1,0]*B[0,1]
  %mul7 = fmul float %A10, %B01
  
  ; Compute A[1,1]*B[1,1]
  %mul8 = fmul float %A11, %B11
  
  ; Compute C[1,1] = A[1,0]*B[0,1] + A[1,1]*B[1,1]
  %sum4 = fadd float %mul7, %mul8
  
  ; Store C[1,1]
  %C11_ptr = getelementptr float, float* %C, i32 3
  store float %sum4, float* %C11_ptr, align 4
  
  ret void
}

; Main function to test matrix multiplication
define i32 @main() {
entry:
  ; Allocate memory for the matrices
  %A = alloca [4 x float], align 16
  %B = alloca [4 x float], align 16
  %C = alloca [4 x float], align 16
  
  ; Initialize A with values [1.0, 2.0, 3.0, 4.0]
  %A0_ptr = getelementptr [4 x float], [4 x float]* %A, i32 0, i32 0
  store float 1.0, float* %A0_ptr, align 4
  %A1_ptr = getelementptr [4 x float], [4 x float]* %A, i32 0, i32 1
  store float 2.0, float* %A1_ptr, align 4
  %A2_ptr = getelementptr [4 x float], [4 x float]* %A, i32 0, i32 2
  store float 3.0, float* %A2_ptr, align 4
  %A3_ptr = getelementptr [4 x float], [4 x float]* %A, i32 0, i32 3
  store float 4.0, float* %A3_ptr, align 4
  
  ; Initialize B with values [5.0, 6.0, 7.0, 8.0]
  %B0_ptr = getelementptr [4 x float], [4 x float]* %B, i32 0, i32 0
  store float 5.0, float* %B0_ptr, align 4
  %B1_ptr = getelementptr [4 x float], [4 x float]* %B, i32 0, i32 1
  store float 6.0, float* %B1_ptr, align 4
  %B2_ptr = getelementptr [4 x float], [4 x float]* %B, i32 0, i32 2
  store float 7.0, float* %B2_ptr, align 4
  %B3_ptr = getelementptr [4 x float], [4 x float]* %B, i32 0, i32 3
  store float 8.0, float* %B3_ptr, align 4
  
  ; Call matrix multiplication function
  %A_base_ptr = getelementptr [4 x float], [4 x float]* %A, i32 0, i32 0
  %B_base_ptr = getelementptr [4 x float], [4 x float]* %B, i32 0, i32 0
  %C_base_ptr = getelementptr [4 x float], [4 x float]* %C, i32 0, i32 0
  call void @matmul_2x2(float* %A_base_ptr, float* %B_base_ptr, float* %C_base_ptr)
  
  ret i32 0
} 