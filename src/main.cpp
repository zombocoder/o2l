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

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "AST/JsonSerializer.hpp"
#include "Common/Exceptions.hpp"
#include "Interpreter.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Runtime/Value.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "O²L Programming Language Interpreter v0.0.1\n\n";
        std::cout << "Usage:\n";
        std::cout << "  o2l run [file.obq]       Run an O²L program (uses o2l.toml entrypoint "
                     "if no file)\n";
        std::cout << "  o2l parse <file.obq>     Parse file and output AST\n";
        std::cout << "  o2l repl                 Start interactive REPL\n";
        std::cout << "  o2l --help               Show this help message\n";
        std::cout << "  o2l --version            Show version information\n";
        return 0;
    }

    std::string command = argv[1];

    if (command == "--help") {
        std::cout << "O²L Programming Language Interpreter\n\n";
        std::cout << "A fully object-oriented programming language with immutable objects.\n";
        std::cout << "Everything is an object - no primitives, no null, no static methods.\n\n";
        std::cout << "Commands:\n";
        std::cout << "  run [file]     Execute an O²L source file (.obq) or use o2l.toml "
                     "entrypoint\n";
        std::cout << "  parse <file>   Parse file and output AST (for LSP/tooling)\n";
        std::cout << "  repl           Start interactive Read-Eval-Print Loop\n";
        std::cout << "  --debug        Enable debug output (use with run command)\n";
        std::cout << "  --json-output  Output in JSON format (use with parse command)\n";
        std::cout << "  --help         Show this help message\n";
        std::cout << "  --version      Show version information\n";
        return 0;
    }

    if (command == "--version") {
        std::cout << "O²L v0.0.1\n";
        std::cout << "Built with C++23\n";
        std::cout << "Apache License 2.0\n";
        return 0;
    }

    // Handle --parse-only flag for LSP compatibility (alternative to 'parse' command)
    if (command == "--parse-only") {
        if (argc < 3) {
            std::cerr << "Error: No input file specified\n";
            std::cerr << "Usage: o2l --parse-only <file.obq> [--json-output]\n";
            return 1;
        }

        // Redirect to parse command logic
        std::vector<char*> new_argv;
        new_argv.push_back(argv[0]);                     // program name
        new_argv.push_back(const_cast<char*>("parse"));  // change to parse command

        // Copy remaining arguments
        for (int i = 2; i < argc; ++i) {
            new_argv.push_back(argv[i]);
        }

        // Update argc to match new_argv size
        int new_argc = new_argv.size();

        // Set up for parse command processing
        std::string filename = new_argv[2];
        bool json_output = false;

        // Check for json output flag
        for (int i = 3; i < new_argc; ++i) {
            if (std::string(new_argv[i]) == "--json-output") {
                json_output = true;
            }
        }

        // Execute parse logic (same as parse command)
        // ... [parse logic here - will be moved to a function]

        // For now, just redirect to show both syntaxes work
        std::cerr << "Note: --parse-only is equivalent to 'parse'. Use: o2l parse " << filename;
        if (json_output) std::cerr << " --json-output";
        std::cerr << "\n";
        return 0;
    }

    if (command == "run") {
        std::string filename;

        if (argc < 3) {
            // No file specified, check for o2l.toml
            if (std::filesystem::exists("o2l.toml")) {
                // Parse o2l.toml to find entrypoint
                std::ifstream toml_file("o2l.toml");
                if (toml_file.is_open()) {
                    std::string line;
                    while (std::getline(toml_file, line)) {
                        // Simple TOML parsing for entrypoint field
                        if (line.find("entrypoint") != std::string::npos &&
                            line.find("=") != std::string::npos) {
                            size_t eq_pos = line.find("=");
                            if (eq_pos != std::string::npos) {
                                std::string value = line.substr(eq_pos + 1);
                                // Remove quotes and whitespace
                                size_t start = value.find_first_not_of(" \t\"");
                                size_t end = value.find_last_not_of(" \t\"");
                                if (start != std::string::npos && end != std::string::npos) {
                                    filename = value.substr(start, end - start + 1);
                                    break;
                                }
                            }
                        }
                    }
                    toml_file.close();
                }

                if (filename.empty()) {
                    std::cerr << "Error: Found o2l.toml but no entrypoint specified\n";
                    std::cerr << "Add 'entrypoint = \"src/main.obq\"' to your o2l.toml\n";
                    return 1;
                }

            } else {
                std::cerr << "Error: No input file specified and no o2l.toml found\n";
                std::cerr << "Usage: o2l run <file.obq>\n";
                std::cerr << "   or: o2l run  (uses entrypoint from o2l.toml)\n";
                return 1;
            }
        } else {
            filename = argv[2];
        }
        bool debug_mode = false;
        std::vector<std::string> program_args;

        // Add the program name (filename) as argv[0] - following C/C++ convention
        program_args.push_back(filename);

        // Check for debug flag and collect program arguments
        // If filename came from o2l.toml, start from index 2, otherwise start from index 3
        int start_idx = (argc >= 3) ? 3 : 2;

        for (int i = start_idx; i < argc; ++i) {
            if (std::string(argv[i]) == "--debug") {
                debug_mode = true;
            } else {
                // All other arguments are passed to the program
                program_args.push_back(std::string(argv[i]));
            }
        }

        // Verify file exists and has .obq extension
        if (!std::filesystem::exists(filename)) {
            std::cerr << "Error: File '" << filename << "' not found\n";
            return 1;
        }

        if (!filename.ends_with(".obq")) {
            std::cerr << "Warning: File '" << filename << "' does not have .obq extension\n";
        }

        // Read the file
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Cannot open file '" << filename << "'\n";
            return 1;
        }

        std::string source_code((std::istreambuf_iterator<char>(file)),
                                std::istreambuf_iterator<char>());
        file.close();

        if (debug_mode) {
            std::cout << "[DEBUG] Running file: " << filename << "\n";
            std::cout << "[DEBUG] Source code length: " << source_code.length() << " characters\n";
        }

        // Initialize lexer, parser, and interpreter
        try {
            if (debug_mode) {
                std::cout << "[DEBUG] Starting lexical analysis...\n";
            }

            // Lexical analysis
            o2l::Lexer lexer(source_code);
            auto tokens = lexer.tokenizeAll();

            if (debug_mode) {
                std::cout << "[DEBUG] Tokenized " << tokens.size() << " tokens\n";
                for (const auto& token : tokens) {
                    std::cout << "[DEBUG] Token: " << static_cast<int>(token.type) << " '"
                              << token.value << "' at " << token.line << ":" << token.column
                              << "\n";
                }
                std::cout << "[DEBUG] Starting parsing...\n";
            }

            // Parsing
            o2l::Parser parser(std::move(tokens), filename);
            auto ast_nodes = parser.parse();

            if (debug_mode) {
                std::cout << "[DEBUG] Parsed " << ast_nodes.size() << " AST nodes\n";
                for (const auto& node : ast_nodes) {
                    std::cout << "[DEBUG] AST Node: " << node->toString() << "\n";
                }
                std::cout << "[DEBUG] Starting interpretation...\n";
            }

            // Interpretation
            o2l::Interpreter interpreter(filename);

            // Set program arguments so they can be accessed via system.os.args
            interpreter.setProgramArguments(program_args);

            // Add the source file's directory to module search paths for relative imports
            std::filesystem::path source_dir = std::filesystem::path(filename).parent_path();
            if (!source_dir.empty()) {
                interpreter.getModuleLoader().addSearchPath(source_dir);
            }

            o2l::Value result = interpreter.execute(ast_nodes);

            // Check if main() returned an Int to use as exit code
            int exit_code = 0;
            if (std::holds_alternative<o2l::Int>(result)) {
                exit_code = std::get<o2l::Int>(result);

                if (debug_mode) {
                    std::cout << "[DEBUG] Execution completed\n";
                    std::cout << "[DEBUG] Main returned exit code: " << exit_code << "\n";
                }
                // Don't print exit code in normal mode - it's hidden
            } else {
                if (debug_mode) {
                    std::cout << "[DEBUG] Execution completed\n";
                    std::cout << "[DEBUG] Result: " << o2l::valueToString(result) << "\n";
                } else {
                    // Print result for non-debug mode (only if not Int exit code)
                    std::cout << o2l::valueToString(result) << "\n";
                }
            }

            return exit_code;

        } catch (const o2l::o2lException& e) {
            std::cerr << "Error: " << e.getMessage() << "\n";

            auto stack_trace = e.getStackTrace();
            if (!stack_trace.empty()) {
                std::cerr << "Stack trace:\n";
                for (const auto& frame : stack_trace) {
                    std::cerr << "  " << frame << "\n";
                }
            }

            return 1;
        } catch (const std::exception& e) {
            std::cerr << "Unexpected error: " << e.what() << "\n";
            return 1;
        }
    }

    if (command == "parse") {
        if (argc < 3) {
            std::cerr << "Error: No input file specified\n";
            std::cerr << "Usage: o2l parse <file.obq> [--json-output]\n";
            return 1;
        }

        std::string filename = argv[2];
        bool json_output = false;

        // Check for json output flag
        for (int i = 3; i < argc; ++i) {
            if (std::string(argv[i]) == "--json-output") {
                json_output = true;
            }
        }

        // Verify file exists and has .obq extension
        if (!std::filesystem::exists(filename)) {
            if (json_output) {
                std::cout << o2l::JsonSerializer::serializeError("File not found: " + filename,
                                                                 filename)
                          << std::endl;
            } else {
                std::cerr << "Error: File '" << filename << "' not found\n";
            }
            return 1;
        }

        if (!filename.ends_with(".obq")) {
            if (!json_output) {
                std::cerr << "Warning: File '" << filename << "' does not have .obq extension\n";
            }
        }

        // Read the file
        std::ifstream file(filename);
        if (!file.is_open()) {
            if (json_output) {
                std::cout << o2l::JsonSerializer::serializeError("Cannot open file: " + filename,
                                                                 filename)
                          << std::endl;
            } else {
                std::cerr << "Error: Cannot open file '" << filename << "'\n";
            }
            return 1;
        }

        std::string source_code((std::istreambuf_iterator<char>(file)),
                                std::istreambuf_iterator<char>());
        file.close();

        // Parse the file (lexing + parsing only, no execution)
        try {
            // Lexical analysis
            o2l::Lexer lexer(source_code);
            auto tokens = lexer.tokenizeAll();

            // Parsing
            o2l::Parser parser(std::move(tokens), filename);
            auto ast_nodes = parser.parse();

            // Output AST in requested format
            if (json_output) {
                std::cout << o2l::JsonSerializer::serializeAST(ast_nodes, true) << std::endl;
            } else {
                std::cout << "Successfully parsed " << ast_nodes.size() << " AST nodes:\n";
                for (size_t i = 0; i < ast_nodes.size(); ++i) {
                    std::cout << "[" << i << "] " << ast_nodes[i]->toString() << "\n";
                }
            }

            return 0;

        } catch (const o2l::o2lException& e) {
            if (json_output) {
                std::cout << o2l::JsonSerializer::serializeError(e.getMessage(), filename)
                          << std::endl;
            } else {
                std::cerr << "Parse Error: " << e.getMessage() << "\n";

                auto stack_trace = e.getStackTrace();
                if (!stack_trace.empty()) {
                    std::cerr << "Location:\n";
                    for (const auto& frame : stack_trace) {
                        std::cerr << "  " << frame << "\n";
                    }
                }
            }
            return 1;
        } catch (const std::exception& e) {
            if (json_output) {
                std::cout << o2l::JsonSerializer::serializeError(
                                 std::string("Unexpected error: ") + e.what(), filename)
                          << std::endl;
            } else {
                std::cerr << "Unexpected error: " << e.what() << "\n";
            }
            return 1;
        }
    }

    if (command == "repl") {
        std::cout << "O²L REPL v0.0.1\n";
        std::cout << "Type 'exit' to quit\n\n";

        std::string input;
        while (true) {
            std::cout << "o2l> ";
            std::getline(std::cin, input);

            if (input == "exit" || input == "quit") {
                std::cout << "Goodbye!\n";
                break;
            }

            if (input.empty()) {
                continue;
            }

            // TODO: Implement REPL evaluation
            std::cout << "Error: REPL evaluation not yet implemented\n";
        }

        return 0;
    }

    std::cerr << "Error: Unknown command '" << command << "'\n";
    std::cerr << "Use 'o2l --help' for usage information\n";
    return 1;
}