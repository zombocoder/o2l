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

#include <sys/wait.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "json.hpp"

using json = nlohmann::json;

class O2LLSPServer {
   private:
    std::string o2l_binary;
    bool initialized = false;
    json client_capabilities;

    std::string find_o2l_binary() {
        return "o2l";
    }

    void log(const std::string& message) {
        std::cerr << "[O2L-LSP] " << message << std::endl;
    }

    void send_response(int id, const json& result, const json& error = json()) {
        json response = {{"jsonrpc", "2.0"}, {"id", id}};

        if (!error.is_null()) {
            response["error"] = error;
        } else {
            response["result"] = result;
        }

        send_message(response);
    }

    void send_notification(const std::string& method, const json& params = nullptr) {
        json notification = {{"jsonrpc", "2.0"}, {"method", method}};

        if (!params.is_null()) {
            notification["params"] = params;
        }

        send_message(notification);
    }

    void send_message(const json& message) {
        std::string content = message.dump();
        std::string headers = "Content-Length: " + std::to_string(content.length()) + "\r\n\r\n";

        std::cout << headers << content << std::flush;
    }

    json parse_o2l_file(const std::string& file_path) {
        // Execute o2l parse command
        std::string command = o2l_binary + " parse \"" + file_path + "\" --json-output 2>&1";

        FILE* pipe = popen(command.c_str(), "r");
        if (!pipe) {
            return {{"success", false}, {"error", "Failed to execute o2l parser"}};
        }

        std::string result;
        char buffer[128];
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            result += buffer;
        }

        int exit_code = pclose(pipe);

        if (exit_code == 0) {
            try {
                json ast_data = json::parse(result);
                return {{"success", true}, {"ast", ast_data}};
            } catch (const json::parse_error& e) {
                return {{"success", false},
                        {"error", "JSON parse error: " + std::string(e.what())}};
            }
        } else {
            return {{"success", false}, {"error", result}};
        }
    }

    json extract_diagnostics_from_ast(const json& ast_result) {
        json diagnostics = json::array();

        if (!ast_result.value("success", false)) {
            std::string error_msg = ast_result.value("error", "Unknown parse error");

            json diagnostic = {{"range",
                                {{"start", {{"line", 0}, {"character", 0}}},
                                 {"end", {{"line", 0}, {"character", 0}}}}},
                               {"severity", 1},  // Error
                               {"message", error_msg},
                               {"source", "o2l"}};

            diagnostics.push_back(diagnostic);
        }

        return diagnostics;
    }

    json handle_initialize(const json& params) {
        client_capabilities = params.value("capabilities", json::object());

        // Create trigger characters arrays
        json completion_triggers = json::array();
        completion_triggers.push_back(".");
        completion_triggers.push_back(":");

        json signature_triggers = json::array();
        signature_triggers.push_back("(");
        signature_triggers.push_back(",");

        json server_capabilities = {
            {"textDocumentSync", 1},  // Full sync
            {"diagnosticProvider", true},
            {"documentSymbolProvider", true},
            {"hoverProvider", true},
            {"completionProvider",
             {{"triggerCharacters", completion_triggers}, {"resolveProvider", false}}},
            {"definitionProvider", true},
            {"referencesProvider", true},
            {"signatureHelpProvider", {{"triggerCharacters", signature_triggers}}}};

        return {{"capabilities", server_capabilities},
                {"serverInfo", {{"name", "o2l-lsp"}, {"version", "1.0.0"}}}};
    }

    void handle_initialized(const json& params) {
        initialized = true;
        log("Received initialized notification - LSP handshake complete");
    }

    void handle_text_document_did_open(const json& params) {
        json text_document = params.value("textDocument", json::object());
        std::string uri = text_document.value("uri", "");

        // Convert URI to file path
        std::string file_path = uri;
        if (file_path.substr(0, 7) == "file://") {
            file_path = file_path.substr(7);
        }

        // Parse the file and send diagnostics
        send_diagnostics(uri, file_path);
    }

    void handle_text_document_did_change(const json& params) {
        json text_document = params.value("textDocument", json::object());
        std::string uri = text_document.value("uri", "");

        // Convert URI to file path
        std::string file_path = uri;
        if (file_path.substr(0, 7) == "file://") {
            file_path = file_path.substr(7);
        }

        // Parse the file and send diagnostics
        send_diagnostics(uri, file_path);
    }

    void send_diagnostics(const std::string& uri, const std::string& file_path) {
        json ast_result = parse_o2l_file(file_path);
        json diagnostics = extract_diagnostics_from_ast(ast_result);

        json params = {{"uri", uri}, {"diagnostics", diagnostics}};

        send_notification("textDocument/publishDiagnostics", params);
    }

    // Extract symbols from AST for completion and navigation
    std::vector<std::pair<std::string, json>> extract_symbols_from_ast(const json& ast) {
        std::vector<std::pair<std::string, json>> symbols;

        if (!ast.contains("ast")) {
            return symbols;
        }

        // Recursively traverse AST to find symbols
        traverse_ast_for_symbols(ast["ast"], symbols);

        return symbols;
    }

    void traverse_ast_for_symbols(const json& node,
                                  std::vector<std::pair<std::string, json>>& symbols) {
        if (!node.contains("node_type")) {
            return;
        }

        std::string node_type = node.value("node_type", "");

        // Extract different types of symbols
        if (node_type == "ObjectDeclarationNode") {
            std::string object_name = node.value("name", "");
            if (!object_name.empty()) {
                json symbol = {{"name", object_name},
                               {"kind", 5},  // Class
                               {"location", extract_location(node)}};
                symbols.push_back({object_name, symbol});

                // Extract methods from object
                if (node.contains("methods")) {
                    for (const auto& method : node["methods"]) {
                        std::string method_name = method.value("name", "");
                        if (!method_name.empty()) {
                            json method_symbol = {{"name", method_name},
                                                  {"kind", 6},  // Method
                                                  {"location", extract_location(method)},
                                                  {"containerName", object_name}};
                            symbols.push_back({method_name, method_symbol});
                        }
                    }
                }

                // Extract properties from object
                if (node.contains("properties")) {
                    for (const auto& property : node["properties"]) {
                        std::string property_name = property.value("name", "");
                        if (!property_name.empty()) {
                            json property_symbol = {{"name", property_name},
                                                    {"kind", 7},  // Property
                                                    {"location", extract_location(property)},
                                                    {"containerName", object_name}};
                            symbols.push_back({property_name, property_symbol});
                        }
                    }
                }
            }
        } else if (node_type == "MethodDeclarationNode") {
            std::string method_name = node.value("name", "");
            if (!method_name.empty()) {
                json symbol = {{"name", method_name},
                               {"kind", 6},  // Method
                               {"location", extract_location(node)}};
                symbols.push_back({method_name, symbol});
            }
        } else if (node_type == "VariableDeclarationNode") {
            std::string var_name = node.value("variable_name", "");
            if (!var_name.empty()) {
                json symbol = {{"name", var_name},
                               {"kind", 13},  // Variable
                               {"location", extract_location(node)}};
                symbols.push_back({var_name, symbol});
            }
        }

        // Recursively traverse child nodes
        for (auto& [key, value] : node.items()) {
            if (value.is_array()) {
                for (const auto& item : value) {
                    if (item.is_object()) {
                        traverse_ast_for_symbols(item, symbols);
                    }
                }
            } else if (value.is_object()) {
                traverse_ast_for_symbols(value, symbols);
            }
        }
    }

    json extract_location(const json& node) {
        json location = {{"uri", ""},
                         {"range",
                          {{"start", {{"line", 0}, {"character", 0}}},
                           {"end", {{"line", 0}, {"character", 0}}}}}};

        if (node.contains("source_location")) {
            const json& src_loc = node["source_location"];
            int line = src_loc.value("line", 0);
            int column = src_loc.value("column", 0);

            location["range"]["start"]["line"] = line;
            location["range"]["start"]["character"] = column;
            location["range"]["end"]["line"] = line;
            location["range"]["end"]["character"] = column + 10;  // Approximate width
        }

        return location;
    }

    // Cache for parsed files
    std::map<std::string, json> file_ast_cache;
    std::map<std::string, std::vector<std::pair<std::string, json>>> file_symbols_cache;

    json get_file_ast(const std::string& file_path) {
        // Check cache first
        auto it = file_ast_cache.find(file_path);
        if (it != file_ast_cache.end()) {
            return it->second;
        }

        // Parse file and cache result
        json result = parse_o2l_file(file_path);
        file_ast_cache[file_path] = result;

        // Extract and cache symbols
        if (result.value("success", false)) {
            auto symbols = extract_symbols_from_ast(result);
            file_symbols_cache[file_path] = symbols;
        }

        return result;
    }

    json handle_completion(const json& params) {
        json text_document = params.value("textDocument", json::object());
        std::string uri = text_document.value("uri", "");
        json position = params.value("position", json::object());
        int line = position.value("line", 0);
        int character = position.value("character", 0);

        // Convert URI to file path
        std::string file_path = uri;
        if (file_path.substr(0, 7) == "file://") {
            file_path = file_path.substr(7);
        }

        // Get symbols from current file
        json ast = get_file_ast(file_path);
        auto symbols = file_symbols_cache[file_path];

        // Build completion items
        json completion_items = json::array();

        // Add built-in types
        std::vector<std::string> builtin_types = {"Text", "Int",    "Float",    "Bool",
                                                  "Char", "Double", "Long",     "List",
                                                  "Map",  "Set",    "Optional", "Result"};

        for (const auto& type : builtin_types) {
            json item = {{"label", type},
                         {"kind", 7},  // Class
                         {"detail", "Built-in type"},
                         {"insertText", type}};
            completion_items.push_back(item);
        }

        // Add keywords
        std::vector<std::string> keywords = {
            "Object", "method",   "property", "constructor", "import", "this",
            "return", "if",       "else",     "while",       "for",    "enum",
            "record", "protocol", "const",    "true",        "false",  "null"};

        for (const auto& keyword : keywords) {
            json item = {{"label", keyword},
                         {"kind", 14},  // Keyword
                         {"detail", "O²L keyword"},
                         {"insertText", keyword}};
            completion_items.push_back(item);
        }

        // Add symbols from current file
        for (const auto& symbol_pair : symbols) {
            json item = {{"label", symbol_pair.first},
                         {"kind", symbol_pair.second.value("kind", 13)},
                         {"detail", symbol_pair.second.value("containerName", "")},
                         {"insertText", symbol_pair.first}};
            completion_items.push_back(item);
        }

        return {{"isIncomplete", false}, {"items", completion_items}};
    }

    json handle_definition(const json& params) {
        json text_document = params.value("textDocument", json::object());
        std::string uri = text_document.value("uri", "");
        json position = params.value("position", json::object());
        int line = position.value("line", 0);
        int character = position.value("character", 0);

        // Convert URI to file path
        std::string file_path = uri;
        if (file_path.substr(0, 7) == "file://") {
            file_path = file_path.substr(7);
        }

        // Get symbols from current file
        auto symbols = file_symbols_cache[file_path];

        // For now, return the first matching symbol
        // In a real implementation, we'd need to parse the current position
        // to find what identifier is being referenced

        json locations = json::array();

        // This is a simplified implementation
        // In practice, you'd need to:
        // 1. Parse the text at the cursor position to get the identifier
        // 2. Find the definition of that identifier in the symbol table
        // 3. Return the location of the definition

        if (!symbols.empty()) {
            json location = symbols[0].second["location"];
            location["uri"] = uri;
            locations.push_back(location);
        }

        return locations;
    }

    json handle_references(const json& params) {
        json text_document = params.value("textDocument", json::object());
        std::string uri = text_document.value("uri", "");
        json position = params.value("position", json::object());

        // Simplified implementation - return empty for now
        return json::array();
    }

    json handle_hover(const json& params) {
        json text_document = params.value("textDocument", json::object());
        std::string uri = text_document.value("uri", "");
        json position = params.value("position", json::object());

        // Simplified implementation
        return {{"contents",
                 {{"kind", "markdown"}, {"value", "O²L hover information (to be implemented)"}}}};
    }

    json handle_document_symbol(const json& params) {
        json text_document = params.value("textDocument", json::object());
        std::string uri = text_document.value("uri", "");

        // Convert URI to file path
        std::string file_path = uri;
        if (file_path.substr(0, 7) == "file://") {
            file_path = file_path.substr(7);
        }

        // Get symbols from current file
        auto symbols = file_symbols_cache[file_path];

        json document_symbols = json::array();

        for (const auto& symbol_pair : symbols) {
            json symbol = symbol_pair.second;
            symbol["location"]["uri"] = uri;  // Ensure URI is set
            document_symbols.push_back(symbol);
        }

        return document_symbols;
    }

    json handle_diagnostic(const json& params) {
        json text_document = params.value("textDocument", json::object());
        std::string uri = text_document.value("uri", "");

        // Convert URI to file path
        std::string file_path = uri;
        if (file_path.substr(0, 7) == "file://") {
            file_path = file_path.substr(7);
        }

        log("Running diagnostics for: " + file_path);

        // Parse the file and generate diagnostics
        json ast_result = get_file_ast(file_path);
        json diagnostics = extract_diagnostics_from_ast(ast_result);

        // Return diagnostic response in the expected format
        json result = {{"kind", "full"}, {"items", diagnostics}};

        return result;
    }

    void handle_request(const json& message) {
        std::string method = message.value("method", "");
        json params = message.value("params", json::object());
        int id = message.value("id", 0);

        log("Received request: '" + method + "' (id: " + std::to_string(id) + ")");

        if (method == "initialize") {
            json result = handle_initialize(params);
            log("Sending initialize response");
            send_response(id, result);
        } else if (method == "shutdown") {
            send_response(id, json());
        } else if (method == "exit") {
            exit(0);
        } else if (method == "textDocument/completion") {
            json result = handle_completion(params);
            send_response(id, result);
        } else if (method == "textDocument/definition") {
            json result = handle_definition(params);
            send_response(id, result);
        } else if (method == "textDocument/references") {
            json result = handle_references(params);
            send_response(id, result);
        } else if (method == "textDocument/hover") {
            json result = handle_hover(params);
            send_response(id, result);
        } else if (method == "textDocument/documentSymbol") {
            json result = handle_document_symbol(params);
            send_response(id, result);
        } else if (method == "textDocument/diagnostic") {
            json result = handle_diagnostic(params);
            send_response(id, result);
        } else {
            // Method not supported
            log("Unsupported method: " + method);
            json error = {{"code", -32601}, {"message", "Method not found: " + method}};
            send_response(id, json(), error);
        }
    }

    void handle_notification(const json& message) {
        std::string method = message.value("method", "");
        json params = message.value("params", json::object());

        log("Received notification: '" + method + "'");

        if (method == "initialized") {
            handle_initialized(params);
        } else if (method == "textDocument/didOpen") {
            handle_text_document_did_open(params);
        } else if (method == "textDocument/didChange") {
            handle_text_document_did_change(params);
        }
        // Ignore other notifications
    }

    json read_message() {
        // Read headers
        std::map<std::string, std::string> headers;
        std::string line;

        while (std::getline(std::cin, line)) {
            // Remove \r if present
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }

            if (line.empty()) {
                break;
            }

            size_t colon_pos = line.find(':');
            if (colon_pos != std::string::npos) {
                std::string key = line.substr(0, colon_pos);
                std::string value = line.substr(colon_pos + 1);

                // Trim whitespace
                key.erase(0, key.find_first_not_of(" \t"));
                key.erase(key.find_last_not_of(" \t") + 1);
                value.erase(0, value.find_first_not_of(" \t"));
                value.erase(value.find_last_not_of(" \t") + 1);

                headers[key] = value;
            }
        }

        // Read content
        int content_length = 0;
        if (headers.find("Content-Length") != headers.end()) {
            content_length = std::stoi(headers["Content-Length"]);
        }

        if (content_length == 0) {
            log("No content length found in headers");
            return json();
        }

        if (content_length > 1000000) {  // 1MB limit
            log("Content length too large: " + std::to_string(content_length));
            return json();
        }

        std::string content(content_length, '\0');
        if (!std::cin.read(&content[0], content_length)) {
            log("Failed to read content, EOF reached");
            return json();
        }

        try {
            return json::parse(content);
        } catch (const json::parse_error& e) {
            log("JSON parse error: " + std::string(e.what()));
            return nullptr;
        }
    }

   public:
    O2LLSPServer() {
        o2l_binary = find_o2l_binary();
    }

    void run() {
        log("Starting O²L LSP server, binary: " + o2l_binary);

        while (true) {
            try {
                json message = read_message();
                if (message.is_null()) {
                    log("Connection closed, shutting down");
                    break;
                }

                if (message.contains("id")) {
                    // Request
                    handle_request(message);
                } else {
                    // Notification
                    handle_notification(message);
                }
            } catch (const std::exception& e) {
                log("Error in main loop: " + std::string(e.what()));
                break;
            }
        }

        log("LSP server shutting down");
    }
};

int main() {
    O2LLSPServer server;
    server.run();
    return 0;
}