cmake_minimum_required(VERSION 3.10)
project(ml-compiler-opt-framework VERSION 0.1.0 LANGUAGES C CXX)

# Enable testing at top level
enable_testing()
include(CTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find LLVM (version ≥ 10)
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "  - LLVM_DIR: ${LLVM_DIR}")

# Make LLVM headers available
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Link against LLVM libraries by passing targets
set(LLVM_COMPONENTS
  Core
  IRReader
  Support
  Analysis
  TransformUtils
  ScalarOpts
  IPO
  Passes
)

llvm_map_components_to_libnames(LLVM_LIBS ${LLVM_COMPONENTS})

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Add source directory
add_subdirectory(src)

# Add tests directory
add_subdirectory(tests)

# Add benchmark directory
add_subdirectory(bench) 