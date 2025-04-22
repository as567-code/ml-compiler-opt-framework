# src/CMakeLists.txt

# Create the main executable
add_executable(ml_compiler_opt main.cpp)

# Set installation targets
install(TARGETS ml_compiler_opt DESTINATION bin) 