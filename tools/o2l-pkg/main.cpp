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
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <map>
#include <algorithm>
#include "Templates.hpp"

namespace fs = std::filesystem;

class O2LPackageManager {
private:
    fs::path current_dir_;
    fs::path o2l_dir_;
    fs::path config_file_;
    
    struct ProjectConfig {
        std::string name;
        std::string version;
        std::string description;
        std::vector<std::string> authors;
        std::map<std::string, std::string> dependencies;
    };
    
    void createDirectory(const fs::path& path) {
        if (!fs::exists(path)) {
            fs::create_directories(path);
            std::cout << "Created directory: " << path << std::endl;
        }
    }
    
    void writeTomlConfig(const fs::path& file_path, const ProjectConfig& config) {
        std::ofstream file(file_path);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot create config file: " + file_path.string());
        }
        
        std::string content = TemplateEngine::renderTomlConfig(
            config.name, config.version, config.description, 
            config.authors, config.dependencies
        );
        
        file << content;
        file.close();
        std::cout << "Created " << file_path.filename() << std::endl;
    }
    
    std::string promptUser(const std::string& prompt, const std::string& default_value = "") {
        std::cout << prompt;
        if (!default_value.empty()) {
            std::cout << " (" << default_value << ")";
        }
        std::cout << ": ";
        
        std::string input;
        std::getline(std::cin, input);
        
        return input.empty() ? default_value : input;
    }
    
public:
    O2LPackageManager() : current_dir_(fs::current_path()) {
        o2l_dir_ = current_dir_ / ".o2l";
        config_file_ = current_dir_ / "o2l.toml";
    }
    
    void init() {
        std::cout << "Initializing new O²L project...\n" << std::endl;
        
        if (fs::exists(config_file_)) {
            std::cout << "Error: o2l.toml already exists in this directory." << std::endl;
            std::cout << "Use 'o2l-pkg add' to manage dependencies." << std::endl;
            return;
        }
        
        ProjectConfig config;
        
        // Get project name (default to directory name)
        std::string dir_name = current_dir_.filename().string();
        config.name = promptUser("Package name", dir_name);
        config.version = promptUser("Version", "0.1.0");
        config.description = promptUser("Description", "An O²L project");
        
        std::string author = promptUser("Author");
        if (!author.empty()) {
            config.authors.push_back(author);
        }
        
        // Create project structure
        createDirectory(o2l_dir_);
        createDirectory(o2l_dir_ / "lib");
        createDirectory(o2l_dir_ / "cache");
        createDirectory("src");
        createDirectory("src/calc");
        createDirectory("src/greeters");
        createDirectory("src/tests");
        createDirectory("src/tests/calc");
        createDirectory("src/tests/greeters");
        
        // Create Calculator module file in calc subdirectory
        fs::path calc_file = fs::path("src") / "calc" / "Calculator.obq";
        if (!fs::exists(calc_file)) {
            std::ofstream calc_src(calc_file);
            calc_src << TemplateEngine::renderCalculatorFile();
            calc_src.close();
            std::cout << "Created " << calc_file << std::endl;
        }
        
        // Create BasicGreeter module file in greeters subdirectory
        fs::path greeter_file = fs::path("src") / "greeters" / "BasicGreeter.obq";
        if (!fs::exists(greeter_file)) {
            std::ofstream greeter_src(greeter_file);
            greeter_src << TemplateEngine::renderGreeterFile(config.name);
            greeter_src.close();
            std::cout << "Created " << greeter_file << std::endl;
        }
        
        // Create main source file if it doesn't exist
        fs::path main_file = fs::path("src") / "main.obq";
        if (!fs::exists(main_file)) {
            std::ofstream main_src(main_file);
            main_src << TemplateEngine::renderMainFile(config.name);
            main_src.close();
            std::cout << "Created " << main_file << std::endl;
        }
        
        // Create Calculator test file in calc package
        fs::path calc_test_file = fs::path("src/tests/calc") / "CalculatorTest.obq";
        if (!fs::exists(calc_test_file)) {
            std::ofstream calc_test_src(calc_test_file);
            calc_test_src << TemplateEngine::renderCalculatorTestFile(config.name);
            calc_test_src.close();
            std::cout << "Created " << calc_test_file << std::endl;
        }
        
        // Create Greeter test file in greeters package
        fs::path greeter_test_file = fs::path("src/tests/greeters") / "GreeterTest.obq";
        if (!fs::exists(greeter_test_file)) {
            std::ofstream greeter_test_src(greeter_test_file);
            greeter_test_src << TemplateEngine::renderGreeterTestFile(config.name);
            greeter_test_src.close();
            std::cout << "Created " << greeter_test_file << std::endl;
        }
        
        // Create main test runner file
        fs::path test_file = fs::path("src/tests") / "main_test.obq";
        if (!fs::exists(test_file)) {
            std::ofstream test_src(test_file);
            test_src << TemplateEngine::renderMainTestFile(config.name);
            test_src.close();
            std::cout << "Created " << test_file << std::endl;
        }
        
        // Create config file
        writeTomlConfig(config_file_, config);
        
        // Create .gitignore if it doesn't exist
        fs::path gitignore = ".gitignore";
        if (!fs::exists(gitignore)) {
            std::ofstream ignore_file(gitignore);
            ignore_file << TemplateEngine::renderGitignore();
            ignore_file.close();
            std::cout << "Created " << gitignore << std::endl;
        }
        
        std::cout << "\nProject initialized successfully!" << std::endl;
        std::cout << "Next steps:" << std::endl;
        std::cout << "  o2l-pkg add <library>       # Add a dependency" << std::endl;
        std::cout << "  o2l run                     # Run your main program (uses entrypoint)" << std::endl;
        std::cout << "  o2l run src/tests/main_test.obq                   # Run all tests" << std::endl;
        std::cout << "  o2l run src/tests/calc/CalculatorTest.obq         # Run calculator tests only" << std::endl;
        std::cout << "  o2l run src/tests/greeters/GreeterTest.obq        # Run greeter tests only" << std::endl;
    }
    
    void add(const std::string& library_name, const std::string& version = "latest") {
        if (!fs::exists(config_file_)) {
            std::cout << "Error: No o2l.toml found. Run 'o2l-pkg init' first." << std::endl;
            return;
        }
        
        std::cout << "Adding library: " << library_name;
        if (version != "latest") {
            std::cout << " version " << version;
        }
        std::cout << std::endl;
        
        // Create library directory structure
        fs::path lib_dir = o2l_dir_ / "lib" / library_name;
        createDirectory(lib_dir);
        
        // For namespace-aware libraries, check if this is a namespaced library
        bool is_namespaced = library_name.find('.') != std::string::npos;
        
        if (is_namespaced) {
            // Handle namespaced library (e.g., "com.example.utils")
            addNamespacedLibrary(library_name, version, lib_dir);
        } else {
            // Handle simple library
            addSimpleLibrary(library_name, version, lib_dir);
        }
        
        std::cout << "Library " << library_name << " added to .o2l/lib/" << std::endl;
        std::cout << "Use: import " << library_name << " to use this library" << std::endl;
    }
    
    void addSimpleLibrary(const std::string& library_name, const std::string& version, const fs::path& lib_dir) {
        fs::path lib_file = lib_dir / (library_name + ".obq");
        if (!fs::exists(lib_file)) {
            std::ofstream lib_src(lib_file);
            lib_src << TemplateEngine::renderLibraryFile(library_name, version);
            lib_src.close();
        }
    }
    
    void addNamespacedLibrary(const std::string& library_name, const std::string& version, const fs::path& lib_dir) {
        // Parse namespace parts (e.g., "com.example.utils" -> ["com", "example", "utils"])
        std::vector<std::string> parts;
        std::string current_part;
        for (char c : library_name) {
            if (c == '.') {
                if (!current_part.empty()) {
                    parts.push_back(current_part);
                    current_part.clear();
                }
            } else {
                current_part += c;
            }
        }
        if (!current_part.empty()) {
            parts.push_back(current_part);
        }
        
        if (parts.empty()) return;
        
        // Create nested directory structure for namespace
        fs::path namespace_dir = lib_dir;
        for (size_t i = 0; i < parts.size() - 1; ++i) {
            namespace_dir /= parts[i];
            createDirectory(namespace_dir);
        }
        
        // Create the main library file with namespace structure
        std::string class_name = parts.back();
        fs::path lib_file = namespace_dir / (class_name + ".obq");
        
        if (!fs::exists(lib_file)) {
            std::ofstream lib_src(lib_file);
            lib_src << TemplateEngine::renderNamespacedLibraryFile(library_name, class_name, version);
            lib_src.close();
        }
        
        // Create an index file at the root for easy importing
        fs::path index_file = lib_dir / (library_name + ".obq");
        if (!fs::exists(index_file)) {
            std::ofstream index_src(index_file);
            index_src << TemplateEngine::renderLibraryIndex(library_name, parts);
            index_src.close();
        }
    }
    
    void list() {
        if (!fs::exists(o2l_dir_)) {
            std::cout << "No .o2l directory found. Run 'o2l-pkg init' first." << std::endl;
            return;
        }
        
        fs::path lib_dir = o2l_dir_ / "lib";
        if (!fs::exists(lib_dir)) {
            std::cout << "No libraries installed." << std::endl;
            return;
        }
        
        std::cout << "Installed libraries:" << std::endl;
        for (const auto& entry : fs::directory_iterator(lib_dir)) {
            if (entry.is_directory()) {
                std::cout << "  " << entry.path().filename().string() << std::endl;
            }
        }
    }
    
    void clean() {
        fs::path cache_dir = o2l_dir_ / "cache";
        if (fs::exists(cache_dir)) {
            fs::remove_all(cache_dir);
            createDirectory(cache_dir);
            std::cout << "Cleaned cache directory." << std::endl;
        } else {
            std::cout << "No cache to clean." << std::endl;
        }
    }
    
    void create(const std::string& path, const std::string& object_name) {
        if (!fs::exists(config_file_)) {
            std::cout << "Error: No o2l.toml found. Run 'o2l-pkg init' first." << std::endl;
            return;
        }
        
        std::cout << "Creating object: " << object_name << " at " << path << std::endl;
        
        // Parse path parts
        std::vector<std::string> parts;
        std::string current_part;
        for (char c : path) {
            if (c == '.') {
                if (!current_part.empty()) {
                    parts.push_back(current_part);
                    current_part.clear();
                }
            } else {
                current_part += c;
            }
        }
        if (!current_part.empty()) {
            parts.push_back(current_part);
        }
        
        if (parts.empty()) {
            std::cout << "Error: Invalid path" << std::endl;
            return;
        }
        
        // Create directory structure in src/
        fs::path class_dir = "src";
        for (const auto& part : parts) {
            class_dir /= part;
            createDirectory(class_dir);
        }
        
        // Create the class file
        fs::path class_file = class_dir / (object_name + ".obq");
        if (!fs::exists(class_file)) {
            std::ofstream class_src(class_file);
            class_src << TemplateEngine::renderNamespaceClass(path, object_name);
            class_src.close();
            std::cout << "Created " << class_file << std::endl;
        }
        
        std::cout << "Object created successfully!" << std::endl;
        std::cout << "You can import with: @import src." << path << "." << object_name << std::endl;
    }
    
    void showHelp() {
        std::cout << "O²L Package Manager v0.1.0" << std::endl;
        std::cout << "Enhanced with namespace support" << std::endl;
        std::cout << std::endl;
        std::cout << "USAGE:" << std::endl;
        std::cout << "    o2l-pkg <COMMAND> [OPTIONS]" << std::endl;
        std::cout << std::endl;
        std::cout << "COMMANDS:" << std::endl;
        std::cout << "    init                     Initialize a new O²L project" << std::endl;
        std::cout << "    add <library>            Add a library dependency" << std::endl;
        std::cout << "    create <path> <name>     Create object in directory structure" << std::endl;
        std::cout << "    list                     List installed libraries" << std::endl;
        std::cout << "    clean                    Clean cache directory" << std::endl;
        std::cout << "    help                     Show this help message" << std::endl;
        std::cout << std::endl;
        std::cout << "EXAMPLES:" << std::endl;
        std::cout << "    o2l-pkg init                             # Initialize new project" << std::endl;
        std::cout << "    o2l-pkg add collections                  # Add simple library" << std::endl;
        std::cout << "    o2l-pkg add com.example.math 1.2.0       # Add namespaced library" << std::endl;
        std::cout << "    o2l-pkg create com.mycompany.utils Utils    # Create object structure" << std::endl;
        std::cout << "    o2l-pkg list                             # List dependencies" << std::endl;
        std::cout << std::endl;
        std::cout << "DIRECTORY STRUCTURE:" << std::endl;
        std::cout << "    • Use 'create' to generate objects in organized directories" << std::endl;
        std::cout << "    • Import with: @import src.com.mycompany.utils.Utils" << std::endl;
        std::cout << "    • Use with: utils: Utils = new Utils()" << std::endl;
    }
};

int main(int argc, char* argv[]) {
    O2LPackageManager pkg_manager;
    
    if (argc < 2) {
        pkg_manager.showHelp();
        return 1;
    }
    
    std::string command = argv[1];
    std::transform(command.begin(), command.end(), command.begin(), ::tolower);
    
    try {
        if (command == "init") {
            pkg_manager.init();
        } else if (command == "add") {
            if (argc < 3) {
                std::cout << "Error: Library name required for 'add' command" << std::endl;
                std::cout << "Usage: o2l-pkg add <library> [version]" << std::endl;
                return 1;
            }
            std::string library = argv[2];
            std::string version = (argc >= 4) ? argv[3] : "latest";
            pkg_manager.add(library, version);
        } else if (command == "create") {
            if (argc < 4) {
                std::cout << "Error: Path and object name required for 'create' command" << std::endl;
                std::cout << "Usage: o2l-pkg create <path> <object_name>" << std::endl;
                std::cout << "Example: o2l-pkg create com.mycompany.utils Utils" << std::endl;
                return 1;
            }
            std::string path = argv[2];
            std::string object_name = argv[3];
            pkg_manager.create(path, object_name);
        } else if (command == "list") {
            pkg_manager.list();
        } else if (command == "clean") {
            pkg_manager.clean();
        } else if (command == "help" || command == "--help" || command == "-h") {
            pkg_manager.showHelp();
        } else {
            std::cout << "Error: Unknown command '" << command << "'" << std::endl;
            std::cout << "Run 'o2l-pkg help' for available commands." << std::endl;
            return 1;
        }
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}