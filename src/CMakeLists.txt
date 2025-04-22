# src/CMakeLists.txt

# Collect all source files
set(SOURCES
  main.cpp
  passes/MemoryCoalescing.cpp
)

# Create the main executable
add_executable(ml_compiler ${SOURCES})

# Include directories
target_include_directories(ml_compiler PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Link LLVM
target_link_libraries(ml_compiler PRIVATE ${LLVM_LIBS})

# Set installation targets
install(TARGETS ml_compiler DESTINATION bin) 