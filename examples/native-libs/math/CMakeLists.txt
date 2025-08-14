cmake_minimum_required(VERSION 3.16)
project(O2LMathLibrary)

# Set C++ standard to C++23 to match O²L
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create shared library
add_library(math SHARED MathLibrary.cpp)

# Include O²L headers
target_include_directories(math PRIVATE ../../../src)

# Set output name to follow platform conventions
set_target_properties(math PROPERTIES
    PREFIX "lib"           # Ensure lib prefix on all platforms
    OUTPUT_NAME "math"     # Base name
)

# Platform-specific settings
if(APPLE)
    set_target_properties(math PROPERTIES
        SUFFIX ".dylib"
    )
elseif(WIN32)
    set_target_properties(math PROPERTIES
        SUFFIX ".dll"
    )
else()
    set_target_properties(math PROPERTIES
        SUFFIX ".so"
    )
endif()

# Installation
install(TARGETS math
    LIBRARY DESTINATION lib/o2l
    RUNTIME DESTINATION lib/o2l  # For Windows DLLs
)

# Copy to build directory for testing
add_custom_command(TARGET math POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:math> ${CMAKE_BINARY_DIR}/
)