# Tests CMakeLists.txt for O²L Programming Language
cmake_minimum_required(VERSION 3.20)

# Find required packages
find_package(GTest REQUIRED)

# Include parent directories for accessing source files
include_directories(../src)

# Collect all source files (excluding main.cpp)
file(GLOB_RECURSE TEST_SOURCES
    "../src/Lexer.cpp"
    "../src/Parser.cpp" 
    "../src/Interpreter.cpp"
    "../src/AST/*.cpp"
    "../src/Runtime/*.cpp"
    "../src/Common/*.cpp"
)

# Test executables
set(TEST_SOURCES_LIST
    test_lexer.cpp
    test_parser.cpp
    test_runtime.cpp
    test_protocols.cpp
    test_integration.cpp
    test_text_methods.cpp
    test_math_library.cpp
    test_testing_library.cpp
    test_datetime_library.cpp
    test_system_os_extended.cpp
    test_system_fs_path.cpp
    test_regexp_library.cpp
    test_else_if_length.cpp
    test_url_library.cpp
    test_json_library.cpp
    test_http_client_library.cpp
    test_http_server_library.cpp
    test_type_conversions.cpp
    test_protocol_signature_validation.cpp
    test_o2l_fmt.cpp
    test_ffi_simplified.cpp
    test_ffi_library.cpp
    test_main.cpp
)

# Create test executable
add_executable(o2l_tests ${TEST_SOURCES_LIST} ${TEST_SOURCES} ../tools/o2l-fmt/formatter.cpp)

# Find and link required dependencies
find_package(PkgConfig)
if(PkgConfig_FOUND)
    pkg_check_modules(FFI libffi)
endif()

# Link against Google Test
target_link_libraries(o2l_tests GTest::gtest GTest::gtest_main)

# Link FFI and system libraries for FFI tests
if(FFI_FOUND)
    target_link_libraries(o2l_tests ${FFI_LIBRARIES})
    target_include_directories(o2l_tests PRIVATE ${FFI_INCLUDE_DIRS})
endif()

# Link dynamic loading libraries
if(APPLE)
    target_link_libraries(o2l_tests ${CMAKE_DL_LIBS})
elseif(UNIX)
    target_link_libraries(o2l_tests ${CMAKE_DL_LIBS})
endif()

# Link SQLite for SQLite integration tests
find_library(SQLITE3_LIBRARY sqlite3)
if(SQLITE3_LIBRARY)
    target_link_libraries(o2l_tests ${SQLITE3_LIBRARY})
endif()

# Set C++23 standard for tests
target_compile_features(o2l_tests PRIVATE cxx_std_23)

# Include directories
target_include_directories(o2l_tests PRIVATE ../src ../tools/o2l-fmt)

# Discover and add tests
include(GoogleTest)
gtest_discover_tests(o2l_tests)

# Add individual test targets for easier running
add_test(NAME lexer_tests COMMAND o2l_tests --gtest_filter="LexerTest.*")
add_test(NAME parser_tests COMMAND o2l_tests --gtest_filter="ParserTest.*")
add_test(NAME runtime_tests COMMAND o2l_tests --gtest_filter="RuntimeTest.*")
add_test(NAME protocol_tests COMMAND o2l_tests --gtest_filter="ProtocolTest.*")
add_test(NAME integration_tests COMMAND o2l_tests --gtest_filter="IntegrationTest.*")
add_test(NAME text_method_tests COMMAND o2l_tests --gtest_filter="TextMethodTest.*")
add_test(NAME math_library_tests COMMAND o2l_tests --gtest_filter="MathLibraryTest.*")
add_test(NAME testing_library_tests COMMAND o2l_tests --gtest_filter="TestLibraryTest.*")
add_test(NAME datetime_library_tests COMMAND o2l_tests --gtest_filter="DateTimeLibraryTest.*")
add_test(NAME system_os_extended_tests COMMAND o2l_tests --gtest_filter="SystemOSExtendedTest.*")
add_test(NAME system_fs_path_tests COMMAND o2l_tests --gtest_filter="SystemFSPathTest.*")
add_test(NAME regexp_library_tests COMMAND o2l_tests --gtest_filter="RegexpLibraryTest.*")
add_test(NAME else_if_length_tests COMMAND o2l_tests --gtest_filter="ElseIfAndLengthTest.*")
add_test(NAME url_library_tests COMMAND o2l_tests --gtest_filter="UrlLibraryTest.*")
add_test(NAME json_library_tests COMMAND o2l_tests --gtest_filter="JsonLibraryTest.*")
add_test(NAME http_client_library_tests COMMAND o2l_tests --gtest_filter="HttpClientLibraryTest.*")
add_test(NAME http_server_library_tests COMMAND o2l_tests --gtest_filter="HttpServerLibraryTest.*")
add_test(NAME type_conversion_tests COMMAND o2l_tests --gtest_filter="TypeConversionTest.*")
add_test(NAME protocol_signature_validation_tests COMMAND o2l_tests --gtest_filter="ProtocolSignatureValidationTest.*")
add_test(NAME o2l_fmt_tests COMMAND o2l_tests --gtest_filter="O2LFmtTest.*")
add_test(NAME ffi_simplified_tests COMMAND o2l_tests --gtest_filter="FFISimplifiedTest.*")
add_test(NAME ffi_library_tests COMMAND o2l_tests --gtest_filter="FFILibraryTest.*")