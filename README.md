# ML Compiler Optimization Framework

![CI](https://github.com/yourusername/ml-compiler-opt-framework/actions/workflows/ci.yml/badge.svg)

A framework for machine learning-based compiler optimizations focused on high-performance GPU code generation.

## Overview

This project provides a compiler optimization framework built on LLVM that targets machine learning workloads. It implements specialized optimization passes for improving memory access patterns, loop transformations, and GPU kernel generation.

## Features

- **Custom Memory Coalescing Pass**: Detects and optimizes strided memory access patterns for better GPU performance
- **Sample IR Files**: Pre-built LLVM IR examples for testing, including matrix multiplication and convolution
- **Benchmark Harness**: Infrastructure for measuring optimization improvements
- **Test Suite**: Comprehensive tests for all compiler passes

## Requirements

- CMake 3.10+
- LLVM 10.0+
- A C++17 compatible compiler
- Google Test for running the test suite

## Installation

### macOS

```bash
# Install dependencies
brew install llvm cmake googletest

# Clone the repository
git clone https://github.com/yourusername/ml-compiler-opt-framework.git
cd ml-compiler-opt-framework

# Build
mkdir -p build && cd build
export PATH="/opt/homebrew/opt/llvm/bin:$PATH"
export LDFLAGS="-L/opt/homebrew/opt/llvm/lib" 
export CPPFLAGS="-I/opt/homebrew/opt/llvm/include"
cmake ..
make
```

### Linux

```bash
# Install dependencies
sudo apt-get update
sudo apt-get install -y cmake llvm llvm-dev libgtest-dev

# Clone the repository
git clone https://github.com/yourusername/ml-compiler-opt-framework.git
cd ml-compiler-opt-framework

# Build
mkdir -p build && cd build
cmake ..
make
```

## Usage

### Running the Optimizer

```bash
# Optimize an LLVM IR file
./src/ml_compiler ../data/matmul.ll > matmul_opt.ll

# Compare the original and optimized files
diff -u ../data/matmul.ll matmul_opt.ll
```

### Running Benchmarks

```bash
# Run benchmarks on sample IR files
./bench/bench_optimizer ../data/matmul.ll ../data/conv2d.ll
```

### Running Tests

```bash
# Run all tests
ctest

# Run a specific test
./tests/test_memory_coalescing
```

## Project Structure

```
ml-compiler-opt-framework/
├── src/                # Source code
│   ├── main.cpp        # Main compiler driver
│   └── passes/         # Custom optimization passes
├── tests/              # Test suite
├── data/               # Sample IR files
├── bench/              # Benchmarking tools
└── docs/               # Documentation
```

## Roadmap

See [ROADMAP.md](ROADMAP.md) for the project development timeline and upcoming features.

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Memory Coalescing Pass

The Memory Coalescing pass analyzes and transforms memory access patterns to improve memory coalescing on GPU architectures. Coalesced memory access is critical for GPU performance, as it allows multiple threads in a warp to access memory in a single transaction rather than requiring separate memory transactions for each thread.

Key features of the pass:
- Detects strided memory access patterns in loops
- Identifies adjacent memory accesses that can be merged
- Transforms code to improve memory access efficiency

### Implementation

The pass is implemented in:
- `src/passes/MemoryCoalescing.h` - Pass declaration
- `src/passes/MemoryCoalescing.cpp` - Pass implementation

## Building the Project

```bash
mkdir build
cd build
cmake ..
make
```

## Running the Tests

To run all tests:
```bash
cd build
ctest
```

To run specific tests:
```bash
./tests/test_memory_coalescing
./tests/test_memory_coalescing_ir
```

### Test IR Files

The `tests/test_files/` directory contains LLVM IR files used for testing the Memory Coalescing pass:

- `strided_access.ll` - Contains functions with various strided memory access patterns

## Using the Optimization Pass

The Memory Coalescing pass can be run on LLVM IR files using the main executable:

```bash
./src/ml_compiler input.ll
```

This will apply the Memory Coalescing pass along with standard LLVM optimizations and output the optimized IR.

## Extending the Framework

To add new optimization passes:

1. Create a new pass header in `src/passes/`
2. Implement the pass in a corresponding .cpp file
3. Register the pass in the main.cpp file or in the PassBuilder
4. Add tests in the `tests/` directory # interactive-ml-model-visualization-tool
# interactive-ml-model-visualization-tool
