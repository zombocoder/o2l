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

#include "HttpServerLibrary.hpp"

#include <algorithm>
#include <chrono>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>

#include "JsonLibrary.hpp"
#include "ListInstance.hpp"
#include "MapInstance.hpp"

// Platform-specific includes
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#ifdef __linux__
#include <sys/epoll.h>
#elif __APPLE__
#include <sys/event.h>
#endif
#endif

namespace o2l {

// Static member initialization
std::map<std::string, std::shared_ptr<HttpServer>> HttpServerLibrary::server_registry;
std::mutex HttpServerLibrary::registry_mutex;

//=============================================================================
// ThreadPool Implementation
//=============================================================================

ThreadPool::ThreadPool(size_t threads) : stop(false), active_threads(0) {
    for (size_t i = 0; i < threads; ++i) {
        workers.emplace_back([this] {
            for (;;) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(this->queue_mutex);
                    this->condition.wait(lock,
                                         [this] { return this->stop || !this->tasks.empty(); });

                    if (this->stop && this->tasks.empty()) return;

                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                }

                ++active_threads;
                task();
                --active_threads;
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    shutdown();
}

template <class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args)
    -> std::future<typename std::invoke_result<F, Args...>::type> {
    using return_type = typename std::invoke_result<F, Args...>::type;

    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queue_mutex);

        if (stop) {
            throw std::runtime_error("enqueue on stopped ThreadPool");
        }

        tasks.emplace([task]() { (*task)(); });
    }

    condition.notify_one();
    return res;
}

void ThreadPool::shutdown() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }

    condition.notify_all();

    for (std::thread& worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }

    workers.clear();
}

//=============================================================================
// Router Implementation
//=============================================================================

void Router::addRoute(const std::string& method, const std::string& pattern, RouteHandler handler) {
    std::lock_guard<std::mutex> lock(routes_mutex);

    Route route;
    route.method = method;
    route.pattern = pattern;
    route.handler = handler;

    // Extract parameter names from pattern (e.g., /users/:id -> "id")
    std::regex param_regex(R"(:([a-zA-Z_][a-zA-Z0-9_]*))");
    std::sregex_iterator iter(pattern.begin(), pattern.end(), param_regex);
    std::sregex_iterator end;

    for (; iter != end; ++iter) {
        route.param_names.push_back(iter->str(1));
    }

    routes.push_back(route);
}

bool Router::matchRoute(const std::string& method, const std::string& path, Route& matched_route,
                        std::map<std::string, std::string>& params) {
    std::lock_guard<std::mutex> lock(routes_mutex);

    for (const auto& route : routes) {
        if (route.method == method || route.method == "*") {
            std::vector<std::string> param_names = route.param_names;
            if (matchPattern(route.pattern, path, params, param_names)) {
                matched_route = route;
                return true;
            }
        }
    }
    return false;
}

bool Router::matchPattern(const std::string& pattern, const std::string& path,
                          std::map<std::string, std::string>& params,
                          std::vector<std::string>& param_names) {
    // Convert route pattern to regex
    std::string regex_pattern = pattern;

    // Replace :param with capture groups
    std::regex param_regex(R"(:([a-zA-Z_][a-zA-Z0-9_]*))");
    regex_pattern = std::regex_replace(regex_pattern, param_regex, "([^/]+)");

    // Handle wildcards
    std::regex wildcard_regex(R"(\*)");
    regex_pattern = std::regex_replace(regex_pattern, wildcard_regex, "(.*)");

    // Exact match
    regex_pattern = "^" + regex_pattern + "$";

    std::regex route_regex(regex_pattern);
    std::smatch matches;

    if (std::regex_match(path, matches, route_regex)) {
        // Extract parameters
        for (size_t i = 1; i < matches.size() && i - 1 < param_names.size(); ++i) {
            params[param_names[i - 1]] = matches[i].str();
        }
        return true;
    }

    return false;
}

//=============================================================================
// MiddlewareChain Implementation
//=============================================================================

void MiddlewareChain::use(MiddlewareFunction middleware) {
    middlewares.push_back(middleware);
}

void MiddlewareChain::execute(const HttpServerRequest& request, HttpServerResponse& response,
                              RouteHandler final_handler) {
    executeNext(0, request, response, final_handler);
}

void MiddlewareChain::executeNext(size_t index, const HttpServerRequest& request,
                                  HttpServerResponse& response, RouteHandler final_handler) {
    if (index >= middlewares.size()) {
        // All middleware executed, call final handler
        if (final_handler) {
            final_handler(request, response);
        }
        return;
    }

    // Call current middleware with next() function
    middlewares[index](request, response, [this, index, &request, &response, final_handler]() {
        executeNext(index + 1, request, response, final_handler);
    });
}

//=============================================================================
// HttpServer Implementation
//=============================================================================

HttpServer::HttpServer()
    : running(false), active_connections(0), total_requests(0), error_count(0), server_socket(-1) {
#ifdef _WIN32
    initializeWinsock();
#endif
}

HttpServer::~HttpServer() {
    stop();
#ifdef _WIN32
    cleanupWinsock();
#endif
}

#ifdef _WIN32
void HttpServer::initializeWinsock() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        throw std::runtime_error("WSAStartup failed: " + std::to_string(result));
    }
}

void HttpServer::cleanupWinsock() {
    WSACleanup();
}
#endif

bool HttpServer::listen() {
    if (running) {
        return false;  // Already running
    }

    // Create socket
#ifdef _WIN32
    server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket == INVALID_SOCKET) {
        logError("Failed to create socket: " + std::to_string(WSAGetLastError()));
        return false;
    }
#else
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        logError("Failed to create socket: " + std::string(strerror(errno)));
        return false;
    }
#endif

    // Set socket options
    int opt = 1;
#ifdef _WIN32
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt)) ==
        SOCKET_ERROR) {
        logError("Failed to set SO_REUSEADDR: " + std::to_string(WSAGetLastError()));
        closesocket(server_socket);
        return false;
    }
#else
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        logError("Failed to set SO_REUSEADDR: " + std::string(strerror(errno)));
        close(server_socket);
        return false;
    }
#endif

    // Bind socket
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(config.port);

    if (config.host == "0.0.0.0" || config.host == "*") {
        address.sin_addr.s_addr = INADDR_ANY;
    } else {
        inet_pton(AF_INET, config.host.c_str(), &address.sin_addr);
    }

#ifdef _WIN32
    if (bind(server_socket, (struct sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        logError("Failed to bind socket: " + std::to_string(WSAGetLastError()));
        closesocket(server_socket);
        return false;
    }

    if (::listen(server_socket, config.max_connections) == SOCKET_ERROR) {
        logError("Failed to listen on socket: " + std::to_string(WSAGetLastError()));
        closesocket(server_socket);
        return false;
    }
#else
    if (bind(server_socket, (struct sockaddr*)&address, sizeof(address)) < 0) {
        logError("Failed to bind socket: " + std::string(strerror(errno)));
        close(server_socket);
        return false;
    }

    if (::listen(server_socket, config.max_connections) < 0) {
        logError("Failed to listen on socket: " + std::string(strerror(errno)));
        close(server_socket);
        return false;
    }
#endif

    // Initialize thread pool
    thread_pool = std::make_unique<ThreadPool>(config.worker_threads);

    // Start accepting connections
    running = true;
    accept_thread = std::thread(&HttpServer::acceptConnections, this);

    std::cout << "HTTP Server listening on " << config.host << ":" << config.port << std::endl;
    return true;
}

void HttpServer::stop() {
    if (!running) {
        return;
    }

    running = false;

    // Close server socket to stop accepting new connections
#ifdef _WIN32
    if (server_socket != INVALID_SOCKET) {
        closesocket(server_socket);
        server_socket = INVALID_SOCKET;
    }
#else
    if (server_socket >= 0) {
        close(server_socket);
        server_socket = -1;
    }
#endif

    // Wait for accept thread to finish
    if (accept_thread.joinable()) {
        accept_thread.join();
    }

    // Shutdown thread pool
    if (thread_pool) {
        thread_pool->shutdown();
        thread_pool.reset();
    }

    std::cout << "HTTP Server stopped" << std::endl;
}

void HttpServer::waitForStop() {
    // Block until server is stopped
    if (accept_thread.joinable()) {
        accept_thread.join();
    }
}

void HttpServer::setCustomLogger(std::shared_ptr<ObjectInstance> logger_obj, Context* context) {
    std::lock_guard<std::mutex> lock(logger_mutex);
    custom_logger = logger_obj;
    logger_context = context;
}

void HttpServer::clearCustomLogger() {
    std::lock_guard<std::mutex> lock(logger_mutex);
    custom_logger = nullptr;
    logger_context = nullptr;
}

void HttpServer::static_(const std::string& url_path, const std::string& file_path) {
    // Add a route that serves static files
    router.get(url_path + "/*", [this, file_path, url_path](const HttpServerRequest& request,
                                                            HttpServerResponse& response) {
        std::string requested_path = request.path;

        // Remove the URL prefix to get the relative file path
        std::string relative_path = requested_path;
        if (requested_path.find(url_path) == 0) {
            relative_path = requested_path.substr(url_path.length());
        }

        // Construct full file path
        std::string full_path = file_path + relative_path;

        // Serve the static file
        if (!serveStaticFile(full_path, response)) {
            response.status_code = 404;
            response.status_message = "Not Found";
            response.body = "File not found";
        }
    });
}

void HttpServer::acceptConnections() {
    while (running) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

#ifdef _WIN32
        SOCKET client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        if (client_socket == INVALID_SOCKET) {
            if (running) {  // Only log if we're still supposed to be running
                logError("Failed to accept connection: " + std::to_string(WSAGetLastError()));
            }
            continue;
        }
#else
        int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        if (client_socket < 0) {
            if (running) {  // Only log if we're still supposed to be running
                logError("Failed to accept connection: " + std::string(strerror(errno)));
            }
            continue;
        }
#endif

        // Handle connection in thread pool
        ++active_connections;
        thread_pool->enqueue([this, client_socket]() {
            this->handleConnection(client_socket);
            --active_connections;
        });
    }
}

void HttpServer::handleConnection(int client_socket) {
    try {
        HttpServerRequest request;

        // Parse HTTP request
        if (!parseHttpRequest(client_socket, request)) {
            sendErrorResponse(client_socket, 400, "Bad Request");
#ifdef _WIN32
            closesocket(client_socket);
#else
            close(client_socket);
#endif
            return;
        }

        // Create response
        HttpServerResponse response;

        // Handle the request
        handleRequest(request, response);

        // Send response
        sendHttpResponse(client_socket, response);

        // Log the request
        logRequest(request, response);

        ++total_requests;

    } catch (const std::exception& e) {
        logError("Error handling connection: " + std::string(e.what()));
        sendErrorResponse(client_socket, 500, "Internal Server Error");
        ++error_count;
    }

    // Close connection
#ifdef _WIN32
    closesocket(client_socket);
#else
    close(client_socket);
#endif
}

bool HttpServer::parseHttpRequest(int client_socket, HttpServerRequest& request) {
    // Read request data
    char buffer[4096];
    std::string request_data;

    // Read until we have complete headers (ending with \r\n\r\n)
    bool headers_complete = false;
    while (!headers_complete && request_data.size() < config.max_request_size) {
#ifdef _WIN32
        int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0) return false;
#else
        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0) return false;
#endif

        buffer[bytes_received] = '\0';
        request_data += buffer;

        if (request_data.find("\r\n\r\n") != std::string::npos) {
            headers_complete = true;
        }
    }

    if (!headers_complete) {
        return false;
    }

    // Parse request line and headers
    std::istringstream stream(request_data);
    std::string line;

    // Parse request line: METHOD PATH HTTP/1.1
    if (!std::getline(stream, line) || line.empty()) {
        return false;
    }

    // Remove \r if present
    if (line.back() == '\r') {
        line.pop_back();
    }

    std::istringstream request_line(line);
    std::string http_version;

    if (!(request_line >> request.method >> request.path >> http_version)) {
        return false;
    }

    // Parse query string
    size_t query_pos = request.path.find('?');
    if (query_pos != std::string::npos) {
        request.query_string = request.path.substr(query_pos + 1);
        request.path = request.path.substr(0, query_pos);
        request.query_params = parseQueryString(request.query_string);
    }

    // Parse headers
    while (std::getline(stream, line) && !line.empty() && line != "\r") {
        if (line.back() == '\r') {
            line.pop_back();
        }

        size_t colon_pos = line.find(':');
        if (colon_pos != std::string::npos) {
            std::string name = line.substr(0, colon_pos);
            std::string value = line.substr(colon_pos + 1);

            // Trim whitespace
            name.erase(0, name.find_first_not_of(" \t"));
            name.erase(name.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);

            // Convert header name to lowercase
            std::transform(name.begin(), name.end(), name.begin(), ::tolower);

            request.headers[name] = value;
        }
    }

    // Read body if Content-Length is specified
    auto content_length_it = request.headers.find("content-length");
    if (content_length_it != request.headers.end()) {
        size_t content_length = std::stoul(content_length_it->second);
        if (content_length > 0 && content_length < config.max_request_size) {
            // Calculate how much body data we already have
            size_t headers_end_pos = request_data.find("\r\n\r\n") + 4;
            size_t body_data_received = request_data.size() - headers_end_pos;

            if (body_data_received > 0) {
                request.body = request_data.substr(headers_end_pos);
            }

            // Read remaining body data
            while (request.body.size() < content_length) {
                size_t remaining = content_length - request.body.size();
                size_t to_read = std::min(remaining, sizeof(buffer) - 1);

#ifdef _WIN32
                int bytes_received = recv(client_socket, buffer, static_cast<int>(to_read), 0);
                if (bytes_received <= 0) break;
#else
                ssize_t bytes_received = recv(client_socket, buffer, to_read, 0);
                if (bytes_received <= 0) break;
#endif

                request.body.append(buffer, bytes_received);
            }
        }
    }

    return true;
}

void HttpServer::sendHttpResponse(int client_socket, const HttpServerResponse& response) {
    std::ostringstream response_stream;

    // Status line
    response_stream << "HTTP/1.1 " << response.status_code << " " << response.status_message
                    << "\r\n";

    // Headers
    for (const auto& header : response.headers) {
        response_stream << header.first << ": " << header.second << "\r\n";
    }

    // Default headers
    if (response.headers.find("Content-Length") == response.headers.end()) {
        response_stream << "Content-Length: " << response.body.size() << "\r\n";
    }

    if (response.headers.find("Connection") == response.headers.end()) {
        response_stream << "Connection: " << (config.enable_keep_alive ? "keep-alive" : "close")
                        << "\r\n";
    }

    if (response.headers.find("Server") == response.headers.end()) {
        response_stream << "Server: O2L-HTTP-Server/1.0\r\n";
    }

    if (response.headers.find("Date") == response.headers.end()) {
        response_stream << "Date: " << formatHttpDate(std::time(nullptr)) << "\r\n";
    }

    // End headers
    response_stream << "\r\n";

    // Body
    if (!response.body.empty()) {
        response_stream << response.body;
    }

    std::string response_data = response_stream.str();

    // Send response
#ifdef _WIN32
    send(client_socket, response_data.c_str(), static_cast<int>(response_data.size()), 0);
#else
    send(client_socket, response_data.c_str(), response_data.size(), 0);
#endif
}

void HttpServer::handleRequest(const HttpServerRequest& request, HttpServerResponse& response) {
    try {
        // Try to match a route
        Router::Route matched_route;
        std::map<std::string, std::string> route_params;

        if (router.matchRoute(request.method, request.path, matched_route, route_params)) {
            // Create request with route parameters
            HttpServerRequest request_with_params = request;
            request_with_params.path_params = route_params;

            // Execute middleware chain and route handler
            middleware_chain.execute(request_with_params, response, matched_route.handler);
        } else {
            // No route found
            response.status_code = 404;
            response.status_message = "Not Found";
            response.body = "404 - Not Found";
        }

    } catch (const std::exception& e) {
        logError("Error handling request: " + std::string(e.what()));
        response.status_code = 500;
        response.status_message = "Internal Server Error";
        response.body = "500 - Internal Server Error";
        ++error_count;
    }
}

// URL decode utility function
std::string urlDecode(const std::string& encoded) {
    std::string decoded;
    decoded.reserve(encoded.length());

    for (size_t i = 0; i < encoded.length(); ++i) {
        if (encoded[i] == '%' && i + 2 < encoded.length()) {
            // Extract hex digits
            std::string hex = encoded.substr(i + 1, 2);
            try {
                int value = std::stoi(hex, nullptr, 16);
                decoded += static_cast<char>(value);
                i += 2;  // Skip the hex digits
            } catch (const std::exception&) {
                // Invalid hex, keep the % character
                decoded += encoded[i];
            }
        } else if (encoded[i] == '+') {
            // Plus signs represent spaces in query strings
            decoded += ' ';
        } else {
            decoded += encoded[i];
        }
    }

    return decoded;
}

// Utility function implementations
std::map<std::string, std::string> HttpServer::parseQueryString(const std::string& query) {
    std::map<std::string, std::string> params;
    std::istringstream query_stream(query);
    std::string pair;

    while (std::getline(query_stream, pair, '&')) {
        size_t equals_pos = pair.find('=');
        if (equals_pos != std::string::npos) {
            std::string key = pair.substr(0, equals_pos);
            std::string value = pair.substr(equals_pos + 1);

            // URL decode key and value
            params[urlDecode(key)] = urlDecode(value);
        } else {
            // Handle case where there's no equals sign (flag parameter)
            params[urlDecode(pair)] = "";
        }
    }

    return params;
}

std::string HttpServer::getMimeType(const std::string& filename) {
    size_t dot_pos = filename.find_last_of('.');
    if (dot_pos == std::string::npos) {
        return "application/octet-stream";
    }

    std::string extension = filename.substr(dot_pos + 1);
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

    static const std::map<std::string, std::string> mime_types = {
        {"html", "text/html"},        {"htm", "text/html"},
        {"css", "text/css"},          {"js", "application/javascript"},
        {"json", "application/json"}, {"xml", "application/xml"},
        {"txt", "text/plain"},        {"png", "image/png"},
        {"jpg", "image/jpeg"},        {"jpeg", "image/jpeg"},
        {"gif", "image/gif"},         {"svg", "image/svg+xml"},
        {"pdf", "application/pdf"},   {"zip", "application/zip"},
        {"gz", "application/gzip"}};

    auto it = mime_types.find(extension);
    return (it != mime_types.end()) ? it->second : "application/octet-stream";
}

bool HttpServer::serveStaticFile(const std::string& file_path, HttpServerResponse& response) {
    std::ifstream file(file_path, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    // Read file contents
    file.seekg(0, std::ios::end);
    size_t file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    response.body.resize(file_size);
    file.read(&response.body[0], file_size);

    // Set content type
    response.headers["Content-Type"] = getMimeType(file_path);
    response.headers["Content-Length"] = std::to_string(file_size);

    return true;
}

std::string HttpServer::formatHttpDate(time_t timestamp) {
    std::tm* gmt = std::gmtime(&timestamp);
    std::ostringstream ss;
    ss << std::put_time(gmt, "%a, %d %b %Y %H:%M:%S GMT");
    return ss.str();
}

void HttpServer::sendErrorResponse(int client_socket, int status_code, const std::string& message) {
    HttpServerResponse error_response;
    error_response.status_code = status_code;
    error_response.status_message = message;
    error_response.body =
        "<html><body><h1>" + std::to_string(status_code) + " " + message + "</h1></body></html>";
    error_response.headers["Content-Type"] = "text/html";

    sendHttpResponse(client_socket, error_response);
}

void HttpServer::logError(const std::string& message) {
    std::lock_guard<std::mutex> lock(logger_mutex);

    // Check if custom logger is available
    if (custom_logger && logger_context) {
        try {
            // Create error log object with getter methods
            auto error_obj = std::make_shared<ObjectInstance>("ErrorLogEntry");

            // Store data for getter methods
            std::string timestamp = formatHttpDate(time(nullptr));
            std::string level = "ERROR";
            std::string error_message = message;

            // Add getter methods for error properties
            error_obj->addMethod(
                "getTimestamp",
                [timestamp](const std::vector<Value>& args, Context& context) {
                    return Value(Text(timestamp));
                },
                true);

            error_obj->addMethod(
                "getLevel",
                [level](const std::vector<Value>& args, Context& context) {
                    return Value(Text(level));
                },
                true);

            error_obj->addMethod(
                "getMessage",
                [error_message](const std::vector<Value>& args, Context& context) {
                    return Value(Text(error_message));
                },
                true);

            // Call custom logger's logError method if available
            if (custom_logger->hasMethod("logError")) {
                std::vector<Value> args = {Value(error_obj)};
                custom_logger->callMethod("logError", args, *logger_context);
                return;  // Custom logging successful
            } else if (custom_logger->hasMethod("log")) {
                // Fallback to general log method
                std::vector<Value> args = {Value(error_obj)};
                custom_logger->callMethod("log", args, *logger_context);
                return;  // Custom logging successful
            }
        } catch (const std::exception& e) {
            // Custom logger failed, fall through to native logging
        }
    }

    // Native error logging
    logErrorNative(message);
}

void HttpServer::logRequest(const HttpServerRequest& request, const HttpServerResponse& response) {
    std::lock_guard<std::mutex> lock(logger_mutex);

    // Check if custom logger is available
    if (custom_logger && logger_context) {
        try {
            // Create log object with request details and getter methods
            auto log_obj = std::make_shared<ObjectInstance>("LogEntry");

            // Store data as properties for internal use
            std::string timestamp = formatHttpDate(time(nullptr));
            std::string method = request.method;
            std::string path = request.path;
            int status = response.status_code;
            int bytes = (int)response.body.size();
            std::string remote_address = request.remote_address;
            std::string user_agent =
                request.headers.count("User-Agent") ? request.headers.at("User-Agent") : "";

            // Add getter methods for each property
            log_obj->addMethod(
                "getTimestamp",
                [timestamp](const std::vector<Value>& args, Context& context) {
                    return Value(Text(timestamp));
                },
                true);

            log_obj->addMethod(
                "getMethod",
                [method](const std::vector<Value>& args, Context& context) {
                    return Value(Text(method));
                },
                true);

            log_obj->addMethod(
                "getPath",
                [path](const std::vector<Value>& args, Context& context) {
                    return Value(Text(path));
                },
                true);

            log_obj->addMethod(
                "getStatus",
                [status](const std::vector<Value>& args, Context& context) {
                    return Value(Int(status));
                },
                true);

            log_obj->addMethod(
                "getBytes",
                [bytes](const std::vector<Value>& args, Context& context) {
                    return Value(Int(bytes));
                },
                true);

            log_obj->addMethod(
                "getRemoteAddress",
                [remote_address](const std::vector<Value>& args, Context& context) {
                    return Value(Text(remote_address));
                },
                true);

            log_obj->addMethod(
                "getUserAgent",
                [user_agent](const std::vector<Value>& args, Context& context) {
                    return Value(Text(user_agent));
                },
                true);

            // Call custom logger's log method
            if (custom_logger->hasMethod("log")) {
                std::vector<Value> args = {Value(log_obj)};
                custom_logger->callMethod("log", args, *logger_context);
                return;  // Custom logging successful
            }
        } catch (const std::exception& e) {
            // Custom logger failed, fall through to native logging
        }
    }

    // Native request logging
    logRequestNative(request, response);
}

void HttpServer::logRequestNative(const HttpServerRequest& request,
                                  const HttpServerResponse& response) {
    std::cout << "[" << formatHttpDate(std::time(nullptr)) << "] " << request.method << " "
              << request.path << " " << response.status_code << " " << response.body.size()
              << " bytes" << std::endl;
}

void HttpServer::logErrorNative(const std::string& message) {
    std::cerr << "[ERROR] " << message << std::endl;
}

//=============================================================================
// HttpServerLibrary O²L Integration
//=============================================================================

std::shared_ptr<ObjectInstance> HttpServerLibrary::createHttpServerObject() {
    auto server_obj = std::make_shared<ObjectInstance>("HttpServer");

    // Server lifecycle methods
    server_obj->addMethod(
        "create",
        [](const std::vector<Value>& args, Context& context) {
            return nativeCreateServer(args, context);
        },
        true);

    server_obj->addMethod(
        "listen",
        [](const std::vector<Value>& args, Context& context) {
            return nativeListen(args, context);
        },
        true);

    server_obj->addMethod(
        "stop",
        [](const std::vector<Value>& args, Context& context) { return nativeStop(args, context); },
        true);

    server_obj->addMethod(
        "isRunning",
        [](const std::vector<Value>& args, Context& context) {
            return nativeIsRunning(args, context);
        },
        true);

    // Configuration methods
    server_obj->addMethod(
        "setHost",
        [](const std::vector<Value>& args, Context& context) {
            return nativeSetHost(args, context);
        },
        true);

    server_obj->addMethod(
        "setPort",
        [](const std::vector<Value>& args, Context& context) {
            return nativeSetPort(args, context);
        },
        true);

    server_obj->addMethod(
        "setWorkerThreads",
        [](const std::vector<Value>& args, Context& context) {
            return nativeSetWorkerThreads(args, context);
        },
        true);

    // Route definition methods
    server_obj->addMethod(
        "get",
        [](const std::vector<Value>& args, Context& context) { return nativeGet(args, context); },
        true);

    server_obj->addMethod(
        "post",
        [](const std::vector<Value>& args, Context& context) { return nativePost(args, context); },
        true);

    server_obj->addMethod(
        "put",
        [](const std::vector<Value>& args, Context& context) { return nativePut(args, context); },
        true);

    server_obj->addMethod(
        "delete",
        [](const std::vector<Value>& args, Context& context) {
            return nativeDelete(args, context);
        },
        true);

    server_obj->addMethod(
        "patch",
        [](const std::vector<Value>& args, Context& context) { return nativePatch(args, context); },
        true);

    // Static file serving
    server_obj->addMethod(
        "static",
        [](const std::vector<Value>& args, Context& context) {
            return nativeStatic(args, context);
        },
        true);

    // Middleware support
    server_obj->addMethod(
        "use",
        [](const std::vector<Value>& args, Context& context) { return nativeUse(args, context); },
        true);

    // Statistics
    server_obj->addMethod(
        "getStats",
        [](const std::vector<Value>& args, Context& context) {
            return nativeGetStats(args, context);
        },
        true);

    // Blocking wait
    server_obj->addMethod(
        "waitForever",
        [](const std::vector<Value>& args, Context& context) {
            return nativeWaitForever(args, context);
        },
        true);

    // Custom logging
    server_obj->addMethod(
        "setLogger",
        [](const std::vector<Value>& args, Context& context) {
            return nativeSetLogger(args, context);
        },
        true);

    return server_obj;
}

// Implementation of native methods (stubs for now)
Value HttpServerLibrary::nativeCreateServer(const std::vector<Value>& args, Context& context) {
    auto server = std::make_shared<HttpServer>();

    // Generate unique ID for server
    static std::atomic<size_t> server_counter{0};
    std::string server_id = "server_" + std::to_string(++server_counter);

    // Store server in registry
    {
        std::lock_guard<std::mutex> lock(registry_mutex);
        server_registry[server_id] = server;
    }

    // Create O²L server object
    auto server_obj = std::make_shared<ObjectInstance>("HttpServerInstance");
    server_obj->setProperty("server_id", Value(Text(server_id)));

    return Value(server_obj);
}

Value HttpServerLibrary::nativeListen(const std::vector<Value>& args, Context& context) {
    if (args.empty()) {
        throw std::runtime_error("listen() requires a server instance");
    }

    // Get server from the first argument
    auto server = getServerFromValue(args[0]);
    if (!server) {
        throw std::runtime_error("Invalid server instance");
    }

    try {
        bool success = server->listen();
        return Value(Bool(success));
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to start server: " + std::string(e.what()));
    }
}

Value HttpServerLibrary::nativeStop(const std::vector<Value>& args, Context& context) {
    if (args.empty()) {
        throw std::runtime_error("stop() requires a server instance");
    }

    // Get server from the first argument
    auto server = getServerFromValue(args[0]);
    if (!server) {
        throw std::runtime_error("Invalid server instance");
    }

    server->stop();
    return Value(Text("Server stopped"));
}

Value HttpServerLibrary::nativeIsRunning(const std::vector<Value>& args, Context& context) {
    if (args.empty()) {
        throw std::runtime_error("isRunning() requires a server instance");
    }

    // Get server from the first argument
    auto server = getServerFromValue(args[0]);
    if (!server) {
        throw std::runtime_error("Invalid server instance");
    }

    return Value(Bool(server->isRunning()));
}

Value HttpServerLibrary::nativeSetHost(const std::vector<Value>& args, Context& context) {
    if (args.size() < 2) {
        throw std::runtime_error("setHost() requires server instance and host string");
    }

    // Get server from the first argument
    auto server = getServerFromValue(args[0]);
    if (!server) {
        throw std::runtime_error("Invalid server instance");
    }

    // Get host from the second argument
    if (!std::holds_alternative<Text>(args[1])) {
        throw std::runtime_error("Host must be a string");
    }

    std::string host = std::get<Text>(args[1]);
    server->setHost(host);

    return Value(Text("Host set to " + host));
}

Value HttpServerLibrary::nativeSetPort(const std::vector<Value>& args, Context& context) {
    if (args.size() < 2) {
        throw std::runtime_error("setPort() requires server instance and port number");
    }

    // Get server from the first argument
    auto server = getServerFromValue(args[0]);
    if (!server) {
        throw std::runtime_error("Invalid server instance");
    }

    // Get port from the second argument
    if (!std::holds_alternative<Int>(args[1])) {
        throw std::runtime_error("Port must be an integer");
    }

    int port = std::get<Int>(args[1]);
    if (port <= 0 || port > 65535) {
        throw std::runtime_error("Port must be between 1 and 65535");
    }

    server->setPort(port);

    return Value(Text("Port set to " + std::to_string(port)));
}

Value HttpServerLibrary::nativeSetWorkerThreads(const std::vector<Value>& args, Context& context) {
    if (args.size() < 2) {
        throw std::runtime_error("setWorkerThreads() requires server instance and thread count");
    }

    // Get server from the first argument
    auto server = getServerFromValue(args[0]);
    if (!server) {
        throw std::runtime_error("Invalid server instance");
    }

    // Get thread count from the second argument
    if (!std::holds_alternative<Int>(args[1])) {
        throw std::runtime_error("Worker thread count must be an integer");
    }

    int threads = std::get<Int>(args[1]);
    if (threads <= 0 || threads > 100) {
        throw std::runtime_error("Worker thread count must be between 1 and 100");
    }

    server->setWorkerThreads(threads);

    return Value(Text("Worker threads set to " + std::to_string(threads)));
}

// Route definition methods
Value HttpServerLibrary::nativeGet(const std::vector<Value>& args, Context& context) {
    if (args.size() < 3) {
        throw std::runtime_error(
            "get() requires server instance, path pattern, and handler (or object + method)");
    }

    // Get server from the first argument
    auto server = getServerFromValue(args[0]);
    if (!server) {
        throw std::runtime_error("Invalid server instance");
    }

    // Get path pattern from the second argument
    if (!std::holds_alternative<Text>(args[1])) {
        throw std::runtime_error("Path pattern must be a string");
    }
    std::string pattern = std::get<Text>(args[1]);

    RouteHandler handler;

    if (args.size() == 4) {
        // Four arguments: server, pattern, object, method_name
        // Create route handler from object and method name
        handler = createObjectMethodHandler(args[2], args[3], context);
    } else {
        // Three arguments: server, pattern, handler (string or object)
        // Create route handler from the third argument
        handler = createRouteHandler(args[2], context);
    }

    // Register the route
    server->get(pattern, handler);

    return Value(Text("GET route registered for " + pattern));
}

Value HttpServerLibrary::nativePost(const std::vector<Value>& args, Context& context) {
    if (args.size() < 3) {
        throw std::runtime_error(
            "post() requires server instance, path pattern, and handler (or object + method)");
    }

    // Get server from the first argument
    auto server = getServerFromValue(args[0]);
    if (!server) {
        throw std::runtime_error("Invalid server instance");
    }

    // Get path pattern from the second argument
    if (!std::holds_alternative<Text>(args[1])) {
        throw std::runtime_error("Path pattern must be a string");
    }
    std::string pattern = std::get<Text>(args[1]);

    RouteHandler handler;

    if (args.size() == 4) {
        // Four arguments: server, pattern, object, method_name
        handler = createObjectMethodHandler(args[2], args[3], context);
    } else {
        // Three arguments: server, pattern, handler (string or object)
        handler = createRouteHandler(args[2], context);
    }

    // Register the route
    server->post(pattern, handler);

    return Value(Text("POST route registered for " + pattern));
}

Value HttpServerLibrary::nativePut(const std::vector<Value>& args, Context& context) {
    if (args.size() < 3) {
        throw std::runtime_error(
            "put() requires server instance, path pattern, and handler (or object + method)");
    }

    // Get server from the first argument
    auto server = getServerFromValue(args[0]);
    if (!server) {
        throw std::runtime_error("Invalid server instance");
    }

    // Get path pattern from the second argument
    if (!std::holds_alternative<Text>(args[1])) {
        throw std::runtime_error("Path pattern must be a string");
    }
    std::string pattern = std::get<Text>(args[1]);

    RouteHandler handler;

    if (args.size() == 4) {
        // Four arguments: server, pattern, object, method_name
        handler = createObjectMethodHandler(args[2], args[3], context);
    } else {
        // Three arguments: server, pattern, handler (string or object)
        handler = createRouteHandler(args[2], context);
    }

    // Register the route
    server->put(pattern, handler);

    return Value(Text("PUT route registered for " + pattern));
}

Value HttpServerLibrary::nativeDelete(const std::vector<Value>& args, Context& context) {
    if (args.size() < 3) {
        throw std::runtime_error(
            "delete() requires server instance, path pattern, and handler (or object + method)");
    }

    // Get server from the first argument
    auto server = getServerFromValue(args[0]);
    if (!server) {
        throw std::runtime_error("Invalid server instance");
    }

    // Get path pattern from the second argument
    if (!std::holds_alternative<Text>(args[1])) {
        throw std::runtime_error("Path pattern must be a string");
    }
    std::string pattern = std::get<Text>(args[1]);

    RouteHandler handler;

    if (args.size() == 4) {
        // Four arguments: server, pattern, object, method_name
        handler = createObjectMethodHandler(args[2], args[3], context);
    } else {
        // Three arguments: server, pattern, handler (string or object)
        handler = createRouteHandler(args[2], context);
    }

    // Register the route
    server->delete_(pattern, handler);

    return Value(Text("DELETE route registered for " + pattern));
}

Value HttpServerLibrary::nativePatch(const std::vector<Value>& args, Context& context) {
    if (args.size() < 3) {
        throw std::runtime_error(
            "patch() requires server instance, path pattern, and handler (or object + method)");
    }

    // Get server from the first argument
    auto server = getServerFromValue(args[0]);
    if (!server) {
        throw std::runtime_error("Invalid server instance");
    }

    // Get path pattern from the second argument
    if (!std::holds_alternative<Text>(args[1])) {
        throw std::runtime_error("Path pattern must be a string");
    }
    std::string pattern = std::get<Text>(args[1]);

    RouteHandler handler;

    if (args.size() == 4) {
        // Four arguments: server, pattern, object, method_name
        handler = createObjectMethodHandler(args[2], args[3], context);
    } else {
        // Three arguments: server, pattern, handler (string or object)
        handler = createRouteHandler(args[2], context);
    }

    // Register the route
    server->patch(pattern, handler);

    return Value(Text("PATCH route registered for " + pattern));
}

Value HttpServerLibrary::nativeStatic(const std::vector<Value>& args, Context& context) {
    if (args.size() < 3) {
        throw std::runtime_error(
            "static() requires server instance, URL path, and file system path");
    }

    // Get server from the first argument
    auto server = getServerFromValue(args[0]);
    if (!server) {
        throw std::runtime_error("Invalid server instance");
    }

    // Get URL path from the second argument
    if (!std::holds_alternative<Text>(args[1])) {
        throw std::runtime_error("URL path must be a string");
    }
    std::string url_path = std::get<Text>(args[1]);

    // Get file system path from the third argument
    if (!std::holds_alternative<Text>(args[2])) {
        throw std::runtime_error("File system path must be a string");
    }
    std::string fs_path = std::get<Text>(args[2]);

    // Register static file serving route
    server->static_(url_path, fs_path);

    return Value(Text("Static file serving registered for " + url_path + " -> " + fs_path));
}

Value HttpServerLibrary::nativeUse(const std::vector<Value>& args, Context& context) {
    if (args.size() < 2) {
        throw std::runtime_error("use() requires server instance and middleware");
    }

    // Get server from the first argument
    auto server = getServerFromValue(args[0]);
    if (!server) {
        throw std::runtime_error("Invalid server instance");
    }

    // Get middleware from the second argument
    const Value& middleware_value = args[1];

    // Create middleware function and register it
    MiddlewareFunction middleware_func = createMiddlewareFunction(middleware_value, context);
    server->use(middleware_func);

    return Value(Text("Middleware registered successfully"));
}

Value HttpServerLibrary::nativeGetStats(const std::vector<Value>& args, Context& context) {
    if (args.empty()) {
        throw std::runtime_error("getStats() requires server instance");
    }

    // Get server from the first argument
    auto server = getServerFromValue(args[0]);
    if (!server) {
        throw std::runtime_error("Invalid server instance");
    }

    // Create comprehensive statistics map
    auto stats = std::make_shared<MapInstance>();

    // Basic server statistics
    stats->put(Text("total_requests"), Value(Int(static_cast<int>(server->getTotalRequests()))));
    stats->put(Text("active_connections"),
               Value(Int(static_cast<int>(server->getActiveConnections()))));
    stats->put(Text("error_count"), Value(Int(static_cast<int>(server->getErrorCount()))));
    stats->put(Text("is_running"), Value(Bool(server->isRunning())));

    // Add server uptime (simple implementation)
    static auto start_time = std::chrono::steady_clock::now();
    auto current_time = std::chrono::steady_clock::now();
    auto uptime_seconds =
        std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count();
    stats->put(Text("uptime_seconds"), Value(Int(static_cast<int>(uptime_seconds))));

    // Calculate request rate (requests per second)
    double request_rate = 0.0;
    if (uptime_seconds > 0) {
        request_rate = static_cast<double>(server->getTotalRequests()) / uptime_seconds;
    }
    stats->put(Text("requests_per_second"), Value(Float(static_cast<float>(request_rate))));

    // Error rate percentage
    double error_rate = 0.0;
    if (server->getTotalRequests() > 0) {
        error_rate =
            (static_cast<double>(server->getErrorCount()) / server->getTotalRequests()) * 100.0;
    }
    stats->put(Text("error_rate_percent"), Value(Float(static_cast<float>(error_rate))));

    return Value(stats);
}

Value HttpServerLibrary::nativeWaitForever(const std::vector<Value>& args, Context& context) {
    if (args.empty()) {
        throw std::runtime_error("waitForever() requires a server instance");
    }

    // Get server from the first argument
    auto server = getServerFromValue(args[0]);
    if (!server) {
        throw std::runtime_error("Invalid server instance");
    }

    // Block until server is stopped
    try {
        server->waitForStop();
        return Value(Text("Server stopped"));
    } catch (const std::exception& e) {
        throw std::runtime_error("Error waiting for server: " + std::string(e.what()));
    }
}

Value HttpServerLibrary::nativeSetLogger(const std::vector<Value>& args, Context& context) {
    if (args.size() < 2) {
        throw std::runtime_error("setLogger() requires server instance and logger object");
    }

    // Get server from the first argument
    auto server = getServerFromValue(args[0]);
    if (!server) {
        throw std::runtime_error("Invalid server instance");
    }

    // Get logger object from second argument
    if (!std::holds_alternative<std::shared_ptr<ObjectInstance>>(args[1])) {
        throw std::runtime_error("Second argument must be a logger object");
    }

    auto logger_obj = std::get<std::shared_ptr<ObjectInstance>>(args[1]);

    // Set the custom logger
    server->setCustomLogger(logger_obj, &context);

    return Value(Text("Custom logger set successfully"));
}

// Helper methods
std::shared_ptr<HttpServer> HttpServerLibrary::getServerFromValue(const Value& server_value) {
    // The server_value should be an ObjectInstance with a server_id property
    if (!std::holds_alternative<std::shared_ptr<ObjectInstance>>(server_value)) {
        return nullptr;
    }

    auto server_obj = std::get<std::shared_ptr<ObjectInstance>>(server_value);
    if (!server_obj->hasProperty("server_id")) {
        return nullptr;
    }

    Value server_id_value = server_obj->getProperty("server_id");
    if (!std::holds_alternative<Text>(server_id_value)) {
        return nullptr;
    }

    std::string server_id = std::get<Text>(server_id_value);

    // Look up server in registry
    std::lock_guard<std::mutex> lock(registry_mutex);
    auto it = server_registry.find(server_id);
    if (it != server_registry.end()) {
        return it->second;
    }

    return nullptr;
}

RouteHandler HttpServerLibrary::createRouteHandler(const Value& handler_value, Context& context) {
    return [handler_value, &context](const HttpServerRequest& request,
                                     HttpServerResponse& response) {
        try {
            // Create request and response objects for O²L
            auto request_obj = createRequestObject(request);
            auto response_obj = createResponseObject(response);

            // Check what type of handler we have
            if (std::holds_alternative<Text>(handler_value)) {
                // String handler - create a simple named response
                std::string handler_name = std::get<Text>(handler_value);

                response.status_code = 200;
                response.headers["Content-Type"] = "application/json";

                // Create a JSON response with request information and handler name
                std::ostringstream json_response;
                json_response << "{"
                              << "\"message\": \"Hello from O²L HTTP Server!\","
                              << "\"handler\": \"" << handler_name << "\","
                              << "\"method\": \"" << request.method << "\","
                              << "\"path\": \"" << request.path << "\","
                              << "\"query\": \"" << request.query_string << "\"";

                // Add path parameters if any
                if (!request.path_params.empty()) {
                    json_response << ",\"path_params\": {";
                    bool first = true;
                    for (const auto& [key, value] : request.path_params) {
                        if (!first) json_response << ",";
                        json_response << "\"" << key << "\": \"" << value << "\"";
                        first = false;
                    }
                    json_response << "}";
                }

                // Add query parameters if any
                if (!request.query_params.empty()) {
                    json_response << ",\"query_params\": {";
                    bool first = true;
                    for (const auto& [key, value] : request.query_params) {
                        if (!first) json_response << ",";
                        json_response << "\"" << key << "\": \"" << value << "\"";
                        first = false;
                    }
                    json_response << "}";
                }

                json_response << ",\"timestamp\": " << std::time(nullptr);
                json_response << ",\"body\": \"" << request.body << "\"";
                json_response << "}";

                response.body = json_response.str();

            } else if (std::holds_alternative<std::shared_ptr<ObjectInstance>>(handler_value)) {
                // Object instance handler - call a method on the object
                auto handler_obj = std::get<std::shared_ptr<ObjectInstance>>(handler_value);

                // Try common method names in order of preference
                std::vector<std::string> method_candidates = {"handle", "execute", "run", "process",
                                                              "call"};
                std::string selected_method;

                for (const std::string& method_name : method_candidates) {
                    if (handler_obj->hasMethod(method_name)) {
                        selected_method = method_name;
                        break;
                    }
                }

                if (!selected_method.empty()) {
                    std::vector<Value> args = {Value(request_obj), Value(response_obj)};
                    Context handler_context = context;  // Copy context for handler
                    Value result = handler_obj->callMethod(selected_method, args, handler_context);

                    // The handler should have modified the response object
                    // If not, provide a default response
                    if (response.body.empty()) {
                        response.status_code = 200;
                        response.headers["Content-Type"] = "application/json";
                        response.body =
                            "{\"message\": \"Response from object handler\", \"result\": \"" +
                            (std::holds_alternative<Text>(result) ? std::get<Text>(result) : "OK") +
                            "\"}";
                    }
                } else {
                    // No suitable handler method found
                    response.status_code = 500;
                    response.headers["Content-Type"] = "application/json";
                    response.body = "{\"error\": \"No suitable handler method found in object '" +
                                    handler_obj->getName() +
                                    "'. Expected one of: handle, execute, run, process, call\"}";
                }

            } else {
                // Unknown handler type, provide default response
                response.status_code = 200;
                response.headers["Content-Type"] = "application/json";
                response.body = "{\"message\": \"Unknown handler type\"}";
            }

        } catch (const std::exception& e) {
            // Error handling
            response.status_code = 500;
            response.headers["Content-Type"] = "application/json";
            response.body = "{\"error\": \"Handler error: " + std::string(e.what()) + "\"}";
        }
    };
}

RouteHandler HttpServerLibrary::createObjectMethodHandler(const Value& object_value,
                                                          const Value& method_name_value,
                                                          Context& context) {
    // Validate arguments
    if (!std::holds_alternative<std::shared_ptr<ObjectInstance>>(object_value)) {
        throw std::runtime_error("Handler object must be an ObjectInstance");
    }
    if (!std::holds_alternative<Text>(method_name_value)) {
        throw std::runtime_error("Method name must be a string");
    }

    auto handler_obj = std::get<std::shared_ptr<ObjectInstance>>(object_value);
    std::string method_name = std::get<Text>(method_name_value);

    // Verify the method exists
    if (!handler_obj->hasMethod(method_name)) {
        throw std::runtime_error("Method '" + method_name + "' not found in object '" +
                                 handler_obj->getName() + "'");
    }

    return [handler_obj, method_name, &context](const HttpServerRequest& request,
                                                HttpServerResponse& response) {
        try {
            // Create request and response objects for O²L
            auto request_obj = createRequestObject(request);
            auto response_obj = createResponseObject(response);

            // Prepare arguments for the method call
            std::vector<Value> method_args = {Value(request_obj), Value(response_obj)};

            // Create a context copy for the handler
            Context handler_context = context;

            // Call the object method
            Value result = handler_obj->callMethod(method_name, method_args, handler_context);

            // The response object should have been modified by the handler
            // If no body was set, use the return value as JSON
            if (response.body.empty()) {
                response.headers["Content-Type"] = "application/json";

                if (std::holds_alternative<Text>(result)) {
                    // If result is text, use it as JSON string
                    std::string result_text = std::get<Text>(result);
                    if (result_text.empty() || result_text[0] != '{') {
                        // Wrap plain text in JSON
                        response.body = "{\"result\": \"" + result_text + "\"}";
                    } else {
                        // Use as-is if it looks like JSON
                        response.body = result_text;
                    }
                } else if (std::holds_alternative<Int>(result)) {
                    response.body = "{\"result\": " + std::to_string(std::get<Int>(result)) + "}";
                } else if (std::holds_alternative<Bool>(result)) {
                    response.body = std::string("{\"result\": ") +
                                    (std::get<Bool>(result) ? "true" : "false") + "}";
                } else {
                    response.body = "{\"result\": \"Method executed successfully\"}";
                }
            }

            // Ensure status code is set
            if (response.status_code == 0) {
                response.status_code = 200;
            }

        } catch (const std::exception& e) {
            // Error handling
            response.status_code = 500;
            response.headers["Content-Type"] = "application/json";
            response.body = "{\"error\": \"Method handler error: " + std::string(e.what()) + "\"}";
        }
    };
}

MiddlewareFunction HttpServerLibrary::createMiddlewareFunction(const Value& middleware_value,
                                                               Context& context) {
    return [middleware_value, &context](const HttpServerRequest& request,
                                        HttpServerResponse& response, std::function<void()> next) {
        try {
            // Create request and response objects for O²L
            auto request_obj = createRequestObject(request);
            auto response_obj = createResponseObject(response);

            // Create a next function object for O²L
            auto next_obj = std::make_shared<ObjectInstance>("NextFunction");
            next_obj->addMethod(
                "call",
                [next](const std::vector<Value>&, Context&) -> Value {
                    next();
                    return Value(Bool(true));
                },
                true);

            // Check what type of middleware we have
            if (std::holds_alternative<std::shared_ptr<ObjectInstance>>(middleware_value)) {
                // Object instance middleware - call a method on the object
                auto middleware_obj = std::get<std::shared_ptr<ObjectInstance>>(middleware_value);

                if (middleware_obj->hasMethod("handle")) {
                    std::vector<Value> args = {Value(request_obj), Value(response_obj),
                                               Value(next_obj)};
                    Context middleware_context = context;  // Copy context for middleware
                    middleware_obj->callMethod("handle", args, middleware_context);
                } else {
                    // If no handle method, just call next
                    next();
                }
            } else if (std::holds_alternative<Text>(middleware_value)) {
                // String middleware - create a simple logging middleware
                std::string middleware_name = std::get<Text>(middleware_value);

                // Log the request (could be enhanced to use custom logger)
                std::cout << "[" << middleware_name << "] " << request.method << " " << request.path
                          << std::endl;

                // Call next to continue processing
                next();
            } else {
                // Unknown middleware type, just call next
                next();
            }

        } catch (const std::exception& e) {
            // Error in middleware - log and continue
            std::cerr << "Middleware error: " << e.what() << std::endl;
            // Don't call next() on error - stops the chain
        }
    };
}

std::shared_ptr<ObjectInstance> HttpServerLibrary::createRequestObject(
    const HttpServerRequest& request) {
    auto request_obj = std::make_shared<ObjectInstance>("HttpRequest");

    // Add basic properties
    request_obj->setProperty("method", Value(Text(request.method)));
    request_obj->setProperty("path", Value(Text(request.path)));
    request_obj->setProperty("query_string", Value(Text(request.query_string)));
    request_obj->setProperty("body", Value(Text(request.body)));
    request_obj->setProperty("remote_address", Value(Text(request.remote_address)));
    request_obj->setProperty("remote_port", Value(Int(request.remote_port)));

    // Create headers map
    auto headers_map = std::make_shared<MapInstance>();
    for (const auto& [key, value] : request.headers) {
        headers_map->put(Text(key), Value(Text(value)));
    }
    request_obj->setProperty("headers", Value(headers_map));

    // Create query parameters map
    auto query_params_map = std::make_shared<MapInstance>();
    for (const auto& [key, value] : request.query_params) {
        query_params_map->put(Text(key), Value(Text(value)));
    }
    request_obj->setProperty("query_params", Value(query_params_map));

    // Create path parameters map
    auto path_params_map = std::make_shared<MapInstance>();
    for (const auto& [key, value] : request.path_params) {
        path_params_map->put(Text(key), Value(Text(value)));
    }
    request_obj->setProperty("path_params", Value(path_params_map));

    // Add convenience methods
    request_obj->addMethod(
        "getMethod",
        [request](const std::vector<Value>& args, Context& context) {
            return Value(Text(request.method));
        },
        true);

    request_obj->addMethod(
        "getPath",
        [request](const std::vector<Value>& args, Context& context) {
            return Value(Text(request.path));
        },
        true);

    request_obj->addMethod(
        "getQuery",
        [request](const std::vector<Value>& args, Context& context) {
            return Value(Text(request.query_string));
        },
        true);

    request_obj->addMethod(
        "getBody",
        [request](const std::vector<Value>& args, Context& context) {
            return Value(Text(request.body));
        },
        true);

    request_obj->addMethod(
        "getHeader",
        [request](const std::vector<Value>& args, Context& context) {
            if (args.empty() || !std::holds_alternative<Text>(args[0])) {
                throw std::runtime_error("getHeader() requires a header name");
            }
            std::string header_name = std::get<Text>(args[0]);
            auto it = request.headers.find(header_name);
            if (it != request.headers.end()) {
                return Value(Text(it->second));
            }
            return Value(Text(""));
        },
        true);

    request_obj->addMethod(
        "getParam",
        [request](const std::vector<Value>& args, Context& context) {
            if (args.empty() || !std::holds_alternative<Text>(args[0])) {
                throw std::runtime_error("getParam() requires a parameter name");
            }
            std::string param_name = std::get<Text>(args[0]);
            auto it = request.path_params.find(param_name);
            if (it != request.path_params.end()) {
                return Value(Text(it->second));
            }
            return Value(Text(""));
        },
        true);

    request_obj->addMethod(
        "getQueryParam",
        [request](const std::vector<Value>& args, Context& context) {
            if (args.empty() || !std::holds_alternative<Text>(args[0])) {
                throw std::runtime_error("getQueryParam() requires a parameter name");
            }
            std::string param_name = std::get<Text>(args[0]);
            auto it = request.query_params.find(param_name);
            if (it != request.query_params.end()) {
                return Value(Text(it->second));
            }
            return Value(Text(""));
        },
        true);

    request_obj->addMethod(
        "getRemoteAddress",
        [request](const std::vector<Value>& args, Context& context) {
            return Value(Text(request.remote_address));
        },
        true);

    request_obj->addMethod(
        "getRemotePort",
        [request](const std::vector<Value>& args, Context& context) {
            return Value(Int(request.remote_port));
        },
        true);

    request_obj->addMethod(
        "getHeaders",
        [request](const std::vector<Value>& args, Context& context) {
            auto headers_map = std::make_shared<MapInstance>();
            for (const auto& [key, value] : request.headers) {
                headers_map->put(Text(key), Value(Text(value)));
            }
            return Value(headers_map);
        },
        true);

    return request_obj;
}

std::shared_ptr<ObjectInstance> HttpServerLibrary::createResponseObject(
    HttpServerResponse& response) {
    auto response_obj = std::make_shared<ObjectInstance>("HttpResponse");

    // Add basic properties
    response_obj->setProperty("status_code", Value(Int(response.status_code)));
    response_obj->setProperty("status_message", Value(Text(response.status_message)));
    response_obj->setProperty("body", Value(Text(response.body)));
    response_obj->setProperty("sent", Value(Bool(response.sent)));
    response_obj->setProperty("chunked", Value(Bool(response.chunked)));

    // Create headers map
    auto headers_map = std::make_shared<MapInstance>();
    for (const auto& [key, value] : response.headers) {
        headers_map->put(Text(key), Value(Text(value)));
    }
    response_obj->setProperty("headers", Value(headers_map));

    // Add response control methods
    response_obj->addMethod(
        "setStatus",
        [&response](const std::vector<Value>& args, Context& context) {
            if (args.empty() || !std::holds_alternative<Int>(args[0])) {
                throw std::runtime_error("setStatus() requires a status code number");
            }
            int status = std::get<Int>(args[0]);
            if (status < 100 || status >= 600) {
                throw std::runtime_error("Invalid HTTP status code: " + std::to_string(status));
            }
            response.status_code = status;

            // Set default status message based on common codes
            switch (status) {
                case 200:
                    response.status_message = "OK";
                    break;
                case 201:
                    response.status_message = "Created";
                    break;
                case 204:
                    response.status_message = "No Content";
                    break;
                case 400:
                    response.status_message = "Bad Request";
                    break;
                case 401:
                    response.status_message = "Unauthorized";
                    break;
                case 403:
                    response.status_message = "Forbidden";
                    break;
                case 404:
                    response.status_message = "Not Found";
                    break;
                case 405:
                    response.status_message = "Method Not Allowed";
                    break;
                case 500:
                    response.status_message = "Internal Server Error";
                    break;
                case 502:
                    response.status_message = "Bad Gateway";
                    break;
                case 503:
                    response.status_message = "Service Unavailable";
                    break;
                default:
                    response.status_message = "Unknown";
                    break;
            }

            return Value(Text("Status set to " + std::to_string(status)));
        },
        true);

    response_obj->addMethod(
        "setHeader",
        [&response](const std::vector<Value>& args, Context& context) {
            if (args.size() < 2 || !std::holds_alternative<Text>(args[0]) ||
                !std::holds_alternative<Text>(args[1])) {
                throw std::runtime_error("setHeader() requires header name and value");
            }
            std::string header_name = std::get<Text>(args[0]);
            std::string header_value = std::get<Text>(args[1]);
            response.headers[header_name] = header_value;
            return Value(Text("Header '" + header_name + "' set"));
        },
        true);

    response_obj->addMethod(
        "setBody",
        [&response](const std::vector<Value>& args, Context& context) {
            if (args.empty() || !std::holds_alternative<Text>(args[0])) {
                throw std::runtime_error("setBody() requires body content");
            }
            std::string body_content = std::get<Text>(args[0]);
            response.body = body_content;
            return Value(Text("Body set"));
        },
        true);

    response_obj->addMethod(
        "json",
        [&response](const std::vector<Value>& args, Context& context) {
            if (args.empty() || !std::holds_alternative<Text>(args[0])) {
                throw std::runtime_error("json() requires JSON string");
            }
            std::string json_content = std::get<Text>(args[0]);
            response.body = json_content;
            response.headers["Content-Type"] = "application/json";
            return Value(Text("JSON response set"));
        },
        true);

    response_obj->addMethod(
        "html",
        [&response](const std::vector<Value>& args, Context& context) {
            if (args.empty() || !std::holds_alternative<Text>(args[0])) {
                throw std::runtime_error("html() requires HTML string");
            }
            std::string html_content = std::get<Text>(args[0]);
            response.body = html_content;
            response.headers["Content-Type"] = "text/html";
            return Value(Text("HTML response set"));
        },
        true);

    response_obj->addMethod(
        "text",
        [&response](const std::vector<Value>& args, Context& context) {
            if (args.empty() || !std::holds_alternative<Text>(args[0])) {
                throw std::runtime_error("text() requires text string");
            }
            std::string text_content = std::get<Text>(args[0]);
            response.body = text_content;
            response.headers["Content-Type"] = "text/plain";
            return Value(Text("Text response set"));
        },
        true);

    response_obj->addMethod(
        "redirect",
        [&response](const std::vector<Value>& args, Context& context) {
            if (args.empty() || !std::holds_alternative<Text>(args[0])) {
                throw std::runtime_error("redirect() requires URL");
            }
            std::string location = std::get<Text>(args[0]);

            // Default to 302 Found, but allow custom status code
            int status_code = 302;
            if (args.size() > 1 && std::holds_alternative<Int>(args[1])) {
                status_code = std::get<Int>(args[1]);
            }

            response.status_code = status_code;
            response.headers["Location"] = location;
            response.body = "";
            return Value(Text("Redirect to " + location));
        },
        true);

    // Utility methods
    response_obj->addMethod(
        "getStatus",
        [&response](const std::vector<Value>& args, Context& context) {
            return Value(Int(response.status_code));
        },
        true);

    response_obj->addMethod(
        "getHeader",
        [&response](const std::vector<Value>& args, Context& context) {
            if (args.empty() || !std::holds_alternative<Text>(args[0])) {
                throw std::runtime_error("getHeader() requires a header name");
            }
            std::string header_name = std::get<Text>(args[0]);
            auto it = response.headers.find(header_name);
            if (it != response.headers.end()) {
                return Value(Text(it->second));
            }
            return Value(Text(""));
        },
        true);

    response_obj->addMethod(
        "getBody",
        [&response](const std::vector<Value>& args, Context& context) {
            return Value(Text(response.body));
        },
        true);

    response_obj->addMethod(
        "send",
        [&response](const std::vector<Value>& args, Context& context) {
            if (args.empty() || !std::holds_alternative<Text>(args[0])) {
                throw std::runtime_error("send() requires response content");
            }
            std::string content = std::get<Text>(args[0]);
            response.body = content;
            response.sent = true;
            return Value(Text("Response sent"));
        },
        true);

    return response_obj;
}

}  // namespace o2l