#include <iostream>
#include <regex>
#include <string>

int main() {
    std::string test = "test{content}";
    
    try {
        std::cout << "Testing regex patterns...\n";
        
        // Test each regex pattern individually
        std::regex r1("\\{");
        std::cout << "1. \\{ - OK\n";
        
        std::regex r2("(\\w)\\s*\\}");
        std::cout << "2. (\\w)\\s*\\} - OK\n";
        
        std::regex r3("\\}\\s*(\\w)");
        std::cout << "3. \\}\\s*(\\w) - OK\n";
        
        std::regex r4("\\}\\s*\\}");
        std::cout << "4. \\}\\s*\\} - OK\n";
        
        std::regex r5("\\n\\s*\\n\\s*\\n+");
        std::cout << "5. \\n\\s*\\n\\s*\\n+ - OK\n";
        
        std::regex r6("\\s*>=\\s*");
        std::cout << "6. \\s*>=\\s* - OK\n";
        
        std::regex r7("\\s*\\|\\|\\s*");
        std::cout << "7. \\s*\\|\\|\\s* - OK\n";
        
        std::cout << "All regex patterns compiled successfully!\n";
        
    } catch (const std::exception& e) {
        std::cout << "Regex compilation error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}