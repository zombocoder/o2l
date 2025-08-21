# Copyright 2021 O²L Programming Language
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 3.20)
project(o2l VERSION 0.0.1 LANGUAGES CXX)

# Require C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler-specific options
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "12.0")
        message(FATAL_ERROR "GCC version must be at least 12.0 for C++23 support")
    endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "15.0")
        message(FATAL_ERROR "Clang version must be at least 15.0 for C++23 support")
    endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "19.35")
        message(FATAL_ERROR "MSVC version must be at least 19.35 (Visual Studio 2022 17.5) for C++23 support")
    endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()

# Feature flags
option(ENABLE_NAMESPACES "Enable namespace functionality (experimental)" OFF)

# Debug/Release configurations
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# Configure feature flags
if(ENABLE_NAMESPACES)
    add_compile_definitions(O2L_ENABLE_NAMESPACES=1)
    message(STATUS "Namespace functionality: ENABLED")
else()
    add_compile_definitions(O2L_ENABLE_NAMESPACES=0)
    message(STATUS "Namespace functionality: DISABLED")
endif()

# Source files
set(SOURCES
    src/main.cpp
    src/Lexer.cpp
    src/Parser.cpp
    src/Interpreter.cpp
    src/AST/Node.cpp
    src/AST/ObjectNode.cpp
    src/AST/MethodCallNode.cpp
    src/AST/MethodDeclarationNode.cpp
    src/AST/ConstructorDeclarationNode.cpp
    src/AST/IdentifierNode.cpp
    src/AST/ImportNode.cpp
    src/AST/NewExpressionNode.cpp
    src/AST/PropertyAccessNode.cpp
    src/AST/PropertyAssignmentNode.cpp
    src/AST/ThisNode.cpp
    src/AST/PropertyDeclarationNode.cpp
    src/AST/FunctionCallNode.cpp
    src/AST/ConstDeclarationNode.cpp
    src/AST/VariableDeclarationNode.cpp
    src/AST/VariableAssignmentNode.cpp
    src/AST/BlockNode.cpp
    src/AST/LiteralNode.cpp
    src/AST/BinaryOpNode.cpp
    src/AST/ReturnNode.cpp
    src/AST/IfStatementNode.cpp
    src/AST/WhileStatementNode.cpp
    src/AST/BreakNode.cpp
    src/AST/ComparisonNode.cpp
    src/AST/LogicalNode.cpp
    src/AST/UnaryNode.cpp
    src/AST/EnumDeclarationNode.cpp
    src/AST/EnumAccessNode.cpp
    src/AST/RecordDeclarationNode.cpp
    src/AST/RecordInstantiationNode.cpp
    src/AST/RecordFieldAccessNode.cpp
    src/AST/MemberAccessNode.cpp
    src/AST/ProtocolDeclarationNode.cpp
    src/AST/NamespaceNode.cpp
    src/AST/QualifiedIdentifierNode.cpp
    src/AST/ThrowNode.cpp
    src/AST/TryCatchFinallyNode.cpp
    src/AST/JsonSerializer.cpp
    src/Runtime/Value.cpp
    src/Runtime/ObjectInstance.cpp
    src/Runtime/Context.cpp
    src/Runtime/ModuleLoader.cpp
    src/Runtime/SystemLibrary.cpp
    src/Runtime/MathLibrary.cpp
    src/Runtime/TestLibrary.cpp
    src/Runtime/DateTimeLibrary.cpp
    src/Runtime/RegexpLibrary.cpp
    src/Runtime/UrlLibrary.cpp
    src/Runtime/JsonLibrary.cpp
    src/Runtime/HttpClientLibrary.cpp
    src/Runtime/HttpServerLibrary.cpp
    src/Runtime/EnumInstance.cpp
    src/Runtime/RecordType.cpp
    src/Runtime/RecordInstance.cpp
    src/Runtime/ProtocolInstance.cpp
    src/Runtime/ListInstance.cpp
    src/Runtime/ListIterator.cpp
    src/Runtime/RepeatIterator.cpp
    src/Runtime/MapInstance.cpp
    src/Runtime/MapIterator.cpp
    src/Runtime/MapObject.cpp
    src/Runtime/SetInstance.cpp
    src/Runtime/SetIterator.cpp
    src/Runtime/ErrorInstance.cpp
    src/Runtime/ResultInstance.cpp
    src/Runtime/NativeLibrary.cpp
    src/Runtime/DynamicLibraryManager.cpp
    src/Runtime/FFILibrary.cpp
    src/Runtime/FFI/SharedLibrary.cpp
    src/Runtime/FFI/FFITypes.cpp
    src/Runtime/FFI/FFIEngine.cpp
    src/Common/Exceptions.cpp
)

# Header files (for IDE support)
set(HEADERS
    src/Lexer.hpp
    src/Parser.hpp
    src/Interpreter.hpp
    src/AST/Node.hpp
    src/AST/ObjectNode.hpp
    src/AST/MethodCallNode.hpp
    src/AST/MethodDeclarationNode.hpp
    src/AST/ConstructorDeclarationNode.hpp
    src/AST/IdentifierNode.hpp
    src/AST/ImportNode.hpp
    src/AST/NewExpressionNode.hpp
    src/AST/PropertyAccessNode.hpp
    src/AST/PropertyAssignmentNode.hpp
    src/AST/PropertyDeclarationNode.hpp
    src/AST/FunctionCallNode.hpp
    src/AST/ConstDeclarationNode.hpp
    src/AST/VariableDeclarationNode.hpp
    src/AST/VariableAssignmentNode.hpp
    src/AST/BlockNode.hpp
    src/AST/LiteralNode.hpp
    src/AST/BinaryOpNode.hpp
    src/AST/ReturnNode.hpp
    src/AST/IfStatementNode.hpp
    src/AST/WhileStatementNode.hpp
    src/AST/BreakNode.hpp
    src/AST/ComparisonNode.hpp
    src/AST/LogicalNode.hpp
    src/AST/UnaryNode.hpp
    src/AST/EnumDeclarationNode.hpp
    src/AST/EnumAccessNode.hpp
    src/AST/RecordDeclarationNode.hpp
    src/AST/RecordInstantiationNode.hpp
    src/AST/RecordFieldAccessNode.hpp
    src/AST/MemberAccessNode.hpp
    src/AST/ProtocolDeclarationNode.hpp
    src/AST/JsonSerializer.hpp
    src/Runtime/Value.hpp
    src/Runtime/ObjectInstance.hpp
    src/Runtime/Context.hpp
    src/Runtime/ModuleLoader.hpp
    src/Runtime/SystemLibrary.hpp
    src/Runtime/MathLibrary.hpp
    src/Runtime/TestLibrary.hpp
    src/Runtime/DateTimeLibrary.hpp
    src/Runtime/RegexpLibrary.hpp
    src/Runtime/UrlLibrary.hpp
    src/Runtime/JsonLibrary.hpp
    src/Runtime/HttpClientLibrary.hpp
    src/Runtime/EnumInstance.hpp
    src/Runtime/RecordType.hpp
    src/Runtime/RecordInstance.hpp
    src/Runtime/ProtocolInstance.hpp
    src/Runtime/ListInstance.hpp
    src/Runtime/ListIterator.hpp
    src/Runtime/RepeatIterator.hpp
    src/Runtime/MapInstance.hpp
    src/Runtime/MapIterator.hpp
    src/Runtime/MapObject.hpp
    src/Runtime/SetInstance.hpp
    src/Runtime/SetIterator.hpp
    src/Runtime/NativeLibrary.hpp
    src/Runtime/DynamicLibraryManager.hpp
    src/Runtime/FFILibrary.hpp
    src/Runtime/FFI/SharedLibrary.hpp
    src/Runtime/FFI/FFITypes.hpp
    src/Runtime/FFI/FFIEngine.hpp
    src/Common/Exceptions.hpp
)

# Create executable
add_executable(o2l ${SOURCES} ${HEADERS})

# Set output directory to build/bin
set_target_properties(o2l PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Include directories
target_include_directories(o2l PRIVATE src)

# Find and link libffi
find_package(PkgConfig)
if(PkgConfig_FOUND)
    pkg_check_modules(FFI libffi)
    if(FFI_FOUND)
        target_include_directories(o2l PRIVATE ${FFI_INCLUDE_DIRS})
        target_link_libraries(o2l ${FFI_LIBRARIES})
        target_compile_definitions(o2l PRIVATE HAVE_FFI=1)
    else()
        message(WARNING "libffi not found - FFI will use minimal fallback")
    endif()
else()
    message(WARNING "pkg-config not found - FFI will use minimal fallback")
endif()

# Platform-specific linking for dynamic library support
if(WIN32)
    # Windows linking for system libraries
    target_link_libraries(o2l wininet ws2_32)
elseif(APPLE)
    # macOS
    target_link_libraries(o2l ${CMAKE_DL_LIBS})
else()
    # Linux and other Unix-like systems
    target_link_libraries(o2l ${CMAKE_DL_LIBS})
endif()

# Enable testing
enable_testing()

# Optional: Add tests directory if it exists
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests")
    add_subdirectory(tests)
endif()

# Add tools directory for o2l-pkg
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tools")
    add_subdirectory(tools)
endif()

# Install target
install(TARGETS o2l DESTINATION .)

# Print build information
message(STATUS "Building O²L Programming Language Interpreter")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")