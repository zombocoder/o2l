#include "formatter.hpp"
#include <iostream>

int main() {
    O2LFormatter formatter;
    
    std::string test_line = "value:Int=42 # inline comment";
    std::cout << "Original line: '" << test_line << "'" << std::endl;
    
    // Test extract_inline_comment function
    std::string code_part;
    std::string comment_result = formatter.extract_inline_comment(test_line, code_part);
    
    std::cout << "Code part: '" << code_part << "'" << std::endl; 
    std::cout << "Comment part: '" << comment_result << "'" << std::endl;
    
    // Test format_code on the whole line
    std::string formatted = formatter.format_code(test_line);
    std::cout << "Final formatted: '" << formatted << "'" << std::endl;
    
    return 0;
}