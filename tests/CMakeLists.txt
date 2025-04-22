# tests/CMakeLists.txt
enable_testing()

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(test_basic test_basic.cpp)
target_link_libraries(test_basic PRIVATE ${GTEST_LIBRARIES} pthread)

add_test(NAME BasicTest COMMAND test_basic) 