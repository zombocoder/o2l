#include <gtest/gtest.h>
#include <memory>
#include <vector>
#include "Lexer.hpp"
#include "Parser.hpp"
#include "AST/Node.hpp"
#include "AST/VariableDeclarationNode.hpp"
#include "AST/PropertyDeclarationNode.hpp"
#include "AST/RecordDeclarationNode.hpp"
#include "AST/ProtocolDeclarationNode.hpp"

using namespace o2l;

class NestedGenericsTest : public ::testing::Test {
protected:
    std::unique_ptr<Parser> createParser(const std::string& source) {
        Lexer lexer(source);
        auto tokens = lexer.tokenizeAll();
        return std::make_unique<Parser>(std::move(tokens));
    }
};

TEST_F(NestedGenericsTest, NestedListAndMap) {
    std::string source = "complex: Map<Text, List<Int>> = {}";
    auto parser = createParser(source);
    auto node = parser->parseVariableDeclaration();
    ASSERT_NE(node, nullptr);
    
    auto var_decl = dynamic_cast<VariableDeclarationNode*>(node.get());
    ASSERT_NE(var_decl, nullptr);
    EXPECT_EQ(var_decl->getTypeName(), "Map<Text, List<Int>>");
}

TEST_F(NestedGenericsTest, TripleNested) {
    std::string source = "nested: List<Map<Text, Set<Int>>> = []";
    auto parser = createParser(source);
    auto node = parser->parseVariableDeclaration();
    ASSERT_NE(node, nullptr);
    
    auto var_decl = dynamic_cast<VariableDeclarationNode*>(node.get());
    ASSERT_NE(var_decl, nullptr);
    EXPECT_EQ(var_decl->getTypeName(), "List<Map<Text, Set<Int>>>");
}

TEST_F(NestedGenericsTest, PropertyNested) {
    std::string source = "property cache: Map<Int, List<Text>>";
    auto parser = createParser(source);
    auto node = parser->parsePropertyDeclaration();
    ASSERT_NE(node, nullptr);
    
    auto prop_decl = dynamic_cast<PropertyDeclarationNode*>(node.get());
    ASSERT_NE(prop_decl, nullptr);
    EXPECT_EQ(prop_decl->getTypeName(), "Map<Int, List<Text>>");
}

TEST_F(NestedGenericsTest, RecordNested) {
    std::string source = "Record Data { items: List<Set<Text>>, count: Int }";
    auto parser = createParser(source);
    auto node = parser->parseRecordDeclaration();
    ASSERT_NE(node, nullptr);
    
    auto record_decl = dynamic_cast<RecordDeclarationNode*>(node.get());
    ASSERT_NE(record_decl, nullptr);
    
    const auto& fields = record_decl->getFields();
    ASSERT_EQ(fields.size(), 2);
    EXPECT_EQ(fields[0].type, "List<Set<Text>>");
}

TEST_F(NestedGenericsTest, ProtocolNested) {
    std::string source = "Protocol Processor { method process(data: List<Map<Int, Text>>): Result<Bool, Error> }";
    auto parser = createParser(source);
    auto node = parser->parseProtocolDeclaration();
    ASSERT_NE(node, nullptr);
    
    auto proto_decl = dynamic_cast<ProtocolDeclarationNode*>(node.get());
    ASSERT_NE(proto_decl, nullptr);
    
    const auto& methods = proto_decl->getMethodSignatures();
    ASSERT_EQ(methods.size(), 1);
    EXPECT_EQ(methods[0].parameters[0].type, "List<Map<Int, Text>>");
    EXPECT_EQ(methods[0].return_type, "Result<Bool, Error>");
}

TEST_F(NestedGenericsTest, SpacesAndNewlinesInGenerics) {
    std::string source = "x: Map<\n  Text,\n  List<Int>\n> = {}";
    auto parser = createParser(source);
    auto node = parser->parseVariableDeclaration();
    ASSERT_NE(node, nullptr);
    
    auto var_decl = dynamic_cast<VariableDeclarationNode*>(node.get());
    ASSERT_NE(var_decl, nullptr);
    EXPECT_EQ(var_decl->getTypeName(), "Map<Text, List<Int>>");
}
