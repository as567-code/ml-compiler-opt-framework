# src/CMakeLists.txt

# Create the main executable
add_executable(ml_compiler main.cpp)

# Link LLVM
target_link_libraries(ml_compiler PRIVATE ${LLVM_LIBS})

# Set installation targets
install(TARGETS ml_compiler DESTINATION bin) 