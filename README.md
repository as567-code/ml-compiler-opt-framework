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