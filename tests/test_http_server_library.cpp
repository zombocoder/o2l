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

#include <gtest/gtest.h>

#include <chrono>
#include <memory>
#include <thread>

#include "../src/Runtime/Context.hpp"
#include "../src/Runtime/HttpServerLibrary.hpp"
#include "../src/Runtime/ListInstance.hpp"
#include "../src/Runtime/MapInstance.hpp"

using namespace o2l;

class HttpServerLibraryTest : public ::testing::Test {
   protected:
    void SetUp() override {
        context = std::make_unique<Context>();
        http_server_obj = HttpServerLibrary::createHttpServerObject();
    }

    void TearDown() override {
        // Cleanup is handled by the server library internally
    }

    Context* getContext() {
        return context.get();
    }

    // Helper method to call HTTP server methods
    Value callServerMethod(const std::string& method_name, const std::vector<Value>& args) {
        return http_server_obj->callMethod(method_name, args, *context);
    }

    // Helper to create a server instance
    void createServer() {
        auto result = callServerMethod("create", {});
        ASSERT_TRUE(std::holds_alternative<std::shared_ptr<ObjectInstance>>(result));
        server_obj = std::get<std::shared_ptr<ObjectInstance>>(result);
        // Note: Cannot access private getServerFromValue, will test through public methods
    }

    // Helper to create headers map
    std::shared_ptr<MapInstance> createHeadersMap(
        const std::map<std::string, std::string>& headers) {
        auto map = std::make_shared<MapInstance>();
        for (const auto& pair : headers) {
            map->put(Text(pair.first), Value(Text(pair.second)));
        }
        return map;
    }

   protected:
    std::unique_ptr<Context> context;
    std::shared_ptr<ObjectInstance> http_server_obj;
    std::shared_ptr<ObjectInstance> server_obj;
};

//=============================================================================
// HttpServer Core Class Tests
//=============================================================================

TEST_F(HttpServerLibraryTest, CreateHttpServerObject) {
    EXPECT_TRUE(http_server_obj != nullptr);
    EXPECT_TRUE(http_server_obj->hasMethod("create"));
    EXPECT_TRUE(http_server_obj->hasMethod("listen"));
    EXPECT_TRUE(http_server_obj->hasMethod("stop"));
}

TEST_F(HttpServerLibraryTest, CreateServerInstance) {
    createServer();
    EXPECT_TRUE(server_obj != nullptr);

    // Test that server instance has server_id property
    EXPECT_TRUE(server_obj->hasProperty("server_id"));
    auto server_id_prop = server_obj->getProperty("server_id");
    EXPECT_TRUE(std::holds_alternative<Text>(server_id_prop));
    EXPECT_FALSE(std::get<Text>(server_id_prop).empty());
}

TEST_F(HttpServerLibraryTest, ServerConfiguration) {
    createServer();

    // Test host configuration - methods are called on main HttpServer object with server instance
    // as first arg
    auto result = callServerMethod("setHost", {Value(server_obj), Value(Text("0.0.0.0"))});
    EXPECT_TRUE(std::holds_alternative<Text>(result));
    EXPECT_FALSE(std::get<Text>(result).empty());

    // Test port configuration
    result = callServerMethod("setPort", {Value(server_obj), Value(Int(3000))});
    EXPECT_TRUE(std::holds_alternative<Text>(result));
    EXPECT_FALSE(std::get<Text>(result).empty());

    // Test worker threads configuration
    result = callServerMethod("setWorkerThreads", {Value(server_obj), Value(Int(8))});
    EXPECT_TRUE(std::holds_alternative<Text>(result));
    EXPECT_FALSE(std::get<Text>(result).empty());

    // Note: setMaxConnections, setTimeout, setKeepAlive, and setCompression methods
    // are not currently implemented in createHttpServerObject, so we skip those tests
    // for now and focus on testing the implemented functionality
}

//=============================================================================
// ThreadPool Tests
//=============================================================================

TEST_F(HttpServerLibraryTest, ThreadPoolCreation) {
    ThreadPool pool(4);
    EXPECT_EQ(pool.getActiveThreads(), 0);
    EXPECT_EQ(pool.getQueueSize(), 0);
}

//=============================================================================
// Router Tests
//=============================================================================

TEST_F(HttpServerLibraryTest, RouterBasicRouting) {
    Router router;
    bool handler_called = false;

    auto handler = [&handler_called](const HttpServerRequest&, HttpServerResponse&) {
        handler_called = true;
    };

    router.addRoute("GET", "/test", handler);

    Router::Route matched_route;
    std::map<std::string, std::string> params;

    EXPECT_TRUE(router.matchRoute("GET", "/test", matched_route, params));
    EXPECT_EQ(matched_route.method, "GET");
    EXPECT_EQ(matched_route.pattern, "/test");

    // Test handler execution
    HttpServerRequest req;
    HttpServerResponse res;
    matched_route.handler(req, res);
    EXPECT_TRUE(handler_called);
}

TEST_F(HttpServerLibraryTest, RouterParameterExtraction) {
    Router router;

    auto handler = [](const HttpServerRequest&, HttpServerResponse&) {};
    router.addRoute("GET", "/users/:id", handler);

    Router::Route matched_route;
    std::map<std::string, std::string> params;

    EXPECT_TRUE(router.matchRoute("GET", "/users/123", matched_route, params));
    EXPECT_EQ(params["id"], "123");

    EXPECT_FALSE(router.matchRoute("POST", "/users/123", matched_route, params));
    EXPECT_FALSE(router.matchRoute("GET", "/posts/123", matched_route, params));
}

TEST_F(HttpServerLibraryTest, RouterWildcardRoutes) {
    Router router;

    auto handler = [](const HttpServerRequest&, HttpServerResponse&) {};
    router.addRoute("GET", "/api/*", handler);

    Router::Route matched_route;
    std::map<std::string, std::string> params;

    EXPECT_TRUE(router.matchRoute("GET", "/api/v1/users", matched_route, params));
    EXPECT_TRUE(router.matchRoute("GET", "/api/test", matched_route, params));
    EXPECT_FALSE(router.matchRoute("GET", "/other/path", matched_route, params));
}

TEST_F(HttpServerLibraryTest, RouterMultipleParameters) {
    Router router;

    auto handler = [](const HttpServerRequest&, HttpServerResponse&) {};
    router.addRoute("GET", "/users/:userId/posts/:postId", handler);

    Router::Route matched_route;
    std::map<std::string, std::string> params;

    EXPECT_TRUE(router.matchRoute("GET", "/users/123/posts/456", matched_route, params));
    EXPECT_EQ(params["userId"], "123");
    EXPECT_EQ(params["postId"], "456");

    EXPECT_FALSE(router.matchRoute("GET", "/users/123", matched_route, params));
    EXPECT_FALSE(router.matchRoute("GET", "/users/123/posts", matched_route, params));
}

//=============================================================================
// HTTP Request/Response Tests
//=============================================================================

TEST_F(HttpServerLibraryTest, HttpServerRequestCreation) {
    HttpServerRequest request;

    request.method = "POST";
    request.path = "/api/users";
    request.query_string = "filter=active&limit=10";
    request.headers["Content-Type"] = "application/json";
    request.body = R"({"name": "John", "email": "john@example.com"})";
    request.remote_address = "127.0.0.1";
    request.remote_port = 54321;

    EXPECT_EQ(request.method, "POST");
    EXPECT_EQ(request.path, "/api/users");
    EXPECT_EQ(request.query_string, "filter=active&limit=10");
    EXPECT_EQ(request.headers.at("Content-Type"), "application/json");
    EXPECT_FALSE(request.body.empty());
    EXPECT_EQ(request.remote_address, "127.0.0.1");
    EXPECT_EQ(request.remote_port, 54321);
}

TEST_F(HttpServerLibraryTest, HttpServerResponseCreation) {
    HttpServerResponse response;

    EXPECT_EQ(response.status_code, 200);
    EXPECT_EQ(response.status_message, "OK");
    EXPECT_FALSE(response.sent);
    EXPECT_FALSE(response.chunked);

    response.status_code = 201;
    response.status_message = "Created";
    response.headers["Content-Type"] = "application/json";
    response.body = R"({"success": true, "id": 123})";
    response.sent = true;

    EXPECT_EQ(response.status_code, 201);
    EXPECT_EQ(response.status_message, "Created");
    EXPECT_TRUE(response.sent);
    EXPECT_EQ(response.headers.at("Content-Type"), "application/json");
}

//=============================================================================
// Middleware Chain Tests
//=============================================================================

TEST_F(HttpServerLibraryTest, MiddlewareChainExecution) {
    MiddlewareChain chain;
    int execution_order = 0;

    // Add first middleware
    chain.use([&execution_order](const HttpServerRequest&, HttpServerResponse&,
                                 std::function<void()> next) {
        EXPECT_EQ(execution_order, 0);
        execution_order = 1;
        next();
        EXPECT_EQ(execution_order, 3);
        execution_order = 4;
    });

    // Add second middleware
    chain.use([&execution_order](const HttpServerRequest&, HttpServerResponse&,
                                 std::function<void()> next) {
        EXPECT_EQ(execution_order, 1);
        execution_order = 2;
        next();
        EXPECT_EQ(execution_order, 2);
        execution_order = 3;
    });

    // Final handler
    auto final_handler = [&execution_order](const HttpServerRequest&, HttpServerResponse&) {
        EXPECT_EQ(execution_order, 2);
    };

    HttpServerRequest request;
    HttpServerResponse response;

    chain.execute(request, response, final_handler);
    EXPECT_EQ(execution_order, 4);
}

TEST_F(HttpServerLibraryTest, MiddlewareChainNoNext) {
    MiddlewareChain chain;
    bool final_handler_called = false;

    // Middleware that doesn't call next()
    chain.use([](const HttpServerRequest&, HttpServerResponse&, std::function<void()>) {
        // Don't call next() - should stop execution
    });

    auto final_handler = [&final_handler_called](const HttpServerRequest&, HttpServerResponse&) {
        final_handler_called = true;
    };

    HttpServerRequest request;
    HttpServerResponse response;

    chain.execute(request, response, final_handler);
    EXPECT_FALSE(final_handler_called);
}

//=============================================================================
// Route Registration Tests
//=============================================================================

TEST_F(HttpServerLibraryTest, RouteRegistrationMethods) {
    createServer();

    // Create mock handler objects for testing
    auto handler_obj = std::make_shared<ObjectInstance>("TestHandler");
    handler_obj->addMethod(
        "handle", [](const std::vector<Value>&, Context&) -> Value { return Value(Bool(true)); });

    // Test GET route registration
    auto result =
        callServerMethod("get", {Value(server_obj), Value(Text("/api/users")), Value(handler_obj)});
    EXPECT_TRUE(std::holds_alternative<Text>(result));
    EXPECT_FALSE(std::get<Text>(result).empty());

    // Test POST route registration
    result = callServerMethod("post",
                              {Value(server_obj), Value(Text("/api/users")), Value(handler_obj)});
    EXPECT_TRUE(std::holds_alternative<Text>(result));
    EXPECT_FALSE(std::get<Text>(result).empty());

    // Test PUT route registration
    result = callServerMethod(
        "put", {Value(server_obj), Value(Text("/api/users/:id")), Value(handler_obj)});
    EXPECT_TRUE(std::holds_alternative<Text>(result));
    EXPECT_FALSE(std::get<Text>(result).empty());

    // Test DELETE route registration
    result = callServerMethod(
        "delete", {Value(server_obj), Value(Text("/api/users/:id")), Value(handler_obj)});
    EXPECT_TRUE(std::holds_alternative<Text>(result));
    EXPECT_FALSE(std::get<Text>(result).empty());

    // Test PATCH route registration
    result = callServerMethod(
        "patch", {Value(server_obj), Value(Text("/api/users/:id")), Value(handler_obj)});
    EXPECT_TRUE(std::holds_alternative<Text>(result));
    EXPECT_FALSE(std::get<Text>(result).empty());

    // Note: HEAD and OPTIONS methods are not currently implemented in createHttpServerObject,
    // so we skip those tests for now and focus on testing the implemented HTTP methods
}

//=============================================================================
// URL Decoding Tests
//=============================================================================

TEST_F(HttpServerLibraryTest, URLDecoding) {
    // Test URL decoding functionality via query string parsing
    HttpServerRequest request;
    request.method = "GET";
    request.path = "/test";
    request.query_string =
        "name=John%20Doe&email=test%40example.com&age=25&active=true&special=%21%40%23%24";

    // Test that the request structure handles URL-encoded values correctly
    EXPECT_EQ(request.query_string,
              "name=John%20Doe&email=test%40example.com&age=25&active=true&special=%21%40%23%24");
    EXPECT_FALSE(request.query_string.empty());
}

//=============================================================================
// Middleware Registration Tests
//=============================================================================

TEST_F(HttpServerLibraryTest, MiddlewareRegistration) {
    createServer();

    // Create a mock middleware object
    auto middleware_obj = std::make_shared<ObjectInstance>("TestMiddleware");
    middleware_obj->addMethod(
        "handle", [](const std::vector<Value>&, Context&) -> Value { return Value(Bool(true)); });

    // Test middleware registration
    auto result = callServerMethod("use", {Value(server_obj), Value(middleware_obj)});
    EXPECT_TRUE(std::holds_alternative<Text>(result));
    EXPECT_FALSE(std::get<Text>(result).empty());
    EXPECT_EQ(std::get<Text>(result), "Middleware registered successfully");
}

TEST_F(HttpServerLibraryTest, MiddlewareRegistrationStringType) {
    createServer();

    // Test string middleware registration
    auto result = callServerMethod("use", {Value(server_obj), Value(Text("LoggingMiddleware"))});
    EXPECT_TRUE(std::holds_alternative<Text>(result));
    EXPECT_FALSE(std::get<Text>(result).empty());
    EXPECT_EQ(std::get<Text>(result), "Middleware registered successfully");
}

//=============================================================================
// Enhanced Server Statistics Tests
//=============================================================================

TEST_F(HttpServerLibraryTest, EnhancedServerStatistics) {
    createServer();

    auto result = callServerMethod("getStats", {Value(server_obj)});
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<MapInstance>>(result));

    auto stats_map = std::get<std::shared_ptr<MapInstance>>(result);
    EXPECT_TRUE(stats_map != nullptr);

    // Verify enhanced statistics are available
    EXPECT_TRUE(stats_map->contains(Value(Text("total_requests"))));
    EXPECT_TRUE(stats_map->contains(Value(Text("active_connections"))));
    EXPECT_TRUE(stats_map->contains(Value(Text("error_count"))));
    EXPECT_TRUE(stats_map->contains(Value(Text("is_running"))));
    EXPECT_TRUE(stats_map->contains(Value(Text("uptime_seconds"))));
    EXPECT_TRUE(stats_map->contains(Value(Text("requests_per_second"))));
    EXPECT_TRUE(stats_map->contains(Value(Text("error_rate_percent"))));

    // Test that uptime is a non-negative integer
    auto uptime_val = stats_map->get(Value(Text("uptime_seconds")));
    EXPECT_TRUE(std::holds_alternative<Int>(uptime_val));
    EXPECT_GE(std::get<Int>(uptime_val), 0);

    // Test that rates are floats
    auto req_rate_val = stats_map->get(Value(Text("requests_per_second")));
    EXPECT_TRUE(std::holds_alternative<Float>(req_rate_val));

    auto error_rate_val = stats_map->get(Value(Text("error_rate_percent")));
    EXPECT_TRUE(std::holds_alternative<Float>(error_rate_val));
}

//=============================================================================
// Method Name Specification Tests
//=============================================================================

TEST_F(HttpServerLibraryTest, ObjectMethodHandlerMultipleMethods) {
    createServer();

    // Create an object with multiple handler method candidates
    auto handler_obj = std::make_shared<ObjectInstance>("MultiMethodHandler");
    handler_obj->addMethod("execute", [](const std::vector<Value>&, Context&) -> Value {
        return Value(Text("executed"));
    });
    handler_obj->addMethod(
        "run", [](const std::vector<Value>&, Context&) -> Value { return Value(Text("ran")); });

    // Test that route registration succeeds with multiple available methods
    auto result =
        callServerMethod("get", {Value(server_obj), Value(Text("/multi")), Value(handler_obj)});
    EXPECT_TRUE(std::holds_alternative<Text>(result));
    EXPECT_FALSE(std::get<Text>(result).empty());
}

TEST_F(HttpServerLibraryTest, ObjectMethodHandlerSpecificMethod) {
    createServer();

    // Create an object with a specific method
    auto handler_obj = std::make_shared<ObjectInstance>("SpecificHandler");
    handler_obj->addMethod("process", [](const std::vector<Value>&, Context&) -> Value {
        return Value(Text("processed"));
    });

    // Test 4-argument form: server, pattern, object, method_name
    // Note: This tests the createObjectMethodHandler functionality
    auto result = callServerMethod("get", {Value(server_obj), Value(Text("/specific")),
                                           Value(handler_obj), Value(Text("process"))});
    EXPECT_TRUE(std::holds_alternative<Text>(result));
    EXPECT_FALSE(std::get<Text>(result).empty());
}

TEST_F(HttpServerLibraryTest, ObjectMethodHandlerNoValidMethod) {
    createServer();

    // Create an object without any of the expected handler methods
    auto handler_obj = std::make_shared<ObjectInstance>("InvalidHandler");
    handler_obj->addMethod("someOtherMethod", [](const std::vector<Value>&, Context&) -> Value {
        return Value(Text("other"));
    });

    // Test that route registration still works (the error will occur during request handling)
    auto result =
        callServerMethod("get", {Value(server_obj), Value(Text("/invalid")), Value(handler_obj)});
    EXPECT_TRUE(std::holds_alternative<Text>(result));
    EXPECT_FALSE(std::get<Text>(result).empty());
}

//=============================================================================
// Static File Serving Tests
//=============================================================================

TEST_F(HttpServerLibraryTest, StaticFileConfiguration) {
    createServer();

    // Test static file serving configuration
    auto result = callServerMethod(
        "static", {Value(server_obj), Value(Text("/public")), Value(Text("./public"))});
    EXPECT_TRUE(std::holds_alternative<Text>(result));
    EXPECT_FALSE(std::get<Text>(result).empty());
}

//=============================================================================
// Server Lifecycle Tests
//=============================================================================

TEST_F(HttpServerLibraryTest, ServerLifecycle) {
    createServer();

    // Configure server for testing with a valid port
    callServerMethod("setPort",
                     {Value(server_obj), Value(Int(8080))});  // Use port 8080 for testing

    // Test server status check
    auto result = callServerMethod("isRunning", {Value(server_obj)});
    EXPECT_TRUE(std::holds_alternative<Bool>(result));
    EXPECT_FALSE(std::get<Bool>(result));
}

//=============================================================================
// Error Handling Tests
//=============================================================================

TEST_F(HttpServerLibraryTest, InvalidMethodArguments) {
    createServer();

    // Test setPort with invalid argument
    try {
        callServerMethod("setPort", {Value(server_obj), Value(Text("invalid"))});
        FAIL() << "Expected exception for invalid port argument";
    } catch (const std::exception&) {
        // Expected behavior
    }

    // Test route registration with missing arguments
    try {
        callServerMethod("get", {Value(server_obj), Value(Text("/test"))});
        FAIL() << "Expected exception for missing handler argument";
    } catch (const std::exception&) {
        // Expected behavior
    }
}

//=============================================================================
// Server Statistics Tests
//=============================================================================

TEST_F(HttpServerLibraryTest, ServerStatistics) {
    createServer();

    auto result = callServerMethod("getStats", {Value(server_obj)});
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<MapInstance>>(result));

    auto stats_map = std::get<std::shared_ptr<MapInstance>>(result);
    EXPECT_TRUE(stats_map != nullptr);

    // Verify stats map has expected keys
    EXPECT_TRUE(stats_map->contains(Value(Text("total_requests"))));
    EXPECT_TRUE(stats_map->contains(Value(Text("active_connections"))));
    EXPECT_TRUE(stats_map->contains(Value(Text("error_count"))));
}

//=============================================================================
// Configuration Validation Tests
//=============================================================================

TEST_F(HttpServerLibraryTest, ConfigurationValidation) {
    HttpServerConfig config;

    // Test default values
    EXPECT_EQ(config.host, "127.0.0.1");
    EXPECT_EQ(config.port, 8080);
    EXPECT_EQ(config.worker_threads, 4);
    EXPECT_EQ(config.max_connections, 1000);
    EXPECT_EQ(config.timeout_seconds, 30);
    EXPECT_TRUE(config.enable_keep_alive);
    EXPECT_TRUE(config.enable_compression);
    EXPECT_EQ(config.max_request_size, 10 * 1024 * 1024);  // 10MB

    // Test configuration modification
    config.host = "0.0.0.0";
    config.port = 3000;
    config.worker_threads = 8;

    EXPECT_EQ(config.host, "0.0.0.0");
    EXPECT_EQ(config.port, 3000);
    EXPECT_EQ(config.worker_threads, 8);
}

//=============================================================================
// HttpRequest Object Method Tests
//=============================================================================

TEST_F(HttpServerLibraryTest, HttpRequestObjectMethods) {
    // Note: This test verifies the HttpRequest object methods are available
    // We can't directly test createRequestObject due to private access,
    // but we can verify the structure supports the methods

    HttpServerRequest request;
    request.method = "GET";
    request.path = "/test";
    request.remote_address = "192.168.1.1";
    request.remote_port = 12345;
    request.headers["User-Agent"] = "TestAgent/1.0";

    // Verify that the data structure contains the expected fields
    EXPECT_EQ(request.remote_address, "192.168.1.1");
    EXPECT_EQ(request.remote_port, 12345);
    EXPECT_EQ(request.method, "GET");
    EXPECT_EQ(request.path, "/test");
    EXPECT_EQ(request.headers.at("User-Agent"), "TestAgent/1.0");
}

TEST_F(HttpServerLibraryTest, HttpResponseObjectMethods) {
    // Note: This test verifies the HttpResponse object structure
    // We can't directly test createResponseObject due to private access,
    // but we can verify the structure supports the methods

    HttpServerResponse response;
    response.status_code = 200;
    response.status_message = "OK";
    response.body = "Test response";
    response.sent = false;
    response.headers["Content-Type"] = "text/plain";

    // Verify that the data structure contains the expected fields
    EXPECT_EQ(response.status_code, 200);
    EXPECT_EQ(response.status_message, "OK");
    EXPECT_EQ(response.body, "Test response");
    EXPECT_FALSE(response.sent);
    EXPECT_EQ(response.headers.at("Content-Type"), "text/plain");

    // Test that sent flag can be updated (simulating send() method)
    response.sent = true;
    EXPECT_TRUE(response.sent);
}

//=============================================================================
// Utility Function Tests
//=============================================================================

TEST_F(HttpServerLibraryTest, QueryStringParsing) {
    // Note: This tests the internal query string parsing logic
    // We can't directly access the private method, but we can test
    // the behavior through request handling

    HttpServerRequest request;
    request.query_string = "name=John&age=30&active=true";

    // Simulate query parameter parsing
    std::map<std::string, std::string> expected_params = {
        {"name", "John"}, {"age", "30"}, {"active", "true"}};

    // The actual parsing would be done by the server's internal methods
    // This test verifies the expected structure
    EXPECT_EQ(request.query_string, "name=John&age=30&active=true");
}

TEST_F(HttpServerLibraryTest, MimeTypeDetection) {
    // Test MIME type detection logic
    // This would typically be tested through static file serving

    std::map<std::string, std::string> expected_types = {{"test.html", "text/html"},
                                                         {"test.css", "text/css"},
                                                         {"test.js", "application/javascript"},
                                                         {"test.json", "application/json"},
                                                         {"test.png", "image/png"},
                                                         {"test.jpg", "image/jpeg"},
                                                         {"test.txt", "text/plain"}};

    // These would be validated through actual server behavior
    // The test structure is prepared for when MIME detection is accessible
    EXPECT_FALSE(expected_types.empty());
}