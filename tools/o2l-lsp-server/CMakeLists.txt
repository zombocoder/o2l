cmake_minimum_required(VERSION 3.16)
project(o2l-lsp-server CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the executable
add_executable(o2l-lsp-server
    main.cpp
)

# Set compiler flags
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(o2l-lsp-server PRIVATE
        -Wall
        -Wextra
        -O2
    )
endif()

# Set output directory to build/bin
set_target_properties(o2l-lsp-server PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Install target
install(TARGETS o2l-lsp-server
    RUNTIME DESTINATION .
)