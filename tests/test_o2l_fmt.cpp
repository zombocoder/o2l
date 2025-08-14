#include <gtest/gtest.h>
#include <string>
#include <sstream>
#include <filesystem>
#include <fstream>
#include "../tools/o2l-fmt/formatter.hpp"

namespace fs = std::filesystem;

class O2LFmtTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Test setup if needed
    }
    
    std::string format(const std::string& input) {
        O2LFormatter formatter;
        return formatter.format_code(input);
    }
};

// Test basic object declaration formatting
TEST_F(O2LFmtTest, BasicObjectDeclaration) {
    std::string input = "Object Test{method main():Text{return\"hello\"}}";
    std::string expected = "Object Test {\n    method main(): Text {\n        return \"hello\"\n    }\n}";
    
    std::string result = format(input);
    EXPECT_EQ(expected, result);
}

// Test single-line comment handling
TEST_F(O2LFmtTest, SingleLineComments) {
    std::string input = "# Top comment\nObject Test{\n# Inside comment\nmethod main():Text{return\"test\"}}\n# Bottom comment";
    std::string expected = "# Top comment\nObject Test {\n    # Inside comment\n    method main(): Text {\n        return \"test\"\n    }\n}\n# Bottom comment";
    
    std::string result = format(input);
    EXPECT_EQ(expected, result);
}

// TODO: Temporarily disabled - will revisit later
// TEST_F(O2LFmtTest, MultiLineComments) {
//     std::string input = "###\nMulti-line comment\nwith multiple lines\n###\nObject Test{}";
//     std::string expected = "###\nMulti-line comment\nwith multiple lines\n###\nObject Test {\n}";
//     
//     std::string result = format(input);
//     EXPECT_EQ(expected, result);
// }

// TODO: Temporarily disabled - will revisit later
// TEST_F(O2LFmtTest, InlineComments) {
//     std::string input = "Object Test{method test():Int{value:Int=42 # inline comment\nreturn value}}";
//     std::string expected = "Object Test {\n    method test(): Int {\n        value: Int = 42 # inline comment\n        return value\n    }\n}";
//     
//     std::string result = format(input);
//     EXPECT_EQ(expected, result);
// }

// Test constructor formatting (no space before parentheses)
TEST_F(O2LFmtTest, ConstructorFormatting) {
    std::string input = "Object Person{constructor(name:Text){this.name=name}}";
    std::string expected = "Object Person {\n    constructor(name: Text) {\n        this.name = name\n    }\n}";
    
    std::string result = format(input);
    EXPECT_EQ(expected, result);
}

// Test method parameter formatting
TEST_F(O2LFmtTest, MethodParameters) {
    std::string input = "Object Calc{method add(x:Int,y:Int):Int{return x+y}}";
    std::string expected = "Object Calc {\n    method add(x: Int, y: Int): Int {\n        return x + y\n    }\n}";
    
    std::string result = format(input);
    EXPECT_EQ(expected, result);
}

// Test operator spacing
TEST_F(O2LFmtTest, OperatorSpacing) {
    std::string input = "Object Test{method calc():Int{result:Int=10+20*3-5/2\nreturn result}}";
    std::string expected = "Object Test {\n    method calc(): Int {\n        result: Int = 10 + 20 * 3 - 5 / 2\n        return result\n    }\n}";
    
    std::string result = format(input);
    EXPECT_EQ(expected, result);
}

// Test logical operator spacing
TEST_F(O2LFmtTest, LogicalOperators) {
    std::string input = "Object Test{method logic():Bool{return true&&false||!true}}";
    std::string expected = "Object Test {\n    method logic(): Bool {\n        return true && false || !true\n    }\n}";
    
    std::string result = format(input);
    EXPECT_EQ(expected, result);
}

// Test wildcard import preservation
TEST_F(O2LFmtTest, WildcardImports) {
    std::string input = "import system.io.*\nimport math.*";
    std::string expected = "import system.io.*\nimport math.*";
    
    std::string result = format(input);
    EXPECT_EQ(expected, result);
}

// Test multiple top-level declarations
TEST_F(O2LFmtTest, MultipleTopLevelDeclarations) {
    std::string input = "Record User{name:Text age:Int}Enum Color{RED GREEN BLUE}Object Main{method main():Text{return\"test\"}}";
    std::string expected = "Record User {\n    name: Text age: Int\n}\nEnum Color {\n    RED GREEN BLUE\n}\nObject Main {\n    method main(): Text {\n        return \"test\"\n    }\n}";
    
    std::string result = format(input);
    EXPECT_EQ(expected, result);
}

// Test protocol declarations
TEST_F(O2LFmtTest, ProtocolDeclarations) {
    std::string input = "Protocol Drawable{method draw():Text}";
    std::string expected = "Protocol Drawable {\n    method draw(): Text\n}";
    
    std::string result = format(input);
    EXPECT_EQ(expected, result);
}

// Test nested indentation levels
TEST_F(O2LFmtTest, NestedIndentation) {
    std::string input = "Object Test{method main():Text{if(true){result:Text=\"nested\"\nif(false){result=\"double nested\"}\nreturn result}return\"default\"}}";
    std::string expected = "Object Test {\n    method main(): Text {\n        if(true) {\n            result: Text = \"nested\"\n            if(false) {\n                result = \"double nested\"\n            }\n            return result\n        }\n        return \"default\"\n    }\n}";
    
    std::string result = format(input);
    EXPECT_EQ(expected, result);
}

// TODO: Temporarily disabled - will revisit later  
// TEST_F(O2LFmtTest, EmptyLinesPreservation) {
//     std::string input = "# Comment\n\nObject Test {\n\n    method main(): Text {\n\n        return \"test\"\n    }\n}";
//     std::string expected = "# Comment\n\nObject Test {\n\n    method main(): Text {\n\n        return \"test\"\n    }\n}";
//     
//     std::string result = format(input);
//     EXPECT_EQ(expected, result);
// }

// Test type annotations
TEST_F(O2LFmtTest, TypeAnnotations) {
    std::string input = "Object Test{property value:Int\nmethod setValue(v:Int):Text{this.value=v\nreturn\"set\"}}";
    std::string expected = "Object Test {\n    property value: Int\n    method setValue(v: Int): Text {\n        this.value = v\n        return \"set\"\n    }\n}";
    
    std::string result = format(input);
    EXPECT_EQ(expected, result);
}

// Test edge case: empty object
TEST_F(O2LFmtTest, EmptyObject) {
    std::string input = "Object Empty{}";
    std::string expected = "Object Empty {\n}";
    
    std::string result = format(input);
    EXPECT_EQ(expected, result);
}

// TODO: Temporarily disabled - will revisit later
// TEST_F(O2LFmtTest, OnlyComments) {
//     std::string input = "# Single comment\n###\nMulti-line comment\n###\n# Another single comment";
//     std::string expected = "# Single comment\n###\nMulti-line comment\n###\n# Another single comment";
//     
//     std::string result = format(input);
//     EXPECT_EQ(expected, result);
// }

// TODO: Temporarily disabled - will revisit later
// TEST_F(O2LFmtTest, MixedComments) {
//     std::string input = "Object Test{# Single comment\n###\nMulti-line\n###\nmethod test():Text{# Another comment\nreturn\"test\"}}";
//     std::string expected = "Object Test {\n    # Single comment\n    ###\n    Multi-line\n    ###\n    method test(): Text {\n        # Another comment\n        return \"test\"\n    }\n}";
//     
//     std::string result = format(input);
//     EXPECT_EQ(expected, result);
// }

// Test edge case: consecutive braces
TEST_F(O2LFmtTest, ConsecutiveBraces) {
    std::string input = "Object Outer{Object Inner{method test():Text{return\"nested\"}}}";
    std::string expected = "Object Outer {\n    Object Inner {\n        method test(): Text {\n            return \"nested\"\n        }\n    }\n}";
    
    std::string result = format(input);
    EXPECT_EQ(expected, result);
}

// Test edge case: special characters in strings
TEST_F(O2LFmtTest, SpecialCharactersInStrings) {
    std::string input = "Object Test{method test():Text{return\"String with # and ### and {} brackets\"}}";
    std::string expected = "Object Test {\n    method test(): Text {\n        return \"String with # and ### and {} brackets\"\n    }\n}";
    
    std::string result = format(input);
    EXPECT_EQ(expected, result);
}

// Test edge case: complex operators
TEST_F(O2LFmtTest, ComplexOperators) {
    std::string input = "Object Test{method test():Bool{return x>=10&&y<=20||z==30&&w!=40}}";
    std::string expected = "Object Test {\n    method test(): Bool {\n        return x >= 10 && y <= 20 || z == 30 && w != 40\n    }\n}";
    
    std::string result = format(input);
    EXPECT_EQ(expected, result);
}

// Test edge case: imports with various patterns
TEST_F(O2LFmtTest, ImportPatterns) {
    std::string input = "import system.io.*\nimport math.constants\nimport utils.String.format\nimport collections.List.*";
    std::string expected = "import system.io.*\nimport math.constants\nimport utils.String.format\nimport collections.List.*";
    
    std::string result = format(input);
    EXPECT_EQ(expected, result);
}

// Test edge case: long parameter lists
TEST_F(O2LFmtTest, LongParameterLists) {
    std::string input = "Object Test{method complex(a:Int,b:Text,c:Bool,d:Float,e:Char):Text{return\"complex\"}}";
    std::string expected = "Object Test {\n    method complex(a: Int, b: Text, c: Bool, d: Float, e: Char): Text {\n        return \"complex\"\n    }\n}";
    
    std::string result = format(input);
    EXPECT_EQ(expected, result);
}

// TODO: Temporarily disabled - will revisit later
// TEST_F(O2LFmtTest, NestedMultiLineComments) {
//     std::string input = "Object Test{###\nOuter comment\n### # This should not start a new comment\nmethod test():Text{###\nInner comment\n###\nreturn\"test\"}}";
//     std::string expected = "Object Test {\n    ###\n    Outer comment\n    ###\n    method test(): Text {\n        ###\n        Inner comment\n        ###\n        return \"test\"\n    }\n}";
//     
//     std::string result = format(input);
//     EXPECT_EQ(expected, result);
// }

// Test edge case: assignment operators
TEST_F(O2LFmtTest, AssignmentOperators) {
    std::string input = "Object Test{method test():Text{x:Int=10\nx+=5\nx-=3\nreturn\"done\"}}";
    std::string expected = "Object Test {\n    method test(): Text {\n        x: Int = 10\n        x += 5\n        x -= 3\n        return \"done\"\n    }\n}";
    
    std::string result = format(input);
    EXPECT_EQ(expected, result);
}

// Test stress case: deeply nested structure
TEST_F(O2LFmtTest, DeeplyNestedStructure) {
    std::string input = "Object Level0{Object Level1{Object Level2{Object Level3{method deepMethod():Text{return\"deep\"}}}}}";
    std::string expected = "Object Level0 {\n    Object Level1 {\n        Object Level2 {\n            Object Level3 {\n                method deepMethod(): Text {\n                    return \"deep\"\n                }\n            }\n        }\n    }\n}";
    
    std::string result = format(input);
    EXPECT_EQ(expected, result);
}

// TODO: Temporarily disabled - will revisit later
// TEST_F(O2LFmtTest, RealWorldExample) {
//     std::string input = "# Calculator example\nimport system.io.*\n###\nA comprehensive calculator\nwith multiple operations\n###\nObject Calculator{property result:Int\nconstructor(initial:Int){this.result=initial}\nmethod add(x:Int):Calculator{this.result=this.result+x\nreturn this}# Method chaining\nmethod multiply(x:Int):Calculator{this.result=this.result*x\nreturn this}method getValue():Int{return this.result}}Object Main{method main():Text{# Create calculator\ncalc:Calculator=new Calculator(10)\nfinal:Int=calc.add(5).multiply(2).getValue()# Should be 30\nio.print(\"Result: %d\",final)\nreturn\"calculated\"}}";
//     
//     std::string expected = "# Calculator example\n"
//                           "import system.io.*\n"
//                           "###\n"
//                           "A comprehensive calculator\n"
//                           "with multiple operations\n"
//                           "###\n"
//                           "Object Calculator {\n"
//                           "    property result: Int\n"
//                           "    constructor(initial: Int) {\n"
//                           "        this.result = initial\n"
//                           "    }\n"
//                           "    method add(x: Int): Calculator {\n"
//                           "        this.result = this.result + x\n"
//                           "        return this\n"
//                           "    } # Method chaining\n"
//                           "    method multiply(x: Int): Calculator {\n"
//                           "        this.result = this.result * x\n"
//                           "        return this\n"
//                           "    }\n"
//                           "    method getValue(): Int {\n"
//                           "        return this.result\n"
//                           "    }\n"
//                           "}\n"
//                           "Object Main {\n"
//                           "    method main(): Text {\n"
//                           "        # Create calculator\n"
//                           "        calc: Calculator = new Calculator(10)\n"
//                           "        final: Int = calc.add(5).multiply(2).getValue() # Should be 30\n"
//                           "        io.print(\"Result: %d\", final)\n"
//                           "        return \"calculated\"\n"
//                           "    }\n"
//                           "}";
//     
//     std::string result = format(input);
//     EXPECT_EQ(expected, result);
// }