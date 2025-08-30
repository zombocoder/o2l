cmake_minimum_required(VERSION 3.20)
project(o2l-fmt VERSION 0.0.1)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable
add_executable(o2l-fmt main.cpp formatter.cpp)

# Find Google Test
find_package(GTest REQUIRED)

# Add test executables
add_executable(o2l-fmt-test tests/test_formatter_gtest.cpp formatter.cpp)
target_link_libraries(o2l-fmt-test GTest::gtest GTest::gtest_main)

add_executable(o2l-fmt-advanced-test tests/test_cases.cpp formatter.cpp)
target_link_libraries(o2l-fmt-advanced-test GTest::gtest GTest::gtest_main)

# Enable testing
enable_testing()
add_test(NAME FormatterBasicTests COMMAND o2l-fmt-test)
add_test(NAME FormatterAdvancedTests COMMAND o2l-fmt-advanced-test)

# Set output directory to build/bin
set_target_properties(o2l-fmt PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Enable all warnings
if(MSVC)
    target_compile_options(o2l-fmt PRIVATE /W4)
else()
    target_compile_options(o2l-fmt PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Install target
install(TARGETS o2l-fmt
    RUNTIME DESTINATION .
)