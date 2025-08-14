#include <iostream>
#include <sstream>
#include <string>

int main() {
    // Simulate what happens in format_code
    std::string formatted_line = "Object Test {\n}";  // This is what format_line returns
    std::cout << "formatted_line: '" << formatted_line << "'" << std::endl;
    std::cout << "Length: " << formatted_line.length() << std::endl;
    
    // Now simulate the getline processing
    std::ostringstream oss;
    std::istringstream line_stream(formatted_line);
    std::string sub_line;
    int sub_count = 0;
    while (std::getline(line_stream, sub_line)) {
        sub_count++;
        std::cout << "sub_line " << sub_count << ": '" << sub_line << "'" << std::endl;
        oss << sub_line << "\n";
    }
    
    std::string result = oss.str();
    std::cout << "Before trailing newline removal: '" << result << "'" << std::endl;
    std::cout << "Length: " << result.length() << std::endl;
    
    // Remove trailing newline if present
    if (!result.empty() && result.back() == '\n') {
        result.pop_back();
    }
    
    std::cout << "After trailing newline removal: '" << result << "'" << std::endl;
    std::cout << "Length: " << result.length() << std::endl;
    
    return 0;
}