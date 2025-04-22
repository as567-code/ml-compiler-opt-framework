# src/passes/CMakeLists.txt

# Collect all pass source files
set(PASSES_SOURCES
  MemoryCoalescing.cpp
)

# Create a static library for passes
add_library(passes STATIC ${PASSES_SOURCES})

# Include directories
target_include_directories(passes PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Link against LLVM
target_link_libraries(passes PRIVATE ${LLVM_LIBS}) 