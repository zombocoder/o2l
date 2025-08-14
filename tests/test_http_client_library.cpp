#include <gtest/gtest.h>

#include "../src/Runtime/Context.hpp"
#include "../src/Runtime/HttpClientLibrary.hpp"
#include "../src/Runtime/ListInstance.hpp"
#include "../src/Runtime/MapInstance.hpp"

using namespace o2l;

class HttpClientLibraryTest : public ::testing::Test {
   protected:
    void SetUp() override {
        context = std::make_unique<Context>();
        http_client = HttpClientLibrary::createHttpClientObject();
    }

    Context* getContext() {
        return context.get();
    }

    // Helper method to call HTTP client methods
    Value callHttpMethod(const std::string& method_name, const std::vector<Value>& args) {
        return http_client->callMethod(method_name, args, *context);
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

    std::unique_ptr<Context> context;
    std::shared_ptr<ObjectInstance> http_client;
};

// Test HTTP Client Object Creation
TEST_F(HttpClientLibraryTest, HttpClientCreation) {
    ASSERT_NE(http_client, nullptr);
    EXPECT_EQ(http_client->getName(), "HttpClient");

    // Verify all basic HTTP methods exist
    EXPECT_TRUE(http_client->hasMethod("get"));
    EXPECT_TRUE(http_client->hasMethod("post"));
    EXPECT_TRUE(http_client->hasMethod("put"));
    EXPECT_TRUE(http_client->hasMethod("delete"));
    EXPECT_TRUE(http_client->hasMethod("patch"));
    EXPECT_TRUE(http_client->hasMethod("head"));
    EXPECT_TRUE(http_client->hasMethod("options"));
}

// Test Basic GET Request
TEST_F(HttpClientLibraryTest, BasicGetRequest) {
    // Test simple GET request
    std::vector<Value> args = {Text("https://httpbin.org/get")};

    EXPECT_NO_THROW({
        Value response = callHttpMethod("get", args);
        ASSERT_TRUE(std::holds_alternative<std::shared_ptr<ObjectInstance>>(response));

        auto response_obj = std::get<std::shared_ptr<ObjectInstance>>(response);
        EXPECT_EQ(response_obj->getName(), "HttpResponse");

        // Check response has required properties
        EXPECT_TRUE(response_obj->hasProperty("status_code"));
        EXPECT_TRUE(response_obj->hasProperty("status_message"));
        EXPECT_TRUE(response_obj->hasProperty("body"));
        EXPECT_TRUE(response_obj->hasProperty("success"));
        EXPECT_TRUE(response_obj->hasProperty("headers"));
    });
}

// Test GET Request with Headers
TEST_F(HttpClientLibraryTest, GetRequestWithHeaders) {
    auto headers =
        createHeadersMap({{"User-Agent", "O2L-HTTP-Client/1.0"}, {"Accept", "application/json"}});

    std::vector<Value> args = {Text("https://httpbin.org/get"), Value(headers)};

    EXPECT_NO_THROW({
        Value response = callHttpMethod("get", args);
        ASSERT_TRUE(std::holds_alternative<std::shared_ptr<ObjectInstance>>(response));
    });
}

// Test POST Request
TEST_F(HttpClientLibraryTest, BasicPostRequest) {
    std::string json_body = R"({"name": "Alice", "age": 30})";

    std::vector<Value> args = {Text("https://httpbin.org/post"), Text(json_body)};

    EXPECT_NO_THROW({
        Value response = callHttpMethod("post", args);
        ASSERT_TRUE(std::holds_alternative<std::shared_ptr<ObjectInstance>>(response));

        auto response_obj = std::get<std::shared_ptr<ObjectInstance>>(response);
        EXPECT_EQ(response_obj->getName(), "HttpResponse");
    });
}

// Test POST Request with Headers
TEST_F(HttpClientLibraryTest, PostRequestWithHeaders) {
    std::string json_body = R"({"message": "Hello World"})";
    auto headers = createHeadersMap(
        {{"Content-Type", "application/json"}, {"Authorization", "Bearer test-token"}});

    std::vector<Value> args = {Text("https://httpbin.org/post"), Text(json_body), Value(headers)};

    EXPECT_NO_THROW({
        Value response = callHttpMethod("post", args);
        ASSERT_TRUE(std::holds_alternative<std::shared_ptr<ObjectInstance>>(response));
    });
}

// Test PUT Request
TEST_F(HttpClientLibraryTest, BasicPutRequest) {
    std::string json_body = R"({"id": 1, "name": "Updated Name"})";

    std::vector<Value> args = {Text("https://httpbin.org/put"), Text(json_body)};

    EXPECT_NO_THROW({
        Value response = callHttpMethod("put", args);
        ASSERT_TRUE(std::holds_alternative<std::shared_ptr<ObjectInstance>>(response));
    });
}

// Test DELETE Request
TEST_F(HttpClientLibraryTest, BasicDeleteRequest) {
    std::vector<Value> args = {Text("https://httpbin.org/delete")};

    EXPECT_NO_THROW({
        Value response = callHttpMethod("delete", args);
        ASSERT_TRUE(std::holds_alternative<std::shared_ptr<ObjectInstance>>(response));
    });
}

// Test PATCH Request
TEST_F(HttpClientLibraryTest, BasicPatchRequest) {
    std::string json_body = R"({"status": "active"})";

    std::vector<Value> args = {Text("https://httpbin.org/patch"), Text(json_body)};

    EXPECT_NO_THROW({
        Value response = callHttpMethod("patch", args);
        ASSERT_TRUE(std::holds_alternative<std::shared_ptr<ObjectInstance>>(response));
    });
}

// Test HEAD Request
TEST_F(HttpClientLibraryTest, BasicHeadRequest) {
    std::vector<Value> args = {Text("https://httpbin.org/get")};

    EXPECT_NO_THROW({
        Value response = callHttpMethod("head", args);
        ASSERT_TRUE(std::holds_alternative<std::shared_ptr<ObjectInstance>>(response));
    });
}

// Test OPTIONS Request
TEST_F(HttpClientLibraryTest, BasicOptionsRequest) {
    std::vector<Value> args = {Text("https://httpbin.org/get")};

    EXPECT_NO_THROW({
        Value response = callHttpMethod("options", args);
        ASSERT_TRUE(std::holds_alternative<std::shared_ptr<ObjectInstance>>(response));
    });
}

// Test Generic Request Method
TEST_F(HttpClientLibraryTest, GenericRequestMethod) {
    std::vector<Value> args = {Text("GET"), Text("https://httpbin.org/get")};

    EXPECT_NO_THROW({
        Value response = callHttpMethod("request", args);
        ASSERT_TRUE(std::holds_alternative<std::shared_ptr<ObjectInstance>>(response));
    });

    // Test with body and headers
    std::string body = R"({"test": "data"})";
    auto headers = createHeadersMap({{"Content-Type", "application/json"}});

    std::vector<Value> full_args = {Text("POST"), Text("https://httpbin.org/post"), Text(body),
                                    Value(headers)};

    EXPECT_NO_THROW({
        Value response = callHttpMethod("request", full_args);
        ASSERT_TRUE(std::holds_alternative<std::shared_ptr<ObjectInstance>>(response));
    });
}

// Test Request Configuration
TEST_F(HttpClientLibraryTest, RequestConfiguration) {
    // Test createRequest
    std::vector<Value> create_args = {Text("POST"), Text("https://httpbin.org/post")};

    EXPECT_NO_THROW({
        Value request_obj = callHttpMethod("createRequest", create_args);
        ASSERT_TRUE(std::holds_alternative<std::shared_ptr<ObjectInstance>>(request_obj));

        auto request = std::get<std::shared_ptr<ObjectInstance>>(request_obj);
        EXPECT_EQ(request->getName(), "HttpRequest");

        // Verify request properties
        EXPECT_TRUE(request->hasProperty("method"));
        EXPECT_TRUE(request->hasProperty("url"));
        EXPECT_TRUE(request->hasProperty("headers"));
        EXPECT_TRUE(request->hasProperty("body"));
        EXPECT_TRUE(request->hasProperty("timeout_seconds"));
    });
}

// Test Authentication Methods
TEST_F(HttpClientLibraryTest, AuthenticationMethods) {
    // Create a dummy request object for testing
    Value request_obj =
        callHttpMethod("createRequest", {Text("GET"), Text("https://httpbin.org/get")});

    // Test Basic Auth
    EXPECT_NO_THROW({
        Value result = callHttpMethod("setBasicAuth", {request_obj, Text("user"), Text("pass")});
        EXPECT_EQ(std::get<Text>(result), "Basic auth set");
    });

    // Test Bearer Token
    EXPECT_NO_THROW({
        Value result = callHttpMethod("setBearerToken", {request_obj, Text("abc123token")});
        EXPECT_EQ(std::get<Text>(result), "Bearer token set");
    });

    // Test API Key
    EXPECT_NO_THROW({
        Value result =
            callHttpMethod("setApiKey", {request_obj, Text("X-API-Key"), Text("secret-key")});
        EXPECT_EQ(std::get<Text>(result), "API key set");
    });
}

// Test URL Utility Methods
TEST_F(HttpClientLibraryTest, UrlUtilities) {
    // Test URL encoding
    std::vector<Value> encode_args = {Text("hello world & test")};
    Value encoded_result = callHttpMethod("urlEncode", encode_args);

    ASSERT_TRUE(std::holds_alternative<Text>(encoded_result));
    std::string encoded = std::get<Text>(encoded_result);
    EXPECT_NE(encoded.find("%20"), std::string::npos);  // Space should be encoded

    // Test URL decoding
    std::vector<Value> decode_args = {Text("hello%20world%20%26%20test")};
    Value decoded_result = callHttpMethod("urlDecode", decode_args);

    ASSERT_TRUE(std::holds_alternative<Text>(decoded_result));
    std::string decoded = std::get<Text>(decoded_result);
    EXPECT_EQ(decoded, "hello world & test");
}

// Test URL Building
TEST_F(HttpClientLibraryTest, UrlBuilding) {
    // Test basic URL building
    std::vector<Value> args = {Text("https://api.example.com/users")};
    Value built_url = callHttpMethod("buildUrl", args);

    ASSERT_TRUE(std::holds_alternative<Text>(built_url));
    EXPECT_EQ(std::get<Text>(built_url), "https://api.example.com/users");

    // Test with query parameters
    auto params = createHeadersMap({{"page", "1"}, {"limit", "10"}, {"sort", "name"}});

    std::vector<Value> params_args = {Text("https://api.example.com/users"), Value(params)};

    EXPECT_NO_THROW({
        Value built_url_with_params = callHttpMethod("buildUrl", params_args);
        ASSERT_TRUE(std::holds_alternative<Text>(built_url_with_params));

        std::string final_url = std::get<Text>(built_url_with_params);
        EXPECT_NE(final_url.find("?"), std::string::npos);
    });
}

// Test URL Parsing
TEST_F(HttpClientLibraryTest, UrlParsing) {
    std::vector<Value> args = {Text("https://api.example.com/v1/users?page=1&limit=10")};
    Value parsed_result = callHttpMethod("parseUrl", args);

    ASSERT_TRUE(std::holds_alternative<std::shared_ptr<MapInstance>>(parsed_result));
    auto url_parts = std::get<std::shared_ptr<MapInstance>>(parsed_result);

    // Check parsed components
    Value protocol = url_parts->get(Text("protocol"));
    Value host = url_parts->get(Text("host"));
    Value path = url_parts->get(Text("path"));
    Value query = url_parts->get(Text("query"));

    EXPECT_EQ(std::get<Text>(protocol), "https");
    EXPECT_EQ(std::get<Text>(host), "api.example.com");
    EXPECT_EQ(std::get<Text>(path), "/v1/users");
    EXPECT_EQ(std::get<Text>(query), "page=1&limit=10");
}

// Test Response Object Properties
TEST_F(HttpClientLibraryTest, ResponseObjectProperties) {
    std::vector<Value> args = {Text("https://httpbin.org/get")};

    EXPECT_NO_THROW({
        Value response = callHttpMethod("get", args);
        ASSERT_TRUE(std::holds_alternative<std::shared_ptr<ObjectInstance>>(response));

        auto response_obj = std::get<std::shared_ptr<ObjectInstance>>(response);

        // Test getStatus - accept any valid HTTP status code
        Value status = callHttpMethod("getStatus", {response});
        ASSERT_TRUE(std::holds_alternative<Int>(status));
        Int status_code = std::get<Int>(status);
        EXPECT_GE(status_code, 100);  // Valid HTTP status codes start from 100
        EXPECT_LT(status_code, 600);  // Valid HTTP status codes are less than 600

        // Test getStatusMessage - should return a non-empty string
        Value status_msg = callHttpMethod("getStatusMessage", {response});
        ASSERT_TRUE(std::holds_alternative<Text>(status_msg));
        Text status_message = std::get<Text>(status_msg);
        EXPECT_FALSE(status_message.empty());

        // Test isSuccess - should be consistent with status code
        Value is_success = callHttpMethod("isSuccess", {response});
        ASSERT_TRUE(std::holds_alternative<Bool>(is_success));
        Bool success = std::get<Bool>(is_success);
        // Success should be true for 2xx status codes
        if (status_code >= 200 && status_code < 300) {
            EXPECT_TRUE(success);
        } else {
            EXPECT_FALSE(success);
        }

        // Test getBody - should return a string (may be empty for errors)
        Value body = callHttpMethod("getBody", {response});
        ASSERT_TRUE(std::holds_alternative<Text>(body));

        // Test getHeaders - should return a Map
        Value headers = callHttpMethod("getHeaders", {response});
        ASSERT_TRUE(std::holds_alternative<std::shared_ptr<MapInstance>>(headers));

        // Test getError - should return a string (may be empty for success)
        Value error = callHttpMethod("getError", {response});
        ASSERT_TRUE(std::holds_alternative<Text>(error));
    });
}

// Test File Operations
TEST_F(HttpClientLibraryTest, FileOperations) {
    // Test uploadFile method signature
    EXPECT_NO_THROW({
        Value result = callHttpMethod(
            "uploadFile", {Text("https://httpbin.org/post"), Text("/tmp/test_file.txt")});
        // Note: This will fail in actual execution since file doesn't exist
        // but we're testing the method signature and error handling
    });

    // Test downloadFile method signature
    EXPECT_NO_THROW({
        Value result = callHttpMethod(
            "downloadFile", {Text("https://httpbin.org/get"), Text("/tmp/downloaded_file.txt")});
    });
}

// Test Request Configuration Methods
TEST_F(HttpClientLibraryTest, RequestConfigurationMethods) {
    Value request_obj =
        callHttpMethod("createRequest", {Text("POST"), Text("https://httpbin.org/post")});

    // Test setHeader
    EXPECT_NO_THROW({
        Value result = callHttpMethod(
            "setHeader", {request_obj, Text("Content-Type"), Text("application/json")});
        EXPECT_EQ(std::get<Text>(result), "Header set");
    });

    // Test setHeaders
    auto headers = createHeadersMap({{"Accept", "application/json"}});
    EXPECT_NO_THROW({
        Value result = callHttpMethod("setHeaders", {request_obj, Value(headers)});
        EXPECT_EQ(std::get<Text>(result), "Headers set");
    });

    // Test setTimeout
    EXPECT_NO_THROW({
        Value result = callHttpMethod("setTimeout", {request_obj, Int(30)});
        EXPECT_EQ(std::get<Text>(result), "Timeout set");
    });

    // Test setBody
    EXPECT_NO_THROW({
        Value result = callHttpMethod("setBody", {request_obj, Text(R"({"data": "test"})")});
        EXPECT_EQ(std::get<Text>(result), "Body set");
    });

    // Test setJsonBody
    EXPECT_NO_THROW({
        Value result = callHttpMethod("setJsonBody", {request_obj, Text(R"({"json": "data"})")});
        EXPECT_EQ(std::get<Text>(result), "JSON body set");
    });
}

// Test Query Parameter Methods
TEST_F(HttpClientLibraryTest, QueryParameterMethods) {
    Value request_obj =
        callHttpMethod("createRequest", {Text("GET"), Text("https://api.example.com/data")});

    // Test setQueryParam
    EXPECT_NO_THROW({
        Value result = callHttpMethod("setQueryParam", {request_obj, Text("page"), Text("1")});
        EXPECT_EQ(std::get<Text>(result), "Query parameter set");
    });

    // Test setQueryParams
    auto params = createHeadersMap({{"limit", "10"}, {"sort", "name"}});

    EXPECT_NO_THROW({
        Value result = callHttpMethod("setQueryParams", {request_obj, Value(params)});
        EXPECT_EQ(std::get<Text>(result), "Query parameters set");
    });
}

// Test Error Handling
TEST_F(HttpClientLibraryTest, ErrorHandling) {
    // Test missing URL parameter
    EXPECT_THROW({ callHttpMethod("get", {}); }, std::runtime_error);

    // Test invalid URL type
    EXPECT_THROW({ callHttpMethod("get", {Int(123)}); }, std::runtime_error);

    // Test POST without URL
    EXPECT_THROW({ callHttpMethod("post", {}); }, std::runtime_error);

    // Test request method with insufficient parameters
    EXPECT_THROW({ callHttpMethod("request", {Text("GET")}); }, std::runtime_error);

    // Test authentication methods with insufficient parameters
    EXPECT_THROW({ callHttpMethod("setBasicAuth", {Text("request")}); }, std::runtime_error);

    // Test utility methods with no parameters
    EXPECT_THROW({ callHttpMethod("urlEncode", {}); }, std::runtime_error);

    EXPECT_THROW({ callHttpMethod("urlDecode", {}); }, std::runtime_error);

    EXPECT_THROW({ callHttpMethod("buildUrl", {}); }, std::runtime_error);

    EXPECT_THROW({ callHttpMethod("parseUrl", {}); }, std::runtime_error);
}

// Test Method Existence
TEST_F(HttpClientLibraryTest, MethodExistence) {
    // Verify all HTTP methods exist
    const std::vector<std::string> http_methods = {"get",   "post", "put",    "delete",
                                                   "patch", "head", "options"};

    for (const auto& method : http_methods) {
        EXPECT_TRUE(http_client->hasMethod(method)) << "Missing method: " << method;
    }

    // Verify advanced methods exist
    const std::vector<std::string> advanced_methods = {"request", "requestWithConfig",
                                                       "createRequest"};

    for (const auto& method : advanced_methods) {
        EXPECT_TRUE(http_client->hasMethod(method)) << "Missing method: " << method;
    }

    // Verify configuration methods exist
    const std::vector<std::string> config_methods = {
        "setHeader",  "setHeaders", "setQueryParam", "setQueryParams",
        "setTimeout", "setBody",    "setJsonBody"};

    for (const auto& method : config_methods) {
        EXPECT_TRUE(http_client->hasMethod(method)) << "Missing method: " << method;
    }

    // Verify authentication methods exist
    const std::vector<std::string> auth_methods = {"setBasicAuth", "setBearerToken", "setApiKey"};

    for (const auto& method : auth_methods) {
        EXPECT_TRUE(http_client->hasMethod(method)) << "Missing method: " << method;
    }

    // Verify response methods exist
    const std::vector<std::string> response_methods = {
        "getStatus", "getStatusMessage", "getHeader", "getHeaders",
        "getBody",   "getJson",          "isSuccess", "getError"};

    for (const auto& method : response_methods) {
        EXPECT_TRUE(http_client->hasMethod(method)) << "Missing method: " << method;
    }

    // Verify utility methods exist
    const std::vector<std::string> utility_methods = {"urlEncode", "urlDecode",  "buildUrl",
                                                      "parseUrl",  "uploadFile", "downloadFile"};

    for (const auto& method : utility_methods) {
        EXPECT_TRUE(http_client->hasMethod(method)) << "Missing method: " << method;
    }
}

// Test HTTP Response JSON Integration
TEST_F(HttpClientLibraryTest, ResponseJsonIntegration) {
    std::vector<Value> args = {Text("https://httpbin.org/get")};

    EXPECT_NO_THROW({
        Value response = callHttpMethod("get", args);

        // Test getJson method - should return response body
        Value json_result = callHttpMethod("getJson", {response});
        ASSERT_TRUE(std::holds_alternative<Text>(json_result));

        // The result should be a string (may be empty for failed requests)
        std::string json_str = std::get<Text>(json_result);
        // If not empty, should be valid JSON format
        if (!json_str.empty()) {
            EXPECT_TRUE(json_str[0] == '{' || json_str[0] == '[' || json_str[0] == '"' ||
                        json_str == "null" || json_str == "true" || json_str == "false" ||
                        std::isdigit(json_str[0]) || json_str[0] == '-');  // Valid JSON starts
        }
    });
}

// Performance Test (basic)
TEST_F(HttpClientLibraryTest, BasicPerformance) {
    // Test multiple requests don't cause issues
    std::vector<Value> args = {Text("https://httpbin.org/get")};

    EXPECT_NO_THROW({
        for (int i = 0; i < 3; ++i) {
            Value response = callHttpMethod("get", args);
            ASSERT_TRUE(std::holds_alternative<std::shared_ptr<ObjectInstance>>(response));

            auto response_obj = std::get<std::shared_ptr<ObjectInstance>>(response);
            EXPECT_EQ(response_obj->getName(), "HttpResponse");
        }
    });
}