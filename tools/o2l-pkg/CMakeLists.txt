cmake_minimum_required(VERSION 3.20)
project(o2l-pkg VERSION 0.0.1)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable
add_executable(o2l-pkg main.cpp)

# Set output directory
set_target_properties(o2l-pkg PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Enable all warnings
if(MSVC)
    target_compile_options(o2l-pkg PRIVATE /W4)
else()
    target_compile_options(o2l-pkg PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Install target
install(TARGETS o2l-pkg
    RUNTIME DESTINATION .
)