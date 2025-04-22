# bench/CMakeLists.txt

# Add the benchmarking executable
add_executable(bench_optimizer benchmark.cpp)

# Link with required libraries
target_link_libraries(bench_optimizer PRIVATE ${LLVM_LIBS} passes)

# Include directories
target_include_directories(bench_optimizer PRIVATE ${CMAKE_SOURCE_DIR}/src) 