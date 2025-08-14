#include "formatter.hpp"
#include <iostream>
#include <sstream>

class DebugO2LFormatter : public O2LFormatter {
public:
    std::string debug_format_code(const std::string& input) {
        std::istringstream iss(input);
        std::string line;
        int line_num = 0;
        
        while (std::getline(iss, line)) {
            line_num++;
            std::cout << "Line " << line_num << " input: '" << line << "'" << std::endl;
            
            if (line.find("Object Test{}") != std::string::npos) {
                // Test this line specifically
                std::string formatted = format_code(line);
                std::cout << "Line " << line_num << " formatted alone: '" << formatted << "'" << std::endl;
            }
        }
        
        // Now format the whole thing
        return format_code(input);
    }
};

int main() {
    DebugO2LFormatter formatter;
    
    std::string test1 = "Object Test{}";
    std::string result1 = formatter.format_code(test1);
    std::cout << "Single line test:" << std::endl;
    std::cout << "Input: '" << test1 << "'" << std::endl;
    std::cout << "Output: '" << result1 << "'" << std::endl;
    
    std::cout << "\n=== Debug multiline processing ===" << std::endl;
    std::string test2 = "###\nMulti-line comment\nwith multiple lines\n###\nObject Test{}";
    std::string result2 = formatter.debug_format_code(test2);
    std::cout << "\nFinal multiline output:" << std::endl;
    std::cout << "'" << result2 << "'" << std::endl;
    
    return 0;
}