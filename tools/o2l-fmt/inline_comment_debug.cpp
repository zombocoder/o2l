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