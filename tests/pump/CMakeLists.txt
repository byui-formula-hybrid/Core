# tests/pump/CMakeLists.txt

# Set module name
set(MODULE_NAME pump)
set(TEST_NAME test_${MODULE_NAME})

# Collect all test source files in this module
file(GLOB TEST_SOURCES "*.cpp")

# Use the Conan-provided GTest package (CONFIG mode)
find_package(GTest REQUIRED CONFIG)

# Create the test executable
add_executable(${TEST_NAME} ${TEST_SOURCES})

# Link against the corresponding CoreLib module and Google Test
target_link_libraries(${TEST_NAME} PRIVATE corelib_${MODULE_NAME} GTest::gtest GTest::gtest_main)

# Register the test with CTest
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})