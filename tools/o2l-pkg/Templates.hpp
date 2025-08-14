#pragma once

#include <string>
#include <map>
#include <vector>

class TemplateEngine {
private:
    std::string replaceVariables(const std::string& template_str, const std::map<std::string, std::string>& variables) {
        std::string result = template_str;
        for (const auto& [key, value] : variables) {
            std::string placeholder = "{{" + key + "}}";
            size_t pos = 0;
            while ((pos = result.find(placeholder, pos)) != std::string::npos) {
                result.replace(pos, placeholder.length(), value);
                pos += value.length();
            }
        }
        return result;
    }
    
public:
    static std::string renderCalculatorFile() {
        return R"(# Calculator module - Basic arithmetic operations
import system.io

Object Calculator {
    @external method add(a: Int, b: Int): Int {
        return a + b
    }
    
    @external method multiply(a: Int, b: Int): Int {
        return a * b
    }
    
    @external method divide(a: Int, b: Int): Int {
        if (b == 0) {
            io.print("Error: Division by zero!")
            return 0
        }
        return a / b
    }
    
    @external method subtract(a: Int, b: Int): Int {
        return a - b
    }
    
    @external method power(base: Int, exponent: Int): Int {
        if (exponent == 0) {
            return 1
        }
        result: Int = 1
        i: Int = 0
        while (i < exponent) {
            result = result * base
            i = i + 1
        }
        return result
    }
}
)";
    }
    
    static std::string renderGreeterFile(const std::string& project_name) {
        const std::string template_str = R"(# Greeter module - Welcome and greeting functionality

Object BasicGreeter {
    @external method greet(name: Text): Text {
        return "Hello from {{project_name}}, " + name + "!"
    }
    
    @external method welcome(): Text {
        return "Welcome to {{project_name}}! This is an O²L application."
    }
    
    @external method farewell(name: Text): Text {
        return "Goodbye, " + name + "! Thanks for using {{project_name}}."
    }
}
)";
        
        TemplateEngine engine;
        return engine.replaceVariables(template_str, {{"project_name", project_name}});
    }

    static std::string renderMainFile(const std::string& project_name) {
        const std::string template_str = R"(# {{project_name}} - Main Application
import system.io
@import src.calc.Calculator
@import src.greeters.BasicGreeter

Object Main {
    method main(): Int {
        greeter: BasicGreeter = new BasicGreeter()
        io.print(greeter.welcome())
        
        # Test the Calculator
        calc: Calculator = new Calculator()
        sum: Int = calc.add(5, 3)
        product: Int = calc.multiply(4, 7)
        difference: Int = calc.subtract(10, 3)
        
        io.print("5 + 3 = %d", sum)
        io.print("4 * 7 = %d", product)
        io.print("10 - 3 = %d", difference)
        
        greeting: Text = greeter.greet("Developer")
        io.print(greeting)
        
        farewell: Text = greeter.farewell("Developer")
        io.print(farewell)
        
        return 0
    }
})";
        
        TemplateEngine engine;
        return engine.replaceVariables(template_str, {{"project_name", project_name}});
    }
    
    static std::string renderCalculatorTestFile(const std::string& /* project_name */) {
        return R"(# Calculator Test Suite
import testing
import system.io
@import src.calc.Calculator

Object CalculatorTest {
    @external method testAddition(): Text {
        testing.runTest("Addition Test")
        calc: Calculator = new Calculator()
        result: Int = calc.add(5, 3)
        testing.assertEqual(8, result, "5 + 3 should equal 8")
        return "Addition test completed"
    }
    
    @external method testMultiplication(): Text {
        testing.runTest("Multiplication Test")
        calc: Calculator = new Calculator()
        product: Int = calc.multiply(4, 7)
        testing.assertEqual(28, product, "4 * 7 should equal 28")
        return "Multiplication test completed"
    }
    
    @external method testDivision(): Text {
        testing.runTest("Division Test")
        calc: Calculator = new Calculator()
        quotient: Int = calc.divide(15, 3)
        testing.assertEqual(5, quotient, "15 / 3 should equal 5")
        return "Division test completed"
    }
    
    @external method testSubtraction(): Text {
        testing.runTest("Subtraction Test")
        calc: Calculator = new Calculator()
        difference: Int = calc.subtract(10, 3)
        testing.assertEqual(7, difference, "10 - 3 should equal 7")
        return "Subtraction test completed"
    }
    
    @external method testDivisionByZero(): Text {
        testing.runTest("Division by Zero Test")
        calc: Calculator = new Calculator()
        zero_result: Int = calc.divide(10, 0)
        testing.assertEqual(0, zero_result, "Division by zero should return 0")
        return "Division by zero test completed"
    }
    
    @external method testPower(): Text {
        testing.runTest("Power Test")
        calc: Calculator = new Calculator()
        power_result: Int = calc.power(2, 3)
        testing.assertEqual(8, power_result, "2^3 should equal 8")
        return "Power test completed"
    }
    
    @external method runAllTests(): Text {
        testing.createSuite("Calculator Tests")
        
        this.testAddition()
        this.testMultiplication()
        this.testDivision()
        this.testSubtraction()
        this.testDivisionByZero()
        this.testPower()
        
        return "Calculator tests completed"
    }
}
)";
    }
    
    static std::string renderGreeterTestFile(const std::string& project_name) {
        const std::string template_str = R"(# Greeter Test Suite
import testing
import system.io
@import src.greeters.BasicGreeter

Object GreeterTest {
    @external method testGreeting(): Text {
        testing.runTest("Greeting Test")
        greeter: BasicGreeter = new BasicGreeter()
        greeting: Text = greeter.greet("World")
        expected: Text = "Hello from {{project_name}}, World!"
        testing.assertEqual(expected, greeting, "Greeting should match expected format")
        return "Greeting test completed"
    }
    
    @external method testWelcome(): Text {
        testing.runTest("Welcome Test")
        greeter: BasicGreeter = new BasicGreeter()
        welcome: Text = greeter.welcome()
        testing.assertContains(welcome, "{{project_name}}", "Welcome should contain project name")
        testing.assertStartsWith(welcome, "Welcome to", "Welcome should start with 'Welcome to'")
        return "Welcome test completed"
    }
    
    @external method testFarewell(): Text {
        testing.runTest("Farewell Test")
        greeter: BasicGreeter = new BasicGreeter()
        farewell: Text = greeter.farewell("Developer")
        testing.assertContains(farewell, "Developer", "Farewell should contain the name")
        testing.assertStartsWith(farewell, "Goodbye", "Farewell should start with 'Goodbye'")
        return "Farewell test completed"
    }
    
    @external method testWelcomeFormat(): Text {
        testing.runTest("Welcome Format Test")
        greeter: BasicGreeter = new BasicGreeter()
        welcome: Text = greeter.welcome()
        expected: Text = "Welcome to {{project_name}}! This is an O²L application."
        testing.assertEqual(expected, welcome, "Welcome message should match expected format")
        return "Welcome format test completed"
    }
    
    @external method runAllTests(): Text {
        testing.createSuite("Greeter Tests")
        
        this.testGreeting()
        this.testWelcome()
        this.testFarewell()
        this.testWelcomeFormat()
        
        return "Greeter tests completed"
    }
}
)";
        
        TemplateEngine engine;
        return engine.replaceVariables(template_str, {{"project_name", project_name}});
    }
    
    static std::string renderMainTestFile(const std::string& project_name) {
        const std::string template_str = R"(# Main Test Runner for {{project_name}}
import testing
import system.io
@import src.tests.calc.CalculatorTest
@import src.tests.greeters.GreeterTest

Object Main {
    method main(): Text {
        io.print("Running tests for {{project_name}}...")
        
        # Run Calculator tests
        calc_tests: CalculatorTest = new CalculatorTest()
        calc_tests.runAllTests()
        
        # Run Greeter tests
        greeter_tests: GreeterTest = new GreeterTest()
        greeter_tests.runAllTests()
        
        # Print results
        testing.printResults()
        
        total: Int = testing.getTotalCount()
        passed: Int = testing.getPassedCount()
        failed: Int = testing.getFailedCount()
        
        io.print("Test Summary: %d total, %d passed, %d failed", total, passed, failed)
        
        if (failed == 0) {
            io.print("All tests passed! ✅")
            return "All tests passed!"
        } else {
            io.print("Some tests failed! ❌")
            return "Some tests failed!"
        }
    }
})";
        
        TemplateEngine engine;
        return engine.replaceVariables(template_str, {{"project_name", project_name}});
    }
    
    static std::string renderGitignore() {
        return R"(# O²L package manager
.o2l/cache/
.o2l/lib/*/

# Build artifacts
build/
*.o
*.so
*.dll
)";
    }
    
    static std::string renderTomlConfig(const std::string& name, const std::string& version, 
                                       const std::string& description, const std::vector<std::string>& authors,
                                       const std::map<std::string, std::string>& dependencies) {
        const std::string template_str = R"([package]
name = "{{name}}"
version = "{{version}}"
description = "{{description}}"
authors = [{{authors}}]
entrypoint = "src/main.obq"

{{dependencies_section}})";
        
        // Build authors list
        std::string authors_str;
        for (size_t i = 0; i < authors.size(); ++i) {
            authors_str += "\"" + authors[i] + "\"";
            if (i < authors.size() - 1) authors_str += ", ";
        }
        
        // Build dependencies section
        std::string dependencies_str;
        if (!dependencies.empty()) {
            dependencies_str = "[dependencies]\n";
            for (const auto& [dep_name, dep_version] : dependencies) {
                dependencies_str += dep_name + " = \"" + dep_version + "\"\n";
            }
        }
        
        TemplateEngine engine;
        return engine.replaceVariables(template_str, {
            {"name", name},
            {"version", version},
            {"description", description},
            {"authors", authors_str},
            {"dependencies_section", dependencies_str}
        });
    }
    
    static std::string renderLibraryFile(const std::string& library_name, const std::string& version) {
        const std::string template_str = R"(# Library: {{library_name}}
# Version: {{version}}
# This is a placeholder - real implementation would download from registry

Object {{library_name}} {
    @external method version(): Text {
        "{{version}}"
    }
    
    @external method info(): Text {
        "{{library_name}} library v{{version}}"
    }
})";
        
        TemplateEngine engine;
        return engine.replaceVariables(template_str, {
            {"library_name", library_name},
            {"version", version}
        });
    }
    
    static std::string renderNamespacedLibraryFile(const std::string& full_namespace, const std::string& class_name, const std::string& version) {
        const std::string template_str = R"(# Namespaced Library: {{full_namespace}}
# Version: {{version}}
# This is a placeholder - real implementation would download from registry

namespace {{namespace_path}} {
    Object {{class_name}} {
        @external method version(): Text {
            "{{version}}"
        }
        
        @external method info(): Text {
            "{{full_namespace}} library v{{version}}"
        }
        
        @external method getName(): Text {
            "{{class_name}}"
        }
    }
})";
        
        // Extract namespace path (everything except the last part)
        std::string namespace_path = full_namespace;
        size_t last_dot = namespace_path.find_last_of('.');
        if (last_dot != std::string::npos) {
            namespace_path = namespace_path.substr(0, last_dot);
        }
        
        TemplateEngine engine;
        return engine.replaceVariables(template_str, {
            {"full_namespace", full_namespace},
            {"namespace_path", namespace_path},
            {"class_name", class_name},
            {"version", version}
        });
    }
    
    static std::string renderLibraryIndex(const std::string& library_name, const std::vector<std::string>& /* parts */) {
        const std::string template_str = R"(# Index file for {{library_name}}
# This file re-exports the main library for convenience

# Import the actual implementation
import {{library_name}}

# Re-export for easier access
# Users can now do: import {{library_name}} directly
)";
        
        TemplateEngine engine;
        return engine.replaceVariables(template_str, {
            {"library_name", library_name}
        });
    }
    
    static std::string renderNamespaceClass(const std::string& full_namespace, const std::string& class_name) {
        const std::string template_str = R"(# {{class_name}} object
# Generated by o2l-pkg create

Object {{class_name}} {
    @external method getName(): Text {
        "{{class_name}}"
    }
    
    @external method getPath(): Text {
        "{{full_namespace}}"
    }
    
    @external method hello(name: Text): Text {
        "Hello from {{class_name}}, " + name + "!"
    }
    
    @external method info(): Text {
        "{{class_name}} object at {{full_namespace}}"
    }
}
)";
        
        TemplateEngine engine;
        return engine.replaceVariables(template_str, {
            {"full_namespace", full_namespace},
            {"class_name", class_name}
        });
    }
};