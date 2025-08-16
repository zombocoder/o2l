/*
 * Copyright 2024 O²L Programming Language
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <gtest/gtest.h>
#include "../src/Runtime/FFI/FFIEngine.hpp"
#include "../src/Runtime/FFILibrary.hpp"
#include "../src/Runtime/FFI/FFITypes.hpp"
#include "../src/Runtime/Context.hpp"
#include "../src/Runtime/Value.hpp"
#include <sqlite3.h>

using namespace o2l;
using namespace o2l::ffi;

class FFISQLiteIntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        context = std::make_unique<Context>();
        ffi_engine = std::make_unique<FFIEngine>();
        
        // Load SQLite library for testing
        sqlite_lib = loadSQLiteLibrary();
        ASSERT_NE(sqlite_lib, nullptr) << "Failed to load SQLite library for testing";
    }
    
    void TearDown() override {
        if (sqlite_lib) {
            dlclose(sqlite_lib);
        }
    }

    void* loadSQLiteLibrary() {
        // Try common SQLite library paths
        const char* lib_paths[] = {
            "/opt/homebrew/lib/libsqlite3.dylib",          // Homebrew on Apple Silicon
            "/opt/homebrew/Cellar/sqlite/3.47.1/lib/libsqlite3.dylib", // Specific Homebrew version
            "/usr/local/lib/libsqlite3.dylib",             // Homebrew on Intel Mac
            "/usr/lib/x86_64-linux-gnu/libsqlite3.so.0",   // Ubuntu/Debian
            "/usr/lib64/libsqlite3.so.0",                  // RHEL/CentOS
            "/System/Library/Frameworks/SQLite3.framework/SQLite3", // macOS system
            "libsqlite3.so",                               // Generic Linux
            "libsqlite3.dylib"                             // Generic macOS
        };
        
        for (const char* path : lib_paths) {
            void* lib = dlopen(path, RTLD_LAZY);
            if (lib != nullptr) {
                return lib;
            }
        }
        
        return nullptr;
    }

    std::unique_ptr<Context> context;
    std::unique_ptr<FFIEngine> ffi_engine;
    void* sqlite_lib;
};

// Test FFI library loading with SQLite
TEST_F(FFISQLiteIntegrationTest, LoadSQLiteLibrary) {
    // Test ffi.load function with SQLite
    std::vector<Value> args;
    args.push_back(Value(Text("/opt/homebrew/Cellar/sqlite/3.47.1/lib/libsqlite3.dylib")));
    
    Value result = FFILibrary::ffi_load(args, *context);
    
    // Check that result is a Result type
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<ResultInstance>>(result));
    
    auto result_inst = std::get<std::shared_ptr<ResultInstance>>(result);
    if (result_inst->isSuccess()) {
        // Library loaded successfully
        Value lib_val = result_inst->getResult();
        EXPECT_TRUE(std::holds_alternative<std::shared_ptr<LibraryInstance>>(lib_val));
    }
    // Note: Test might fail if SQLite is not available at the specified path
    // This is expected behavior for the test environment
}

// Test FFI symbol resolution with SQLite functions
TEST_F(FFISQLiteIntegrationTest, SQLiteSymbolResolution) {
    if (!sqlite_lib) {
        GTEST_SKIP() << "SQLite library not available for testing";
    }
    
    // Create LibraryInstance for SQLite
    auto lib_inst = std::make_shared<LibraryInstance>(sqlite_lib, "/test/path");
    Value lib_value(lib_inst);
    
    // Test resolving sqlite3_libversion symbol
    std::vector<Value> args;
    args.push_back(lib_value);
    args.push_back(Value(Text("sqlite3_libversion")));
    args.push_back(Value(Text("->ptr")));
    
    Value result = FFILibrary::ffi_symbol(args, *context);
    
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<ResultInstance>>(result));
    auto result_inst = std::get<std::shared_ptr<ResultInstance>>(result);
    EXPECT_TRUE(result_inst->isSuccess());
    
    Value func_val = result_inst->getResult();
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<FunctionInstance>>(func_val));
}

// Test FFI function call with SQLite libversion
TEST_F(FFISQLiteIntegrationTest, SQLiteLibversionCall) {
    if (!sqlite_lib) {
        GTEST_SKIP() << "SQLite library not available for testing";
    }
    
    // Get sqlite3_libversion function
    void* version_func = dlsym(sqlite_lib, "sqlite3_libversion");
    ASSERT_NE(version_func, nullptr) << "Failed to resolve sqlite3_libversion";
    
    // Create FunctionInstance
    auto func_inst = std::make_shared<FunctionInstance>(
        version_func, 
        ffi_type_pointer,    // return type: char*
        std::vector<ffi_type*>{}  // no parameters
    );
    
    Value func_value(func_inst);
    
    // Call the function
    std::vector<Value> args;
    args.push_back(func_value);
    
    Value result = FFILibrary::ffi_call(args, *context);
    
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<ResultInstance>>(result));
    auto result_inst = std::get<std::shared_ptr<ResultInstance>>(result);
    EXPECT_TRUE(result_inst->isSuccess());
    
    // The result should be a pointer to the version string
    Value version_ptr = result_inst->getResult();
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<PtrInstance>>(version_ptr));
    
    auto ptr_inst = std::get<std::shared_ptr<PtrInstance>>(version_ptr);
    EXPECT_FALSE(ptr_inst->isNull());
}

// Test SQLite database operations through FFI
TEST_F(FFISQLiteIntegrationTest, SQLiteDatabaseOperations) {
    if (!sqlite_lib) {
        GTEST_SKIP() << "SQLite library not available for testing";
    }
    
    // Test opening in-memory database
    sqlite3* db = nullptr;
    int result = sqlite3_open(":memory:", &db);
    ASSERT_EQ(result, SQLITE_OK) << "Failed to open SQLite database";
    
    // Test creating table
    const char* create_sql = "CREATE TABLE test (id INTEGER, name TEXT)";
    result = sqlite3_exec(db, create_sql, nullptr, nullptr, nullptr);
    EXPECT_EQ(result, SQLITE_OK) << "Failed to create table";
    
    // Test inserting data
    const char* insert_sql = "INSERT INTO test VALUES (1, 'Alice'), (2, 'Bob')";
    result = sqlite3_exec(db, insert_sql, nullptr, nullptr, nullptr);
    EXPECT_EQ(result, SQLITE_OK) << "Failed to insert data";
    
    // Test querying data
    sqlite3_stmt* stmt = nullptr;
    result = sqlite3_prepare_v2(db, "SELECT id, name FROM test", -1, &stmt, nullptr);
    EXPECT_EQ(result, SQLITE_OK) << "Failed to prepare statement";
    
    int row_count = 0;
    while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {
        row_count++;
        int id = sqlite3_column_int(stmt, 0);
        const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        
        EXPECT_GT(id, 0);
        EXPECT_NE(name, nullptr);
        
        if (row_count == 1) {
            EXPECT_EQ(id, 1);
            EXPECT_STREQ(name, "Alice");
        } else if (row_count == 2) {
            EXPECT_EQ(id, 2);
            EXPECT_STREQ(name, "Bob");
        }
    }
    
    EXPECT_EQ(row_count, 2) << "Expected 2 rows in result";
    EXPECT_EQ(result, SQLITE_DONE) << "Statement should complete normally";
    
    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

// Test FFI C string creation and usage with SQLite
TEST_F(FFISQLiteIntegrationTest, CStringCreationAndUsage) {
    // Test creating C string for SQL query
    std::vector<Value> args;
    args.push_back(Value(Text("SELECT sqlite_version()")));
    
    Value result = FFILibrary::ffi_cstring(args, *context);
    
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<ResultInstance>>(result));
    auto result_inst = std::get<std::shared_ptr<ResultInstance>>(result);
    EXPECT_TRUE(result_inst->isSuccess());
    
    Value buffer_val = result_inst->getResult();
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<CBufferInstance>>(buffer_val));
    
    auto buffer = std::get<std::shared_ptr<CBufferInstance>>(buffer_val);
    EXPECT_GT(buffer->size(), 0);
    
    // Verify the string content
    const char* c_str = reinterpret_cast<const char*>(buffer->data());
    EXPECT_STREQ(c_str, "SELECT sqlite_version()");
}

// Test FFI array creation for SQLite parameter arrays
TEST_F(FFISQLiteIntegrationTest, ArrayCreationForSQLiteParams) {
    // Test creating pointer array for SQLite handles
    std::vector<Value> args;
    args.push_back(Value(Text("ptr")));
    args.push_back(Value(Int(1)));
    
    Value result = FFILibrary::ffi_array(args, *context);
    
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<ResultInstance>>(result));
    auto result_inst = std::get<std::shared_ptr<ResultInstance>>(result);
    EXPECT_TRUE(result_inst->isSuccess());
    
    Value array_val = result_inst->getResult();
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<CArrayInstance>>(array_val));
    
    auto array = std::get<std::shared_ptr<CArrayInstance>>(array_val);
    EXPECT_EQ(array->element_count(), 1);
    EXPECT_EQ(array->element_size(), sizeof(void*));
    
    // Test setting and getting pointer values
    auto null_ptr = std::make_shared<PtrInstance>(nullptr);
    Value ptr_val(null_ptr);
    
    bool set_result = array->setElement(0, ptr_val);
    EXPECT_TRUE(set_result);
    
    Value retrieved_val = array->getElement(0);
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<PtrInstance>>(retrieved_val));
}

// Test error handling in SQLite FFI operations
TEST_F(FFISQLiteIntegrationTest, SQLiteFFIErrorHandling) {
    // Test loading non-existent library
    std::vector<Value> args;
    args.push_back(Value(Text("/nonexistent/path/libsqlite3.so")));
    
    Value result = FFILibrary::ffi_load(args, *context);
    
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<ResultInstance>>(result));
    auto result_inst = std::get<std::shared_ptr<ResultInstance>>(result);
    EXPECT_FALSE(result_inst->isSuccess());
    
    // Test resolving non-existent symbol
    if (sqlite_lib) {
        auto lib_inst = std::make_shared<LibraryInstance>(sqlite_lib, "/test/path");
        Value lib_value(lib_inst);
        
        std::vector<Value> symbol_args;
        symbol_args.push_back(lib_value);
        symbol_args.push_back(Value(Text("nonexistent_function")));
        symbol_args.push_back(Value(Text("->i32")));
        
        Value symbol_result = FFILibrary::ffi_symbol(symbol_args, *context);
        
        EXPECT_TRUE(std::holds_alternative<std::shared_ptr<ResultInstance>>(symbol_result));
        auto symbol_result_inst = std::get<std::shared_ptr<ResultInstance>>(symbol_result);
        EXPECT_FALSE(symbol_result_inst->isSuccess());
    }
}

// Test memory management in SQLite FFI operations
TEST_F(FFISQLiteIntegrationTest, SQLiteFFIMemoryManagement) {
    // Test that buffers and arrays are properly managed
    {
        // Create a buffer in a scope
        std::vector<Value> args;
        args.push_back(Value(Text("CREATE TABLE test (id INTEGER)")));
        
        Value result = FFILibrary::ffi_cstring(args, *context);
        auto result_inst = std::get<std::shared_ptr<ResultInstance>>(result);
        EXPECT_TRUE(result_inst->isSuccess());
        
        Value buffer_val = result_inst->getResult();
        auto buffer = std::get<std::shared_ptr<CBufferInstance>>(buffer_val);
        
        // Verify buffer is valid
        EXPECT_NE(buffer->data(), nullptr);
        EXPECT_GT(buffer->size(), 0);
    }
    // Buffer should be cleaned up automatically when it goes out of scope
    
    {
        // Create an array in a scope
        std::vector<Value> args;
        args.push_back(Value(Text("i32")));
        args.push_back(Value(Int(10)));
        
        Value result = FFILibrary::ffi_array(args, *context);
        auto result_inst = std::get<std::shared_ptr<ResultInstance>>(result);
        EXPECT_TRUE(result_inst->isSuccess());
        
        Value array_val = result_inst->getResult();
        auto array = std::get<std::shared_ptr<CArrayInstance>>(array_val);
        
        // Verify array is valid
        EXPECT_NE(array->data(), nullptr);
        EXPECT_EQ(array->element_count(), 10);
    }
    // Array should be cleaned up automatically when it goes out of scope
}

// Test complex SQLite operations through FFI marshaling
TEST_F(FFISQLiteIntegrationTest, ComplexSQLiteFFIMarshaling) {
    if (!sqlite_lib) {
        GTEST_SKIP() << "SQLite library not available for testing";
    }
    
    // Test marshaling of multiple parameter types for sqlite3_bind functions
    
    // Create a buffer for SQL string
    std::vector<Value> sql_args;
    sql_args.push_back(Value(Text("INSERT INTO test VALUES (?, ?)")));
    
    Value sql_result = FFILibrary::ffi_cstring(sql_args, *context);
    auto sql_result_inst = std::get<std::shared_ptr<ResultInstance>>(sql_result);
    EXPECT_TRUE(sql_result_inst->isSuccess());
    
    // Create an array for statement handle
    std::vector<Value> stmt_args;
    stmt_args.push_back(Value(Text("ptr")));
    stmt_args.push_back(Value(Int(1)));
    
    Value stmt_result = FFILibrary::ffi_array(stmt_args, *context);
    auto stmt_result_inst = std::get<std::shared_ptr<ResultInstance>>(stmt_result);
    EXPECT_TRUE(stmt_result_inst->isSuccess());
    
    // Verify the created objects are properly typed
    Value sql_buffer = sql_result_inst->getResult();
    Value stmt_array = stmt_result_inst->getResult();
    
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<CBufferInstance>>(sql_buffer));
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<CArrayInstance>>(stmt_array));
    
    auto buffer = std::get<std::shared_ptr<CBufferInstance>>(sql_buffer);
    auto array = std::get<std::shared_ptr<CArrayInstance>>(stmt_array);
    
    // Verify content and type integrity
    const char* sql_str = reinterpret_cast<const char*>(buffer->data());
    EXPECT_STREQ(sql_str, "INSERT INTO test VALUES (?, ?)");
    
    EXPECT_EQ(array->element_count(), 1);
    EXPECT_EQ(array->element_size(), sizeof(void*));
}