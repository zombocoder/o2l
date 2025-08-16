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

class O2LFmtTool {
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
        std::cout << "    -r, --recursive  Process directories recursively\n";
        std::cout << "    -s, --stdin      Read from stdin (default if no files given)\n";
        std::cout << "    -h, --help       Show this help message\n\n";
        std::cout << "EXAMPLES:\n";
        std::cout << "    o2l-fmt file.obq                # Format file.obq to stdout\n";
        std::cout << "    o2l-fmt -w file.obq             # Format file.obq in place\n";
        std::cout << "    o2l-fmt -r src/                 # Format all .obq files in src/\n";
        std::cout << "    o2l-fmt -d file.obq             # Show diff of changes\n";
        std::cout << "    o2l-fmt -l .                    # List files that need formatting\n\n";
        std::cout << "FORMATTING RULES:\n";
        std::cout << "    • 4-space indentation\n";
        std::cout << "    • Spaces around operators (=, +, -, *, /, ==, etc.)\n";
        std::cout << "    • Space after keywords (Object, method, if, etc.)\n";
        std::cout << "    • Consistent brace placement\n";
        std::cout << "    • Proper type annotation spacing (name: Type)\n";
    }
    
    int run(int argc, char* argv[]) {
        bool write_files = false;
        bool show_diff = false;
        bool list_only = false;
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
        for (const auto& path : files) {
            if (fs::is_directory(path)) {
                if (recursive) {
                    process_directory(path, write_files, show_diff, list_only);
                } else {
                    std::cerr << "Skipping directory: " << path << " (use -r for recursive)\n";
                }
            } else {
                process_file(path, write_files, show_diff, list_only);
            }
        }
        
        return 0;
    }
    
private:
    int format_stdin() {
        std::string input;
        std::string line;
        
        while (std::getline(std::cin, line)) {
            input += line + "\n";
        }
        
        O2LFormatter formatter;
        std::string formatted = formatter.format_code(input);
        std::cout << formatted;
        
        return 0;
    }
    
    void process_directory(const std::string& dir_path, bool write_files, bool show_diff, bool list_only) {
        for (const auto& entry : fs::recursive_directory_iterator(dir_path)) {
            if (entry.is_regular_file() && entry.path().extension() == ".obq") {
                process_file(entry.path().string(), write_files, show_diff, list_only);
            }
        }
    }
    
    void process_file(const std::string& file_path, bool write_files, bool show_diff, bool list_only) {
        // Check if file has .obq extension
        if (!file_path.ends_with(".obq")) {
            std::cerr << "Skipping non-O²L file: " << file_path << std::endl;
            return;
        }
        
        // Read file content
        std::ifstream file(file_path);
        if (!file.is_open()) {
            std::cerr << "Error: Cannot open file " << file_path << std::endl;
            return;
        }
        
        std::string content;
        std::string line;
        while (std::getline(file, line)) {
            content += line + "\n";
        }
        file.close();
        
        // Format the code
        O2LFormatter formatter;
        std::string formatted = formatter.format_code(content);
        
        // Check if file needs formatting
        bool needs_formatting = (content != formatted);
        
        if (list_only) {
            if (needs_formatting) {
                std::cout << file_path << std::endl;
            }
            return;
        }
        
        if (show_diff) {
            if (needs_formatting) {
                show_file_diff(file_path, content, formatted);
            }
            return;
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
                }
            }
        } else {
            std::cout << formatted;
        }
    }
    
    void show_file_diff(const std::string& file_path, const std::string& original, const std::string& formatted) {
        std::cout << "--- " << file_path << " (original)\n";
        std::cout << "+++ " << file_path << " (formatted)\n";
        
        std::istringstream orig_stream(original);
        std::istringstream fmt_stream(formatted);
        std::string orig_line, fmt_line;
        int line_num = 1;
        
        while (std::getline(orig_stream, orig_line) || std::getline(fmt_stream, fmt_line)) {
            if (orig_line != fmt_line) {
                if (!orig_line.empty()) {
                    std::cout << "-" << line_num << ": " << orig_line << std::endl;
                }
                if (!fmt_line.empty()) {
                    std::cout << "+" << line_num << ": " << fmt_line << std::endl;
                }
            }
            line_num++;
            orig_line.clear();
            fmt_line.clear();
        }
    }
};

int main(int argc, char* argv[]) {
    O2LFmtTool tool;
    return tool.run(argc, argv);
}