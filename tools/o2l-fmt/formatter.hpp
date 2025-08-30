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

#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <regex>
#include <stdexcept>

class O2LFormatter {
private:
    int indent_level = 0;
    std::string indent_str = "    "; // 4 spaces default
    bool in_block_comment = false;
    
public:
    O2LFormatter(int indent_size = 4) {
        indent_str = std::string(indent_size, ' ');
    }
    
    std::string format_code(const std::string& input);
    std::string format_code_with_validation(const std::string& input);
    
private:
    std::string format_line(const std::string& line);
    std::string extract_inline_comment(const std::string& line, std::string& code_part);
    std::string break_up_oneliners(const std::string& line);
    std::string format_statement(const std::string& stmt);
    std::string add_space_after_keywords(const std::string& line);
    std::string format_declarations(const std::string& line);
    std::string format_operators(const std::string& line);
    std::string format_type_annotations(const std::string& line);
    std::string get_indent();
    std::string get_indent_for_level(int level);
    bool starts_with_close_brace(const std::string& line);
    bool ends_with_open_brace(const std::string& line);
    std::string trim(const std::string& str);
};