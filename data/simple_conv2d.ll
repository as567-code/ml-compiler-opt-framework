; Simple 2D convolution in LLVM IR (3x3 kernel on 5x5 input)
; Output[i,j] = sum_m sum_n Input[i+m,j+n] * Kernel[m,n]

target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function to perform 2D convolution
; Input: 5x5 matrix (float*)
; Kernel: 3x3 matrix (float*)
; Output: 3x3 matrix (float*) - result of convolution
define void @conv2d_3x3(float* %input, float* %kernel, float* %output) {
entry:
  br label %outer_loop_header

outer_loop_header:                              
  %i = phi i32 [ 0, %entry ], [ %i_next, %outer_loop_latch ]
  %cond1 = icmp slt i32 %i, 3
  br i1 %cond1, label %inner_loop_header, label %return

inner_loop_header:                              
  %j = phi i32 [ 0, %outer_loop_header ], [ %j_next, %inner_loop_latch ]
  %cond2 = icmp slt i32 %j, 3
  br i1 %cond2, label %conv_loop, label %outer_loop_latch

conv_loop:                                      
  ; Initialize accumulator
  %acc = alloca float, align 4
  store float 0.0, float* %acc, align 4
  
  ; Perform convolution for output[i][j]
  br label %kernel_loop_i

kernel_loop_i:                                  
  %m = phi i32 [ 0, %conv_loop ], [ %m_next, %kernel_loop_j_end ]
  %cond_m = icmp slt i32 %m, 3
  br i1 %cond_m, label %kernel_loop_j, label %conv_end

kernel_loop_j:                                  
  %n = phi i32 [ 0, %kernel_loop_i ], [ %n_next, %kernel_loop_j_body ]
  %cond_n = icmp slt i32 %n, 3
  br i1 %cond_n, label %kernel_loop_j_body, label %kernel_loop_j_end

kernel_loop_j_body:                             
  ; Compute input and kernel indices
  %i_plus_m = add i32 %i, %m
  %j_plus_n = add i32 %j, %n
  
  ; Access input[i+m][j+n]
  %input_row_offset = mul i32 %i_plus_m, 5
  %input_idx = add i32 %input_row_offset, %j_plus_n
  %input_ptr = getelementptr float, float* %input, i32 %input_idx
  %input_val = load float, float* %input_ptr, align 4
  
  ; Access kernel[m][n]
  %kernel_row_offset = mul i32 %m, 3
  %kernel_idx = add i32 %kernel_row_offset, %n
  %kernel_ptr = getelementptr float, float* %kernel, i32 %kernel_idx
  %kernel_val = load float, float* %kernel_ptr, align 4
  
  ; Multiply and accumulate
  %mul = fmul float %input_val, %kernel_val
  %curr_acc = load float, float* %acc, align 4
  %new_acc = fadd float %curr_acc, %mul
  store float %new_acc, float* %acc, align 4
  
  %n_next = add i32 %n, 1
  br label %kernel_loop_j

kernel_loop_j_end:                               
  %m_next = add i32 %m, 1
  br label %kernel_loop_i

conv_end:                                       
  ; Store result to output[i][j]
  %result = load float, float* %acc, align 4
  %output_row_offset = mul i32 %i, 3
  %output_idx = add i32 %output_row_offset, %j
  %output_ptr = getelementptr float, float* %output, i32 %output_idx
  store float %result, float* %output_ptr, align 4
  
  %j_next = add i32 %j, 1
  br label %inner_loop_latch

inner_loop_latch:                               
  br label %inner_loop_header

outer_loop_latch:                               
  %i_next = add i32 %i, 1
  br label %outer_loop_header

return:                                         
  ret void
}

; Main function to test convolution - simplified
define i32 @main() {
entry:
  ; Allocate memory for the matrices
  %input = alloca [25 x float], align 16  ; 5x5 input
  %kernel = alloca [9 x float], align 16  ; 3x3 kernel
  %output = alloca [9 x float], align 16  ; 3x3 output
  
  ; Initialize kernel with value 0.1 for all elements except center (0.2)
  ; Manually initialize all kernel elements
  %kernel_ptr_0 = getelementptr [9 x float], [9 x float]* %kernel, i32 0, i32 0
  store float 0.1, float* %kernel_ptr_0, align 4
  %kernel_ptr_1 = getelementptr [9 x float], [9 x float]* %kernel, i32 0, i32 1
  store float 0.1, float* %kernel_ptr_1, align 4
  %kernel_ptr_2 = getelementptr [9 x float], [9 x float]* %kernel, i32 0, i32 2
  store float 0.1, float* %kernel_ptr_2, align 4
  %kernel_ptr_3 = getelementptr [9 x float], [9 x float]* %kernel, i32 0, i32 3
  store float 0.1, float* %kernel_ptr_3, align 4
  %kernel_ptr_4 = getelementptr [9 x float], [9 x float]* %kernel, i32 0, i32 4
  store float 0.2, float* %kernel_ptr_4, align 4  ; Center has higher weight
  %kernel_ptr_5 = getelementptr [9 x float], [9 x float]* %kernel, i32 0, i32 5
  store float 0.1, float* %kernel_ptr_5, align 4
  %kernel_ptr_6 = getelementptr [9 x float], [9 x float]* %kernel, i32 0, i32 6
  store float 0.1, float* %kernel_ptr_6, align 4
  %kernel_ptr_7 = getelementptr [9 x float], [9 x float]* %kernel, i32 0, i32 7
  store float 0.1, float* %kernel_ptr_7, align 4
  %kernel_ptr_8 = getelementptr [9 x float], [9 x float]* %kernel, i32 0, i32 8
  store float 0.1, float* %kernel_ptr_8, align 4
  
  ; Initialize input matrix with ramp values (0 to 24)
  %input_ptr_0 = getelementptr [25 x float], [25 x float]* %input, i32 0, i32 0
  store float 0.0, float* %input_ptr_0, align 4
  %input_ptr_1 = getelementptr [25 x float], [25 x float]* %input, i32 0, i32 1
  store float 1.0, float* %input_ptr_1, align 4
  ; And so on... (simplified to just initialize first few elements)
  
  ; Call convolution function
  %input_base = getelementptr [25 x float], [25 x float]* %input, i32 0, i32 0
  %kernel_base = getelementptr [9 x float], [9 x float]* %kernel, i32 0, i32 0
  %output_base = getelementptr [9 x float], [9 x float]* %output, i32 0, i32 0
  call void @conv2d_3x3(float* %input_base, float* %kernel_base, float* %output_base)
  
  ret i32 0
} 