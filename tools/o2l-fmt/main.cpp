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

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <sstream>
#include <algorithm>
#include <regex>
#include "formatter.hpp"

namespace fs = std::filesystem;

struct FormattingConfig {
    int indent_size = 4;
};

class O2LFmtTool {
private:
    FormattingConfig load_config_from_toml(const std::string& project_dir) {
        FormattingConfig config;
        fs::path toml_path = fs::path(project_dir) / "o2l.toml";
        
        if (!fs::exists(toml_path)) {
            return config; // Return default config
        }
        
        std::ifstream file(toml_path);
        if (!file.is_open()) {
            return config;
        }
        
        std::string line;
        bool in_formatting_section = false;
        
        while (std::getline(file, line)) {
            // Trim whitespace
            line.erase(line.find_last_not_of(" \t\r\n") + 1);
            line.erase(0, line.find_first_not_of(" \t\r\n"));
            
            if (line.empty() || line[0] == '#') continue;
            
            if (line == "[formatting]") {
                in_formatting_section = true;
                continue;
            }
            
            if (line[0] == '[' && line != "[formatting]") {
                in_formatting_section = false;
                continue;
            }
            
            if (in_formatting_section) {
                auto eq_pos = line.find('=');
                if (eq_pos != std::string::npos) {
                    std::string key = line.substr(0, eq_pos);
                    std::string value = line.substr(eq_pos + 1);
                    
                    // Trim key and value
                    key.erase(key.find_last_not_of(" \t") + 1);
                    key.erase(0, key.find_first_not_of(" \t"));
                    value.erase(value.find_last_not_of(" \t") + 1);
                    value.erase(0, value.find_first_not_of(" \t"));
                    
                    if (key == "indent_size") {
                        try {
                            config.indent_size = std::stoi(value);
                        } catch (...) {
                            // Keep default if parsing fails
                        }
                    }
                }
            }
        }
        
        return config;
    }

public:
    void show_help() {
        std::cout << "O²L Code Formatter v0.1.0\n";
        std::cout << "Format O²L source code according to standard conventions\n\n";
        std::cout << "USAGE:\n";
        std::cout << "    o2l-fmt [OPTIONS] [FILES...]\n\n";
        std::cout << "OPTIONS:\n";
        std::cout << "    -w, --write      Write result to (source) file instead of stdout\n";
        std::cout << "    -d, --diff       Display diffs instead of rewriting files\n";
        std::cout << "    -l, --list       List files whose formatting differs from o2l-fmt's\n";
        std::cout << "    -c, --check      Exit with non-zero status if formatting is needed (CI mode)\n";
        std::cout << "    -r, --recursive  Process directories recursively\n";
        std::cout << "    -s, --stdin      Read from stdin (default if no files given)\n";
        std::cout << "    -h, --help       Show this help message\n\n";
        std::cout << "EXAMPLES:\n";
        std::cout << "    o2l-fmt file.obq                # Format file.obq to stdout\n";
        std::cout << "    o2l-fmt -w file.obq             # Format file.obq in place\n";
        std::cout << "    o2l-fmt -r src/                 # Format all .obq files in src/\n";
        std::cout << "    o2l-fmt -d file.obq             # Show diff of changes\n";
        std::cout << "    o2l-fmt -l .                    # List files that need formatting\n";
        std::cout << "    o2l-fmt -c src/                 # Check if formatting needed (CI)\n\n";
        std::cout << "FORMATTING RULES:\n";
        std::cout << "    • 4-space indentation\n";
        std::cout << "    • Spaces around operators (=, +, -, *, /, ==, etc.)\n";
        std::cout << "    • Space after keywords (Object, method, if, etc.)\n";
        std::cout << "    • Consistent brace placement\n";
        std::cout << "    • Proper type annotation spacing (name: Type)\n";
        std::cout << "    • 3 newlines after import blocks\n";
        std::cout << "    • 1 newline between Object methods\n";
        std::cout << "    • 1 newline after Object property blocks\n";
    }
    
    int run(int argc, char* argv[]) {
        bool write_files = false;
        bool show_diff = false;
        bool list_only = false;
        bool check_mode = false;
        bool recursive = false;
        bool use_stdin = false;
        std::vector<std::string> files;
        
        // Parse command line arguments
        for (int i = 1; i < argc; i++) {
            std::string arg = argv[i];
            
            if (arg == "-h" || arg == "--help") {
                show_help();
                return 0;
            } else if (arg == "-w" || arg == "--write") {
                write_files = true;
            } else if (arg == "-d" || arg == "--diff") {
                show_diff = true;
            } else if (arg == "-l" || arg == "--list") {
                list_only = true;
            } else if (arg == "-c" || arg == "--check") {
                check_mode = true;
            } else if (arg == "-r" || arg == "--recursive") {
                recursive = true;
            } else if (arg == "-s" || arg == "--stdin") {
                use_stdin = true;
            } else if (arg.starts_with("-")) {
                std::cerr << "Unknown option: " << arg << std::endl;
                return 1;
            } else {
                files.push_back(arg);
            }
        }
        
        // If no files specified and no stdin flag, show help
        if (files.empty() && !use_stdin) {
            show_help();
            return 0;
        }
        
        // If explicit stdin or no files, read from stdin
        if (use_stdin || files.empty()) {
            return format_stdin();
        }
        
        // Process each file/directory
        int exit_code = 0;
        for (const auto& path : files) {
            if (fs::is_directory(path)) {
                if (recursive) {
                    int dir_exit = process_directory(path, write_files, show_diff, list_only, check_mode);
                    if (dir_exit != 0) exit_code = dir_exit;
                } else {
                    std::cerr << "Skipping directory: " << path << " (use -r for recursive)\n";
                }
            } else {
                int file_exit = process_file(path, write_files, show_diff, list_only, check_mode);
                if (file_exit != 0) exit_code = file_exit;
            }
        }
        
        return exit_code;
    }
    
private:
    int format_stdin() {
        std::string input;
        std::string line;
        
        while (std::getline(std::cin, line)) {
            input += line + "\n";
        }
        
        // Use default 4-space indentation for stdin (no project context)
        O2LFormatter formatter(4);
        std::string formatted = formatter.format_code(input);
        std::cout << formatted;
        
        return 0;
    }
    
    int process_directory(const std::string& dir_path, bool write_files, bool show_diff, bool list_only, bool check_mode) {
        int exit_code = 0;
        for (const auto& entry : fs::recursive_directory_iterator(dir_path)) {
            if (entry.is_regular_file() && entry.path().extension() == ".obq") {
                int file_exit = process_file(entry.path().string(), write_files, show_diff, list_only, check_mode);
                if (file_exit != 0) exit_code = file_exit;
            }
        }
        return exit_code;
    }
    
    int process_file(const std::string& file_path, bool write_files, bool show_diff, bool list_only, bool check_mode) {
        // Check if file has .obq extension
        if (!file_path.ends_with(".obq")) {
            std::cerr << "Skipping non-O²L file: " << file_path << std::endl;
            return 0;
        }
        
        // Load formatting config from project directory
        fs::path current_path = fs::absolute(file_path).parent_path();
        fs::path project_dir = current_path;
        
        // Walk up to find o2l.toml
        while (project_dir != project_dir.root_path()) {
            if (fs::exists(project_dir / "o2l.toml")) {
                break;
            }
            project_dir = project_dir.parent_path();
        }
        
        FormattingConfig config = load_config_from_toml(project_dir.string());
        
        // Read file content
        std::ifstream file(file_path);
        if (!file.is_open()) {
            std::cerr << "Error: Cannot open file " << file_path << std::endl;
            return 1;
        }
        
        std::string content;
        std::string line;
        while (std::getline(file, line)) {
            content += line + "\n";
        }
        file.close();
        
        // Format the code with config
        O2LFormatter formatter(config.indent_size);
        std::string formatted = formatter.format_code(content);
        
        // Check if file needs formatting (handle trailing newlines consistently)
        std::string content_no_trailing = content;
        if (!content_no_trailing.empty() && content_no_trailing.back() == '\n') {
            content_no_trailing.pop_back();
        }
        bool needs_formatting = (content_no_trailing != formatted);
        
        if (list_only) {
            if (needs_formatting) {
                std::cout << file_path << std::endl;
            }
            return 0;
        }
        
        if (show_diff) {
            if (needs_formatting) {
                show_file_diff(file_path, content, formatted);
            }
            return 0;
        }
        
        if (check_mode) {
            if (needs_formatting) {
                std::cerr << "File needs formatting: " << file_path << std::endl;
                return 1;
            }
            return 0;
        }
        
        if (write_files) {
            if (needs_formatting) {
                std::ofstream out_file(file_path);
                if (out_file.is_open()) {
                    out_file << formatted;
                    out_file.close();
                    std::cout << "Formatted: " << file_path << std::endl;
                } else {
                    std::cerr << "Error: Cannot write to file " << file_path << std::endl;
                    return 1;
                }
            }
        } else {
            std::cout << formatted;
        }
        
        return 0;
    }
    
    void show_file_diff(const std::string& file_path, const std::string& original, const std::string& formatted) {
        std::cout << "--- " << file_path << " (original)\n";
        std::cout << "+++ " << file_path << " (formatted)\n";
        
        std::istringstream orig_stream(original);
        std::istringstream fmt_stream(formatted);
        std::vector<std::string> orig_lines, fmt_lines;
        std::string line;
        
        // Read all lines from both streams
        while (std::getline(orig_stream, line)) {
            orig_lines.push_back(line);
        }
        while (std::getline(fmt_stream, line)) {
            fmt_lines.push_back(line);
        }
        
        // Compare line by line
        size_t max_lines = std::max(orig_lines.size(), fmt_lines.size());
        for (size_t i = 0; i < max_lines; i++) {
            std::string orig_line = (i < orig_lines.size()) ? orig_lines[i] : "";
            std::string fmt_line = (i < fmt_lines.size()) ? fmt_lines[i] : "";
            
            if (orig_line != fmt_line) {
                if (i < orig_lines.size()) {
                    std::cout << "-" << (i + 1) << ": " << orig_line << std::endl;
                }
                if (i < fmt_lines.size()) {
                    std::cout << "+" << (i + 1) << ": " << fmt_line << std::endl;
                }
            }
        }
    }
};

int main(int argc, char* argv[]) {
    O2LFmtTool tool;
    return tool.run(argc, argv);
}