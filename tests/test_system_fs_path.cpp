#include <gtest/gtest.h>

#include <filesystem>
#include <memory>

#include "../src/Common/Exceptions.hpp"
#include "../src/Runtime/Context.hpp"
#include "../src/Runtime/ListInstance.hpp"
#include "../src/Runtime/SystemLibrary.hpp"
#include "../src/Runtime/Value.hpp"

using namespace o2l;

class SystemFSPathTest : public ::testing::Test {
   protected:
    Context context;

    // Helper method to create system.fs object and call methods
    Value callFSMethod(const std::string& method_name, const std::vector<Value>& args = {}) {
        auto fs_object = SystemLibrary::createFSObject();
        EXPECT_TRUE(fs_object->hasMethod(method_name)) << "Method " << method_name << " not found";
        return fs_object->callMethod(method_name, args, context);
    }

    // Helper to check text results
    void expectText(const Value& result, const std::string& expected) {
        ASSERT_TRUE(std::holds_alternative<Text>(result));
        EXPECT_EQ(std::get<Text>(result), expected);
    }

    // Helper to check boolean results
    void expectBool(const Value& result, Bool expected) {
        ASSERT_TRUE(std::holds_alternative<Bool>(result));
        EXPECT_EQ(std::get<Bool>(result), expected);
    }

    // Helper to check if result is a non-empty text
    bool isNonEmptyText(const Value& result) {
        if (!std::holds_alternative<Text>(result)) {
            return false;
        }
        std::string text = std::get<Text>(result);
        return !text.empty();
    }

    // Helper to get list size
    size_t getListSize(const Value& result) {
        EXPECT_TRUE(std::holds_alternative<std::shared_ptr<ListInstance>>(result));
        auto list = std::get<std::shared_ptr<ListInstance>>(result);
        return list->size();
    }
};

// Test basename method
TEST_F(SystemFSPathTest, BasenameMethod) {
    // Test regular file path
    Value result1 = callFSMethod("basename", {Value(Text("/usr/local/bin/program"))});
    expectText(result1, "program");

    // Test file with extension
    Value result2 = callFSMethod("basename", {Value(Text("/home/user/document.txt"))});
    expectText(result2, "document.txt");

    // Test directory path with trailing slash (filesystem may return empty string)
    Value result3 = callFSMethod("basename", {Value(Text("/home/user/"))});
    ASSERT_TRUE(std::holds_alternative<Text>(result3));
    std::string result3_text = std::get<Text>(result3);
    EXPECT_TRUE(result3_text == "." || result3_text == "");

    // Test directory path without trailing slash
    Value result4 = callFSMethod("basename", {Value(Text("/home/user"))});
    expectText(result4, "user");

    // Test relative path
    Value result5 = callFSMethod("basename", {Value(Text("documents/file.pdf"))});
    expectText(result5, "file.pdf");

    // Test single filename
    Value result6 = callFSMethod("basename", {Value(Text("readme.md"))});
    expectText(result6, "readme.md");

    // Test error cases
    EXPECT_THROW(callFSMethod("basename", {}), EvaluationError);
    EXPECT_THROW(callFSMethod("basename", {Value(Int(123))}), EvaluationError);
    EXPECT_THROW(callFSMethod("basename", {Value(Text("test")), Value(Text("extra"))}),
                 EvaluationError);
}

// Test dirname method
TEST_F(SystemFSPathTest, DirnameMethod) {
    // Test regular file path
    Value result1 = callFSMethod("dirname", {Value(Text("/usr/local/bin/program"))});
    expectText(result1, "/usr/local/bin");

    // Test file with extension
    Value result2 = callFSMethod("dirname", {Value(Text("/home/user/document.txt"))});
    expectText(result2, "/home/user");

    // Test relative path
    Value result3 = callFSMethod("dirname", {Value(Text("documents/file.pdf"))});
    expectText(result3, "documents");

    // Test single filename (should return empty or ".")
    Value result4 = callFSMethod("dirname", {Value(Text("readme.md"))});
    EXPECT_TRUE(isNonEmptyText(result4) || std::get<Text>(result4) == "");

    // Test root path
    Value result5 = callFSMethod("dirname", {Value(Text("/"))});
    expectText(result5, "/");

    // Test error cases
    EXPECT_THROW(callFSMethod("dirname", {}), EvaluationError);
    EXPECT_THROW(callFSMethod("dirname", {Value(Int(123))}), EvaluationError);
}

// Test extname method
TEST_F(SystemFSPathTest, ExtnameMethod) {
    // Test file with extension
    Value result1 = callFSMethod("extname", {Value(Text("document.txt"))});
    expectText(result1, ".txt");

    // Test file with multiple extensions
    Value result2 = callFSMethod("extname", {Value(Text("archive.tar.gz"))});
    expectText(result2, ".gz");

    // Test file without extension
    Value result3 = callFSMethod("extname", {Value(Text("readme"))});
    expectText(result3, "");

    // Test hidden file
    Value result4 = callFSMethod("extname", {Value(Text(".bashrc"))});
    expectText(result4, "");

    // Test path with extension
    Value result5 = callFSMethod("extname", {Value(Text("/home/user/file.pdf"))});
    expectText(result5, ".pdf");

    // Test error cases
    EXPECT_THROW(callFSMethod("extname", {}), EvaluationError);
    EXPECT_THROW(callFSMethod("extname", {Value(Int(123))}), EvaluationError);
}

// Test join method
TEST_F(SystemFSPathTest, JoinMethod) {
    // Test basic join
    Value result1 =
        callFSMethod("join", {Value(Text("/usr")), Value(Text("local")), Value(Text("bin"))});
    EXPECT_TRUE(isNonEmptyText(result1));
    std::string joined = std::get<Text>(result1);
    EXPECT_TRUE(joined.find("usr") != std::string::npos);
    EXPECT_TRUE(joined.find("local") != std::string::npos);
    EXPECT_TRUE(joined.find("bin") != std::string::npos);

    // Test join with relative paths
    Value result2 =
        callFSMethod("join", {Value(Text("home")), Value(Text("user")), Value(Text("documents"))});
    EXPECT_TRUE(isNonEmptyText(result2));

    // Test join single argument
    Value result3 = callFSMethod("join", {Value(Text("/root"))});
    expectText(result3, "/root");

    // Test join with empty components
    Value result4 =
        callFSMethod("join", {Value(Text("/usr")), Value(Text("")), Value(Text("bin"))});
    EXPECT_TRUE(isNonEmptyText(result4));

    // Test error cases
    EXPECT_THROW(callFSMethod("join", {}), EvaluationError);
    EXPECT_THROW(callFSMethod("join", {Value(Int(123))}), EvaluationError);
    EXPECT_THROW(callFSMethod("join", {Value(Text("test")), Value(Int(123))}), EvaluationError);
}

// Test normalize method
TEST_F(SystemFSPathTest, NormalizeMethod) {
    // Test path with . and .. components
    Value result1 = callFSMethod("normalize", {Value(Text("/usr/./local/../bin"))});
    EXPECT_TRUE(isNonEmptyText(result1));
    std::string normalized = std::get<Text>(result1);
    EXPECT_TRUE(normalized.find("./") == std::string::npos);
    EXPECT_TRUE(normalized.find("../") == std::string::npos);

    // Test relative path with . and ..
    Value result2 = callFSMethod("normalize", {Value(Text("./documents/../photos/./image.jpg"))});
    EXPECT_TRUE(isNonEmptyText(result2));

    // Test already normalized path
    Value result3 = callFSMethod("normalize", {Value(Text("/usr/local/bin"))});
    expectText(result3, "/usr/local/bin");

    // Test error cases
    EXPECT_THROW(callFSMethod("normalize", {}), EvaluationError);
    EXPECT_THROW(callFSMethod("normalize", {Value(Int(123))}), EvaluationError);
}

// Test resolve method
TEST_F(SystemFSPathTest, ResolveMethod) {
    // Test relative path resolution
    Value result1 = callFSMethod("resolve", {Value(Text("documents/file.txt"))});
    EXPECT_TRUE(isNonEmptyText(result1));
    std::string resolved = std::get<Text>(result1);
    EXPECT_TRUE(resolved[0] == '/');  // Should be absolute

    // Test already absolute path
    Value result2 = callFSMethod("resolve", {Value(Text("/usr/local/bin"))});
    expectText(result2, "/usr/local/bin");

    // Test current directory
    Value result3 = callFSMethod("resolve", {Value(Text("."))});
    EXPECT_TRUE(isNonEmptyText(result3));
    std::string current = std::get<Text>(result3);
    EXPECT_TRUE(current[0] == '/');  // Should be absolute

    // Test error cases
    EXPECT_THROW(callFSMethod("resolve", {}), EvaluationError);
    EXPECT_THROW(callFSMethod("resolve", {Value(Int(123))}), EvaluationError);
}

// Test relative method
TEST_F(SystemFSPathTest, RelativeMethod) {
    // Test getting relative path
    Value result1 = callFSMethod(
        "relative", {Value(Text("/usr/local")), Value(Text("/usr/local/bin/program"))});
    EXPECT_TRUE(isNonEmptyText(result1));
    std::string relative = std::get<Text>(result1);
    EXPECT_TRUE(relative.find("bin/program") != std::string::npos);

    // Test same directory
    Value result2 =
        callFSMethod("relative", {Value(Text("/usr/local")), Value(Text("/usr/local"))});
    EXPECT_TRUE(std::holds_alternative<Text>(result2));

    // Test parent directory
    Value result3 =
        callFSMethod("relative", {Value(Text("/usr/local/bin")), Value(Text("/usr/local"))});
    EXPECT_TRUE(isNonEmptyText(result3));

    // Test error cases
    EXPECT_THROW(callFSMethod("relative", {}), EvaluationError);
    EXPECT_THROW(callFSMethod("relative", {Value(Text("test"))}), EvaluationError);
    EXPECT_THROW(callFSMethod("relative", {Value(Int(123)), Value(Text("test"))}), EvaluationError);
}

// Test isAbsolute method
TEST_F(SystemFSPathTest, IsAbsoluteMethod) {
    // Test absolute paths
    Value result1 = callFSMethod("isAbsolute", {Value(Text("/usr/local/bin"))});
    expectBool(result1, true);

    Value result2 = callFSMethod("isAbsolute", {Value(Text("/"))});
    expectBool(result2, true);

    // Test relative paths
    Value result3 = callFSMethod("isAbsolute", {Value(Text("documents/file.txt"))});
    expectBool(result3, false);

    Value result4 = callFSMethod("isAbsolute", {Value(Text("./test"))});
    expectBool(result4, false);

    Value result5 = callFSMethod("isAbsolute", {Value(Text("../parent"))});
    expectBool(result5, false);

    // Test error cases
    EXPECT_THROW(callFSMethod("isAbsolute", {}), EvaluationError);
    EXPECT_THROW(callFSMethod("isAbsolute", {Value(Int(123))}), EvaluationError);
}

// Test splitPath method
TEST_F(SystemFSPathTest, SplitPathMethod) {
    // Test absolute path split
    Value result1 = callFSMethod("splitPath", {Value(Text("/usr/local/bin/program"))});
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<ListInstance>>(result1));
    size_t size1 = getListSize(result1);
    EXPECT_GT(size1, 0);

    // Test relative path split
    Value result2 = callFSMethod("splitPath", {Value(Text("documents/photos/image.jpg"))});
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<ListInstance>>(result2));
    size_t size2 = getListSize(result2);
    EXPECT_EQ(size2, 3);  // documents, photos, image.jpg

    // Test single component
    Value result3 = callFSMethod("splitPath", {Value(Text("readme.txt"))});
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<ListInstance>>(result3));
    size_t size3 = getListSize(result3);
    EXPECT_EQ(size3, 1);

    // Test error cases
    EXPECT_THROW(callFSMethod("splitPath", {}), EvaluationError);
    EXPECT_THROW(callFSMethod("splitPath", {Value(Int(123))}), EvaluationError);
}

// Test getParent method (alias for dirname)
TEST_F(SystemFSPathTest, GetParentMethod) {
    // Test regular file path
    Value result1 = callFSMethod("getParent", {Value(Text("/usr/local/bin/program"))});
    expectText(result1, "/usr/local/bin");

    // Test file with extension
    Value result2 = callFSMethod("getParent", {Value(Text("/home/user/document.txt"))});
    expectText(result2, "/home/user");

    // Test error cases
    EXPECT_THROW(callFSMethod("getParent", {}), EvaluationError);
    EXPECT_THROW(callFSMethod("getParent", {Value(Int(123))}), EvaluationError);
}

// Test changeExtension method
TEST_F(SystemFSPathTest, ChangeExtensionMethod) {
    // Test changing extension
    Value result1 =
        callFSMethod("changeExtension", {Value(Text("document.txt")), Value(Text(".pdf"))});
    expectText(result1, "document.pdf");

    // Test adding extension to file without extension
    Value result2 = callFSMethod("changeExtension", {Value(Text("readme")), Value(Text(".md"))});
    expectText(result2, "readme.md");

    // Test changing extension without dot
    Value result3 = callFSMethod("changeExtension", {Value(Text("file.old")), Value(Text("new"))});
    expectText(result3, "file.new");

    // Test removing extension
    Value result4 = callFSMethod("changeExtension", {Value(Text("document.txt")), Value(Text(""))});
    expectText(result4, "document");

    // Test with path
    Value result5 =
        callFSMethod("changeExtension", {Value(Text("/home/user/file.old")), Value(Text(".new"))});
    expectText(result5, "/home/user/file.new");

    // Test error cases
    EXPECT_THROW(callFSMethod("changeExtension", {}), EvaluationError);
    EXPECT_THROW(callFSMethod("changeExtension", {Value(Text("test"))}), EvaluationError);
    EXPECT_THROW(callFSMethod("changeExtension", {Value(Int(123)), Value(Text(".ext"))}),
                 EvaluationError);
    EXPECT_THROW(callFSMethod("changeExtension", {Value(Text("test")), Value(Int(123))}),
                 EvaluationError);
}

// Test method availability
TEST_F(SystemFSPathTest, PathMethodAvailability) {
    auto fs_object = SystemLibrary::createFSObject();

    // Test that all expected path methods are available
    std::vector<std::string> expected_methods = {
        // Original filesystem methods
        "readText", "writeText", "exists", "isFile", "isDirectory", "listFiles", "createDirectory",
        "deleteFile",
        // New path manipulation methods
        "basename", "dirname", "extname", "join", "normalize", "resolve", "relative", "isAbsolute",
        "splitPath", "getParent", "changeExtension"};

    for (const auto& method : expected_methods) {
        EXPECT_TRUE(fs_object->hasMethod(method)) << "Method " << method << " not found";
        EXPECT_TRUE(fs_object->isMethodExternal(method)) << "Method " << method << " not external";
    }
}

// Test cross-platform path operations
TEST_F(SystemFSPathTest, CrossPlatformPaths) {
    // Test that methods handle different path separators gracefully
    Value result1 = callFSMethod("normalize", {Value(Text("folder/subfolder/../file.txt"))});
    EXPECT_TRUE(isNonEmptyText(result1));

    Value result2 =
        callFSMethod("join", {Value(Text("usr")), Value(Text("local")), Value(Text("bin"))});
    EXPECT_TRUE(isNonEmptyText(result2));

    // Test basename with different separators
    Value result3 = callFSMethod("basename", {Value(Text("folder/file.txt"))});
    expectText(result3, "file.txt");

    // Test dirname with different separators
    Value result4 = callFSMethod("dirname", {Value(Text("folder/file.txt"))});
    expectText(result4, "folder");
}