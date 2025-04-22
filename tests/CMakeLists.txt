# tests/CMakeLists.txt

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/src)

# Create tests directory for IR files if it doesn't exist
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test_files)

# Add basic test
add_executable(test_basic test_basic.cpp)
target_link_libraries(test_basic PRIVATE ${GTEST_LIBRARIES} pthread)
add_test(NAME BasicTest COMMAND test_basic)

# Add memory coalescing test
add_executable(test_memory_coalescing test_memory_coalescing.cpp)
target_link_libraries(test_memory_coalescing PRIVATE 
    ${GTEST_LIBRARIES} 
    ${LLVM_LIBS}
    passes
    pthread)
target_include_directories(test_memory_coalescing PRIVATE ${CMAKE_SOURCE_DIR}/src)
add_test(NAME MemoryCoalescingTest COMMAND test_memory_coalescing)

# Add memory coalescing IR file test
add_executable(test_memory_coalescing_ir test_memory_coalescing_ir.cpp)
target_link_libraries(test_memory_coalescing_ir PRIVATE 
    ${GTEST_LIBRARIES} 
    ${LLVM_LIBS}
    passes
    pthread)
target_include_directories(test_memory_coalescing_ir PRIVATE ${CMAKE_SOURCE_DIR}/src)
add_test(NAME MemoryCoalescingIRTest COMMAND test_memory_coalescing_ir)

# Make sure CTest knows about all the tests
include(CTest)
set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1) 