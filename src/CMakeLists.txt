# src/CMakeLists.txt

# Add the passes subdirectory
add_subdirectory(passes)

# Collect main source files
set(SOURCES
  main.cpp
)

# Create the main executable
add_executable(ml_compiler ${SOURCES})

# Include directories
target_include_directories(ml_compiler PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Link LLVM and passes library
target_link_libraries(ml_compiler PRIVATE ${LLVM_LIBS} passes)

# Set installation targets
install(TARGETS ml_compiler DESTINATION bin) 