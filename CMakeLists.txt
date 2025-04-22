cmake_minimum_required(VERSION 3.10)
project(ml-compiler-opt-framework VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Add source directory
add_subdirectory(src)

# Add tests directory
add_subdirectory(tests) 