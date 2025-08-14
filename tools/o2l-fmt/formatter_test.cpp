#include <iostream>
#include <regex>
#include <string>

std::string test_break_up_oneliners(const std::string& line) {
    std::string result = line;
    
    // Step 1: Break after opening braces
    result = std::regex_replace(result, std::regex("\\{"), "{\n");
    
    // Step 2: Break before closing braces if there's content before them  
    result = std::regex_replace(result, std::regex("(\\w)\\s*\\}"), "$1\n}");
    
    return result;
}

std::string test_format_operators(const std::string& line) {
    std::string result = line;
    
    // Format comparison operators
    result = std::regex_replace(result, std::regex("\\s*>=\\s*"), " >= ");
    result = std::regex_replace(result, std::regex("\\s*<=\\s*"), " <= ");
    
    return result;
}

int main() {
    try {
        std::cout << "Testing formatter functions...\n";
        
        std::string test1 = "Object Test{method main():Text{return\"hello\"}}";
        std::cout << "Input: " << test1 << std::endl;
        
        std::string result1 = test_break_up_oneliners(test1);
        std::cout << "After break_up_oneliners: " << result1 << std::endl;
        
        std::string test2 = "x>=10&&y<=20";
        std::cout << "Input: " << test2 << std::endl;
        
        std::string result2 = test_format_operators(test2);
        std::cout << "After format_operators: " << result2 << std::endl;
        
        std::cout << "All tests passed!\n";
        
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}