#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

#include "Context.hpp"
#include "ObjectInstance.hpp"
#include "Value.hpp"

namespace o2l {

// Forward declarations
class HttpServerRequest;
class HttpServerResponse;
class Router;
class ThreadPool;
class MiddlewareChain;

// HTTP Server Request structure
struct HttpServerRequest {
    std::string method;
    std::string path;
    std::string query_string;
    std::map<std::string, std::string> headers;
    std::map<std::string, std::string> query_params;
    std::map<std::string, std::string> path_params;
    std::string body;
    std::string remote_address;
    int remote_port;

    HttpServerRequest() : remote_port(0) {}
};

// HTTP Server Response structure
struct HttpServerResponse {
    int status_code;
    std::string status_message;
    std::map<std::string, std::string> headers;
    std::string body;
    bool sent;
    bool chunked;

    HttpServerResponse() : status_code(200), status_message("OK"), sent(false), chunked(false) {}
};

// Route handler function type
using RouteHandler = std::function<void(const HttpServerRequest&, HttpServerResponse&)>;

// Middleware function type
using MiddlewareFunction =
    std::function<void(const HttpServerRequest&, HttpServerResponse&, std::function<void()>)>;

// Server configuration
struct HttpServerConfig {
    std::string host;
    int port;
    int worker_threads;
    int max_connections;
    int timeout_seconds;
    bool enable_keep_alive;
    bool enable_compression;
    size_t max_request_size;

    HttpServerConfig()
        : host("127.0.0.1"),
          port(8080),
          worker_threads(4),
          max_connections(1000),
          timeout_seconds(30),
          enable_keep_alive(true),
          enable_compression(true),
          max_request_size(10 * 1024 * 1024) {}  // 10MB default
};

// Thread Pool for handling concurrent requests
class ThreadPool {
   public:
    ThreadPool(size_t threads);
    ~ThreadPool();

    template <class F, class... Args>
    auto enqueue(F&& f, Args&&... args)
        -> std::future<typename std::invoke_result<F, Args...>::type>;

    void shutdown();
    size_t getActiveThreads() const {
        return active_threads;
    }
    size_t getQueueSize() const {
        return tasks.size();
    }

   private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    std::atomic<bool> stop;
    std::atomic<size_t> active_threads;
};

// Router for handling URL pattern matching
class Router {
   public:
    struct Route {
        std::string method;
        std::string pattern;
        RouteHandler handler;
        std::vector<std::string> param_names;
    };

    void addRoute(const std::string& method, const std::string& pattern, RouteHandler handler);
    bool matchRoute(const std::string& method, const std::string& path, Route& matched_route,
                    std::map<std::string, std::string>& params);

    void get(const std::string& pattern, RouteHandler handler) {
        addRoute("GET", pattern, handler);
    }
    void post(const std::string& pattern, RouteHandler handler) {
        addRoute("POST", pattern, handler);
    }
    void put(const std::string& pattern, RouteHandler handler) {
        addRoute("PUT", pattern, handler);
    }
    void delete_(const std::string& pattern, RouteHandler handler) {
        addRoute("DELETE", pattern, handler);
    }
    void patch(const std::string& pattern, RouteHandler handler) {
        addRoute("PATCH", pattern, handler);
    }
    void head(const std::string& pattern, RouteHandler handler) {
        addRoute("HEAD", pattern, handler);
    }
    void options(const std::string& pattern, RouteHandler handler) {
        addRoute("OPTIONS", pattern, handler);
    }

   private:
    std::vector<Route> routes;
    std::mutex routes_mutex;

    bool matchPattern(const std::string& pattern, const std::string& path,
                      std::map<std::string, std::string>& params,
                      std::vector<std::string>& param_names);
};

// Middleware chain for request processing
class MiddlewareChain {
   public:
    void use(MiddlewareFunction middleware);
    void execute(const HttpServerRequest& request, HttpServerResponse& response,
                 RouteHandler final_handler);

   private:
    std::vector<MiddlewareFunction> middlewares;
    void executeNext(size_t index, const HttpServerRequest& request, HttpServerResponse& response,
                     RouteHandler final_handler);
};

// Main HTTP Server class
class HttpServer {
   public:
    HttpServer();
    ~HttpServer();

    // Configuration
    void setHost(const std::string& host) {
        config.host = host;
    }
    void setPort(int port) {
        config.port = port;
    }
    void setWorkerThreads(int threads) {
        config.worker_threads = threads;
    }
    void setMaxConnections(int max_conn) {
        config.max_connections = max_conn;
    }
    void setTimeout(int seconds) {
        config.timeout_seconds = seconds;
    }
    void setKeepAlive(bool enable) {
        config.enable_keep_alive = enable;
    }
    void setCompression(bool enable) {
        config.enable_compression = enable;
    }
    void setMaxRequestSize(size_t size) {
        config.max_request_size = size;
    }

    // Routing
    void get(const std::string& pattern, RouteHandler handler) {
        router.get(pattern, handler);
    }
    void post(const std::string& pattern, RouteHandler handler) {
        router.post(pattern, handler);
    }
    void put(const std::string& pattern, RouteHandler handler) {
        router.put(pattern, handler);
    }
    void delete_(const std::string& pattern, RouteHandler handler) {
        router.delete_(pattern, handler);
    }
    void patch(const std::string& pattern, RouteHandler handler) {
        router.patch(pattern, handler);
    }
    void head(const std::string& pattern, RouteHandler handler) {
        router.head(pattern, handler);
    }
    void options(const std::string& pattern, RouteHandler handler) {
        router.options(pattern, handler);
    }

    // Middleware
    void use(MiddlewareFunction middleware) {
        middleware_chain.use(middleware);
    }

    // Static file serving
    void static_(const std::string& url_path, const std::string& file_path);

    // Server lifecycle
    bool listen();
    void stop();
    void waitForStop();
    bool isRunning() const {
        return running;
    }

    // Logging configuration
    void setCustomLogger(std::shared_ptr<ObjectInstance> logger_obj, Context* context);
    void clearCustomLogger();

    // Statistics
    size_t getActiveConnections() const {
        return active_connections;
    }
    size_t getTotalRequests() const {
        return total_requests;
    }
    size_t getErrorCount() const {
        return error_count;
    }

   private:
    HttpServerConfig config;
    Router router;
    MiddlewareChain middleware_chain;
    std::unique_ptr<ThreadPool> thread_pool;

    // Server state
    std::atomic<bool> running;
    std::atomic<size_t> active_connections;
    std::atomic<size_t> total_requests;
    std::atomic<size_t> error_count;

    // Socket handling
    int server_socket;
    std::thread accept_thread;

    // Custom logging
    std::shared_ptr<ObjectInstance> custom_logger;
    Context* logger_context;
    std::mutex logger_mutex;

    // Platform-specific implementations
#ifdef _WIN32
    void initializeWinsock();
    void cleanupWinsock();
#endif

    // Core server functionality
    void acceptConnections();
    void handleConnection(int client_socket);
    bool parseHttpRequest(int client_socket, HttpServerRequest& request);
    void sendHttpResponse(int client_socket, const HttpServerResponse& response);
    void handleRequest(const HttpServerRequest& request, HttpServerResponse& response);

    // Utility functions
    std::map<std::string, std::string> parseQueryString(const std::string& query);
    std::string getMimeType(const std::string& filename);
    bool serveStaticFile(const std::string& file_path, HttpServerResponse& response);
    std::string formatHttpDate(time_t timestamp);

    // Error handling
    void sendErrorResponse(int client_socket, int status_code, const std::string& message);
    void logError(const std::string& message);
    void logRequest(const HttpServerRequest& request, const HttpServerResponse& response);

    // Native logging fallbacks
    void logRequestNative(const HttpServerRequest& request, const HttpServerResponse& response);
    void logErrorNative(const std::string& message);
};

// O²L HTTP Server Library wrapper
class HttpServerLibrary {
   public:
    // Create the http.server module object
    static std::shared_ptr<ObjectInstance> createHttpServerObject();

    // Server management methods
    static Value nativeCreateServer(const std::vector<Value>& args, Context& context);
    static Value nativeListen(const std::vector<Value>& args, Context& context);
    static Value nativeStop(const std::vector<Value>& args, Context& context);
    static Value nativeIsRunning(const std::vector<Value>& args, Context& context);

    // Configuration methods
    static Value nativeSetHost(const std::vector<Value>& args, Context& context);
    static Value nativeSetPort(const std::vector<Value>& args, Context& context);
    static Value nativeSetWorkerThreads(const std::vector<Value>& args, Context& context);
    static Value nativeSetMaxConnections(const std::vector<Value>& args, Context& context);
    static Value nativeSetTimeout(const std::vector<Value>& args, Context& context);
    static Value nativeSetKeepAlive(const std::vector<Value>& args, Context& context);
    static Value nativeSetCompression(const std::vector<Value>& args, Context& context);

    // Route definition methods
    static Value nativeGet(const std::vector<Value>& args, Context& context);
    static Value nativePost(const std::vector<Value>& args, Context& context);
    static Value nativePut(const std::vector<Value>& args, Context& context);
    static Value nativeDelete(const std::vector<Value>& args, Context& context);
    static Value nativePatch(const std::vector<Value>& args, Context& context);
    static Value nativeHead(const std::vector<Value>& args, Context& context);
    static Value nativeOptions(const std::vector<Value>& args, Context& context);

    // Static file serving
    static Value nativeStatic(const std::vector<Value>& args, Context& context);

    // Middleware support
    static Value nativeUse(const std::vector<Value>& args, Context& context);

    // Response utilities
    static Value nativeSetStatus(const std::vector<Value>& args, Context& context);
    static Value nativeSetHeader(const std::vector<Value>& args, Context& context);
    static Value nativeSend(const std::vector<Value>& args, Context& context);
    static Value nativeSendJson(const std::vector<Value>& args, Context& context);
    static Value nativeSendFile(const std::vector<Value>& args, Context& context);

    // Request utilities
    static Value nativeGetMethod(const std::vector<Value>& args, Context& context);
    static Value nativeGetPath(const std::vector<Value>& args, Context& context);
    static Value nativeGetQuery(const std::vector<Value>& args, Context& context);
    static Value nativeGetHeaders(const std::vector<Value>& args, Context& context);
    static Value nativeGetBody(const std::vector<Value>& args, Context& context);
    static Value nativeGetParam(const std::vector<Value>& args, Context& context);
    static Value nativeGetJsonBody(const std::vector<Value>& args, Context& context);

    // Statistics methods
    static Value nativeGetStats(const std::vector<Value>& args, Context& context);

    // Blocking operations
    static Value nativeWaitForever(const std::vector<Value>& args, Context& context);

    // Logging configuration
    static Value nativeSetLogger(const std::vector<Value>& args, Context& context);

   private:
    // Helper methods
    static std::shared_ptr<HttpServer> getServerFromValue(const Value& server_value);
    static std::shared_ptr<ObjectInstance> createRequestObject(const HttpServerRequest& request);
    static std::shared_ptr<ObjectInstance> createResponseObject(HttpServerResponse& response);
    static RouteHandler createRouteHandler(const Value& handler_value, Context& context);
    static RouteHandler createObjectMethodHandler(const Value& object_value,
                                                  const Value& method_name_value, Context& context);
    static MiddlewareFunction createMiddlewareFunction(const Value& middleware_value,
                                                       Context& context);

    // Server registry for managing server instances
    static std::map<std::string, std::shared_ptr<HttpServer>> server_registry;
    static std::mutex registry_mutex;
};

}  // namespace o2l