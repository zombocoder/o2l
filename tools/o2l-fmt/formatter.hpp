#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <regex>

class O2LFormatter {
private:
    int indent_level = 0;
    const std::string indent_str = "    "; // 4 spaces
    bool in_block_comment = false;
    
public:
    std::string format_code(const std::string& input);
    
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