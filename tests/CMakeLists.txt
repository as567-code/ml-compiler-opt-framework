# tests/CMakeLists.txt
enable_testing()

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/src)

# Add basic test
add_executable(test_basic test_basic.cpp)
target_link_libraries(test_basic PRIVATE ${GTEST_LIBRARIES} pthread)
add_test(NAME BasicTest COMMAND test_basic)

# Add memory coalescing test
add_executable(test_memory_coalescing test_memory_coalescing.cpp)
target_link_libraries(test_memory_coalescing PRIVATE 
    ${GTEST_LIBRARIES} 
    ${LLVM_LIBS}
    pthread)
target_include_directories(test_memory_coalescing PRIVATE ${CMAKE_SOURCE_DIR}/src)
add_test(NAME MemoryCoalescingTest COMMAND test_memory_coalescing) 