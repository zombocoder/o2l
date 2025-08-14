cmake_minimum_required(VERSION 3.20)
project(o2l-fmt VERSION 0.0.1)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable
add_executable(o2l-fmt main.cpp formatter.cpp)

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