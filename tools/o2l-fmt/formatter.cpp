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

std::string O2LFormatter::format_code(const std::string& input) {
    // Reset state for each formatting operation
    indent_level = 0;
    in_block_comment = false;
    
    std::istringstream iss(input);
    std::ostringstream oss;
    std::string line;
    
    while (std::getline(iss, line)) {
        std::string trimmed = trim(line);
        
        // Handle empty lines - preserve them
        if (trimmed.empty()) {
            oss << "\n";
            continue;
        }
        
        std::string formatted_line = format_line(line);
        if (!formatted_line.empty()) {
            // Check if this line contains newlines (from breaking up one-liners)
            std::istringstream line_stream(formatted_line);
            std::string sub_line;
            while (std::getline(line_stream, sub_line)) {
                oss << sub_line << "\n";
            }
        }
    }
    
    std::string result = oss.str();
    // Remove trailing newline if present
    if (!result.empty() && result.back() == '\n') {
        result.pop_back();
    }
    
    // Apply document-level spacing rules
    // 3 newlines after import blocks
    result = std::regex_replace(result, std::regex("(import[^\\n]*\\n)\\n*(Object|Record|Enum|Protocol)"), "$1\n\n$2");
    // 1 newline between top-level declarations (Object, Enum, Record, Protocol)
    result = std::regex_replace(result, std::regex("\\}\\n(Object|Record|Enum|Protocol)"), "}\n\n$1");
    // 1 newline between Object methods  
    result = std::regex_replace(result, std::regex("\\}\\n(\\s*)(@?\\w*\\s*method)"), "}\n\n$1$2");
    // 1 newline after Object property blocks
    result = std::regex_replace(result, std::regex("(property[^\\n]*\\n)(\\s*)(method|constructor)"), "$1\n$2$3");
    
    return result;
}

std::string O2LFormatter::format_line(const std::string& line) {
    std::string trimmed = trim(line);
    
    // Skip empty lines
    if (trimmed.empty()) {
        return "";
    }
    
    // Apply indentation logic to ALL lines (not just broken-up ones)
    
    // Skip closing brace handling here - it's handled in sub-line processing
    
    // Determine if this is actually a top-level declaration (indent_level == 0)
    int current_indent_level = indent_level;
    bool is_truly_top_level = (indent_level == 0 && (trimmed.starts_with("Object ") || 
        trimmed.starts_with("Record ") || 
        trimmed.starts_with("Enum ") || 
        trimmed.starts_with("Protocol ")));
        
    if (is_truly_top_level) {
        current_indent_level = 0;
        // Keep indent_level at 0 for top-level, but allow nested declarations
    }
    
    // Handle single-line comments (#)
    if (trimmed.starts_with("#") && !trimmed.starts_with("###")) {
        std::string result = get_indent_for_level(current_indent_level) + trimmed;
        // Handle opening braces (increase indent after line)
        if (ends_with_open_brace(trimmed)) {
            indent_level++;
        }
        return result;
    }
    
    // Handle multi-line comments (###)
    if (trimmed.starts_with("###")) {
        // Check if this line also ends the comment
        if (trimmed.ends_with("###") && trimmed.length() == 3) {
            // This is a single-line ### comment, don't change block comment state
            in_block_comment = false;
        } else {
            in_block_comment = true;
        }
        std::string result = get_indent_for_level(current_indent_level) + trimmed;
        // Handle opening braces (increase indent after line)
        if (ends_with_open_brace(trimmed)) {
            indent_level++;
        }
        return result;
    }
    
    if (in_block_comment) {
        if (trimmed.ends_with("###")) {
            in_block_comment = false;
        }
        // Return comment lines without any formatting processing
        std::string result = get_indent_for_level(current_indent_level) + trimmed;
        // Handle opening braces (increase indent after line)  
        if (ends_with_open_brace(trimmed)) {
            indent_level++;
        }
        return result;
    }
    
    // Break up one-liners into multiple lines and process each sub-line
    std::string broken_up = break_up_oneliners(trimmed);
    std::istringstream sub_stream(broken_up);
    std::ostringstream result_stream;
    std::string sub_line;
    
    while (std::getline(sub_stream, sub_line)) {
        sub_line = trim(sub_line);
        if (sub_line.empty()) continue;
        
        
        // Determine if this is actually a top-level declaration (check BEFORE modifying indent_level)
        bool is_truly_top_level_sub = (indent_level == 0 && (sub_line.starts_with("Object ") || 
            sub_line.starts_with("Record ") || 
            sub_line.starts_with("Enum ") || 
            sub_line.starts_with("Protocol ")));
        
        int current_sub_indent_level = indent_level;
        
        // Handle closing braces (use proper indent, then decrease for next lines)
        if (starts_with_close_brace(sub_line)) {
            // Closing brace should be at same level as its matching opening brace
            current_sub_indent_level = std::max(0, indent_level - 1);
            // Then decrease indent for subsequent lines
            indent_level = std::max(0, indent_level - 1);
        }
            
        if (is_truly_top_level_sub) {
            current_sub_indent_level = 0;
            // Keep indent_level at 0 for top-level, but allow nested declarations
        }
        
        std::string formatted_sub = format_statement(sub_line);
        
        // Safety check: if format_statement returns empty for a closing brace, preserve it
        if (formatted_sub.empty() && starts_with_close_brace(sub_line)) {
            formatted_sub = sub_line;  // Preserve the original sub_line
        }
        
        result_stream << get_indent_for_level(current_sub_indent_level) << formatted_sub;
        
        // Handle opening braces (increase indent after line)
        if (ends_with_open_brace(sub_line)) {
            indent_level++;
        }
        
        // Add newline unless this is the last sub-line
        result_stream << "\n";
    }
    
    std::string result = result_stream.str();
    // Remove trailing newline
    if (!result.empty() && result.back() == '\n') {
        result.pop_back();
    }
    
    // Brace checking moved to main entry point
    
    return result;
}

std::string O2LFormatter::format_code_with_validation(const std::string& input) {
    std::string result = format_code(input);
    
    // Check for unbalanced braces in the complete document
    int open_count = 0, close_count = 0;
    for (char c : result) {
        if (c == '{') open_count++;
        if (c == '}') close_count++;
    }
    
    if (open_count != close_count) {
        throw std::runtime_error("Unbalanced braces detected: " + std::to_string(open_count) + " open, " + std::to_string(close_count) + " close");
    }
    
    return result;
}

std::string O2LFormatter::extract_inline_comment(const std::string& line, std::string& code_part) {
    // Look for # comments but not ### (multi-line) 
    size_t comment_pos = line.find('#');
    while (comment_pos != std::string::npos) {
        // Check if it's not part of ### - more robust check
        bool is_multiline_start = (comment_pos + 2 < line.length() && 
                                  line.substr(comment_pos, 3) == "###");
        bool is_inside_string = false;
        
        // Check if # is inside a string literal
        size_t quote_count = 0;
        for (size_t i = 0; i < comment_pos; i++) {
            if (line[i] == '"' && (i == 0 || line[i-1] != '\\')) {
                quote_count++;
            }
        }
        is_inside_string = (quote_count % 2 == 1);
        
        if (!is_multiline_start && !is_inside_string) {
            // Found a single-line comment - extract it
            code_part = trim(line.substr(0, comment_pos));
            std::string comment = line.substr(comment_pos);
            return comment;
        }
        comment_pos = line.find('#', comment_pos + 1);
    }
    code_part = line;
    return "";
}

std::string O2LFormatter::break_up_oneliners(const std::string& line) {
    std::string result = line;
    
    // Protect string literals by replacing them with placeholders
    std::vector<std::string> strings;
    std::string protected_result = result;
    
    // Find and replace all string literals
    std::regex string_regex("\"[^\"]*\"");
    std::sregex_iterator iter(result.begin(), result.end(), string_regex);
    std::sregex_iterator end;
    
    int string_index = 0;
    for (std::sregex_iterator i = iter; i != end; ++i) {
        std::smatch match = *i;
        strings.push_back(match.str());
        std::string placeholder = "STRING_PLACEHOLDER_" + std::to_string(string_index++);
        protected_result = std::regex_replace(protected_result, std::regex("\"[^\"]*\""), placeholder, std::regex_constants::format_first_only);
    }
    
    result = protected_result;
    
    // Step 0: Break after import statements when followed by keywords  
    result = std::regex_replace(result, std::regex("(import\\s+[^\\n\\{]+)(Object|Record|Enum|Protocol)"), "$1\n\n\n$2");
    
    // Step 1: Break after opening braces (INCLUDING empty braces for proper formatting)
    result = std::regex_replace(result, std::regex("\\{"), "{\n");
    
    // Step 2: Break before closing braces (including empty braces)
    result = std::regex_replace(result, std::regex("([^\\n])\\s*\\}"), "$1\n}");
    
    // Step 3: Break after closing braces followed by any word character (including Object, method, etc.)
    result = std::regex_replace(result, std::regex("\\}\\s*(\\w)"), "}\n$1");
    
    // Step 4: Break multiple consecutive closing braces
    result = std::regex_replace(result, std::regex("\\}\\s*\\}"), "}\n}");
    
    // Step 5: Apply brace breaking again to catch any patterns created by previous steps
    result = std::regex_replace(result, std::regex("\\}\\s*(\\w)"), "}\n$1");
    
    // Step 6: Clean up excessive newlines (multiple passes)
    result = std::regex_replace(result, std::regex("\\n\\s*\\n\\s*\\n+"), "\n\n");
    result = std::regex_replace(result, std::regex("\\n\\n\\n+"), "\n\n");
    
    // Restore string literals
    for (size_t i = 0; i < strings.size(); i++) {
        std::string placeholder = "STRING_PLACEHOLDER_" + std::to_string(i);
        result = std::regex_replace(result, std::regex(placeholder), strings[i]);
    }
    
    return result;
}

std::string O2LFormatter::format_statement(const std::string& stmt) {
    // Handle pure closing braces first to avoid any processing
    if (stmt == "}") {
        return "}";
    }
    
    std::string result = stmt;
    
    // Add space after keywords
    result = add_space_after_keywords(result);
    
    // Format object/method declarations
    result = format_declarations(result);
    
    // Format operators with proper spacing
    result = format_operators(result);
    
    // Format colons in type annotations
    result = format_type_annotations(result);
    
    return result;
}

std::string O2LFormatter::add_space_after_keywords(const std::string& line) {
    std::string result = line;
    
    // Handle keywords that should never have space before parentheses
    std::vector<std::string> no_space_before_parens = {"constructor"};
    for (const auto& keyword : no_space_before_parens) {
        std::regex pattern("\\b" + keyword + "\\b(?!\\s|\\()");
        result = std::regex_replace(result, pattern, keyword + " ");
    }
    
    // Handle other keywords that should have space when followed by identifiers
    std::vector<std::string> other_keywords = {
        "Protocol", "Record", "Enum", "property", "Object", "method", "import", 
        "return", "while", "this", "else", "for", "if"
    };
    
    for (const auto& keyword : other_keywords) {
        // Add space after keyword if not already present and followed by word character, quote, or certain symbols
        // Use word boundaries to ensure we only match complete keywords, not parts of other words
        std::regex pattern("\\b" + keyword + "\\b(?=[\\w\"'0-9])");
        result = std::regex_replace(result, pattern, keyword + " ");
    }
    
    return result;
}

std::string O2LFormatter::format_declarations(const std::string& line) {
    std::string result = line;
    
    // Format object declarations: "Object Name {"
    result = std::regex_replace(result, std::regex("Object\\s+(\\w+)\\s*\\{"), "Object $1 {");
    
    // Format record declarations: "Record Name {"
    result = std::regex_replace(result, std::regex("Record\\s+(\\w+)\\s*\\{"), "Record $1 {");
    
    // Format enum declarations: "Enum Name {"
    result = std::regex_replace(result, std::regex("Enum\\s+(\\w+)\\s*\\{"), "Enum $1 {");
    
    // Format protocol declarations: "Protocol Name {"
    result = std::regex_replace(result, std::regex("Protocol\\s+(\\w+)\\s*\\{"), "Protocol $1 {");
    
    // Format method declarations: "method name(): Type {"
    result = std::regex_replace(result, std::regex("method\\s+(\\w+)\\s*\\("), "method $1(");
    
    // Format method return types: "): Type {" -> "): Type {"
    result = std::regex_replace(result, std::regex("\\)\\s*:\\s*(\\w+)\\s*\\{"), "): $1 {");
    
    // Format method return types without braces: "): Type" -> "): Type"
    result = std::regex_replace(result, std::regex("\\)\\s*:\\s*(\\w+)(?!\\s)"), "): $1");
    
    // Format constructor with space before brace: "constructor(...) {"
    result = std::regex_replace(result, std::regex("(constructor\\([^)]*\\))\\s*\\{"), "$1 {");
    
    // Format control structures with space before brace
    result = std::regex_replace(result, std::regex("(if\\s*\\([^)]*\\))\\s*\\{"), "$1 {");
    result = std::regex_replace(result, std::regex("(while\\s*\\([^)]*\\))\\s*\\{"), "$1 {");
    result = std::regex_replace(result, std::regex("(else)\\s*\\{"), "$1 {");
    
    // Format property declarations: "property name: Type"
    result = std::regex_replace(result, std::regex("property\\s+(\\w+)\\s*:"), "property $1:");
    
    // Format parameter lists with proper spacing: "(x:Int,y:Int)" -> "(x: Int, y: Int)"
    result = std::regex_replace(result, std::regex("(\\w+)\\s*:\\s*(\\w+)\\s*,\\s*"), "$1: $2, ");
    result = std::regex_replace(result, std::regex("(\\w+)\\s*:\\s*(\\w+)\\s*\\)"), "$1: $2)");
    
    return result;
}

std::string O2LFormatter::format_operators(const std::string& line) {
    std::string result = line;
    
    // Don't format operators in lines that start with comments (but allow inline comments)
    std::string trimmed = trim(result);
    if (trimmed.starts_with("#")) {
        return result;
    }
    
    // Extract and protect inline comments
    std::string code_part;
    std::string inline_comment = extract_inline_comment(result, code_part);
    
    // Only format the code part, not the comment
    if (!inline_comment.empty()) {
        result = code_part;
    }
    
    // Protect string literals from operator formatting
    std::vector<std::string> strings;
    std::string protected_result = result;
    
    // Find and replace all string literals with placeholders
    std::regex string_regex("\"[^\"]*\"");
    std::sregex_iterator iter(result.begin(), result.end(), string_regex);
    std::sregex_iterator end;
    
    int string_index = 0;
    for (std::sregex_iterator i = iter; i != end; ++i) {
        std::smatch match = *i;
        strings.push_back(match.str());
        std::string placeholder = "STRING_LITERAL_" + std::to_string(string_index++);
        protected_result = std::regex_replace(protected_result, std::regex("\"[^\"]*\""), placeholder, std::regex_constants::format_first_only);
    }
    
    result = protected_result;
    
    // Use placeholders to protect compound operators while formatting
    result = std::regex_replace(result, std::regex("\\s*>=\\s*"), "PLACEHOLDER_GTE");
    result = std::regex_replace(result, std::regex("\\s*<=\\s*"), "PLACEHOLDER_LTE");
    result = std::regex_replace(result, std::regex("\\s*==\\s*"), "PLACEHOLDER_EQ");
    result = std::regex_replace(result, std::regex("\\s*!=\\s*"), "PLACEHOLDER_NE");
    result = std::regex_replace(result, std::regex("\\s*\\+=\\s*"), "PLACEHOLDER_PLUS_EQ");
    result = std::regex_replace(result, std::regex("\\s*-=\\s*"), "PLACEHOLDER_MINUS_EQ");
    result = std::regex_replace(result, std::regex("\\s*\\*=\\s*"), "PLACEHOLDER_MULT_EQ");
    result = std::regex_replace(result, std::regex("\\s*/=\\s*"), "PLACEHOLDER_DIV_EQ");
    result = std::regex_replace(result, std::regex("\\s*%=\\s*"), "PLACEHOLDER_MOD_EQ");
    result = std::regex_replace(result, std::regex("\\s*&&\\s*"), "PLACEHOLDER_AND");
    result = std::regex_replace(result, std::regex("\\s*\\|\\|\\s*"), "PLACEHOLDER_OR");
    
    // Now format single operators
    result = std::regex_replace(result, std::regex("\\s*=\\s*"), " = ");
    result = std::regex_replace(result, std::regex("\\s*<\\s*"), " < ");
    result = std::regex_replace(result, std::regex("\\s*>\\s*"), " > ");
    result = std::regex_replace(result, std::regex("\\s*\\+\\s*"), " + ");
    result = std::regex_replace(result, std::regex("\\s*-\\s*"), " - ");
    result = std::regex_replace(result, std::regex("\\s*/\\s*"), " / ");
    result = std::regex_replace(result, std::regex("%\\s*"), "% ");
    
    // Format * operator, but preserve .* for wildcard imports
    if (result.find(".*") == std::string::npos) {
        result = std::regex_replace(result, std::regex("\\s*\\*\\s*"), " * ");
    }
    
    // Restore compound operators with proper spacing
    result = std::regex_replace(result, std::regex("PLACEHOLDER_GTE"), " >= ");
    result = std::regex_replace(result, std::regex("PLACEHOLDER_LTE"), " <= ");
    result = std::regex_replace(result, std::regex("PLACEHOLDER_EQ"), " == ");
    result = std::regex_replace(result, std::regex("PLACEHOLDER_NE"), " != ");
    result = std::regex_replace(result, std::regex("PLACEHOLDER_PLUS_EQ"), " += ");
    result = std::regex_replace(result, std::regex("PLACEHOLDER_MINUS_EQ"), " -= ");
    result = std::regex_replace(result, std::regex("PLACEHOLDER_MULT_EQ"), " *= ");
    result = std::regex_replace(result, std::regex("PLACEHOLDER_DIV_EQ"), " /= ");
    result = std::regex_replace(result, std::regex("PLACEHOLDER_MOD_EQ"), " %= ");
    result = std::regex_replace(result, std::regex("PLACEHOLDER_AND"), " && ");
    result = std::regex_replace(result, std::regex("PLACEHOLDER_OR"), " || ");
    
    // Restore string literals
    for (size_t i = 0; i < strings.size(); i++) {
        std::string placeholder = "STRING_LITERAL_" + std::to_string(i);
        result = std::regex_replace(result, std::regex(placeholder), strings[i]);
    }
    
    // Restore inline comment if it existed
    if (!inline_comment.empty()) {
        result = result + " " + inline_comment;
    }
    
    return result;
}

std::string O2LFormatter::format_type_annotations(const std::string& line) {
    std::string result = line;
    
    // Format type annotations: "name: Type" with space after colon
    result = std::regex_replace(result, std::regex("(\\w+)\\s*:\\s*(\\w+)"), "$1: $2");
    
    return result;
}

std::string O2LFormatter::get_indent() {
    std::string result;
    for (int i = 0; i < indent_level; i++) {
        result += indent_str;
    }
    return result;
}

std::string O2LFormatter::get_indent_for_level(int level) {
    std::string result;
    for (int i = 0; i < level; i++) {
        result += indent_str;
    }
    return result;
}

bool O2LFormatter::starts_with_close_brace(const std::string& line) {
    return line.starts_with("}");
}

bool O2LFormatter::ends_with_open_brace(const std::string& line) {
    return line.ends_with("{");
}

std::string O2LFormatter::trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t");
    if (start == std::string::npos) return "";
    size_t end = str.find_last_not_of(" \t");
    return str.substr(start, end - start + 1);
}