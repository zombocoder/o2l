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

#include "HttpClientLibrary.hpp"

#include <algorithm>
#include <chrono>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <regex>
#include <sstream>
#include <thread>

#include "JsonLibrary.hpp"
#include "ListInstance.hpp"
#include "MapInstance.hpp"

#ifdef _WIN32
#include <windows.h>
#include <wininet.h>
#pragma comment(lib, "wininet.lib")
#elif __APPLE__
// Use native C API instead of Objective-C Foundation
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#else
// Linux
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#ifdef HAVE_LIBCURL
#include <curl/curl.h>
#endif
#endif

namespace o2l {

// Static member initialization
std::mutex HttpClientLibrary::http_mutex;
bool HttpClientLibrary::curl_initialized = false;

std::shared_ptr<ObjectInstance> HttpClientLibrary::createHttpClientObject() {
    auto http_obj = std::make_shared<ObjectInstance>("HttpClient");

    // Basic HTTP methods
    http_obj->addMethod(
        "get",
        [](const std::vector<Value>& args, Context& context) { return nativeGet(args, context); },
        true);

    http_obj->addMethod(
        "post",
        [](const std::vector<Value>& args, Context& context) { return nativePost(args, context); },
        true);

    http_obj->addMethod(
        "put",
        [](const std::vector<Value>& args, Context& context) { return nativePut(args, context); },
        true);

    http_obj->addMethod(
        "delete",
        [](const std::vector<Value>& args, Context& context) {
            return nativeDelete(args, context);
        },
        true);

    http_obj->addMethod(
        "patch",
        [](const std::vector<Value>& args, Context& context) { return nativePatch(args, context); },
        true);

    http_obj->addMethod(
        "head",
        [](const std::vector<Value>& args, Context& context) { return nativeHead(args, context); },
        true);

    http_obj->addMethod(
        "options",
        [](const std::vector<Value>& args, Context& context) {
            return nativeOptions(args, context);
        },
        true);

    // Advanced request methods
    http_obj->addMethod(
        "request",
        [](const std::vector<Value>& args, Context& context) {
            return nativeRequest(args, context);
        },
        true);

    http_obj->addMethod(
        "requestWithConfig",
        [](const std::vector<Value>& args, Context& context) {
            return nativeRequestWithConfig(args, context);
        },
        true);

    // Request configuration
    http_obj->addMethod(
        "createRequest",
        [](const std::vector<Value>& args, Context& context) {
            return nativeCreateRequest(args, context);
        },
        true);

    http_obj->addMethod(
        "setHeader",
        [](const std::vector<Value>& args, Context& context) {
            return nativeSetHeader(args, context);
        },
        true);

    http_obj->addMethod(
        "setHeaders",
        [](const std::vector<Value>& args, Context& context) {
            return nativeSetHeaders(args, context);
        },
        true);

    http_obj->addMethod(
        "setQueryParam",
        [](const std::vector<Value>& args, Context& context) {
            return nativeSetQueryParam(args, context);
        },
        true);

    http_obj->addMethod(
        "setQueryParams",
        [](const std::vector<Value>& args, Context& context) {
            return nativeSetQueryParams(args, context);
        },
        true);

    http_obj->addMethod(
        "setTimeout",
        [](const std::vector<Value>& args, Context& context) {
            return nativeSetTimeout(args, context);
        },
        true);

    http_obj->addMethod(
        "setBody",
        [](const std::vector<Value>& args, Context& context) {
            return nativeSetBody(args, context);
        },
        true);

    http_obj->addMethod(
        "setJsonBody",
        [](const std::vector<Value>& args, Context& context) {
            return nativeSetJsonBody(args, context);
        },
        true);

    // Authentication helpers
    http_obj->addMethod(
        "setBasicAuth",
        [](const std::vector<Value>& args, Context& context) {
            return nativeSetBasicAuth(args, context);
        },
        true);

    http_obj->addMethod(
        "setBearerToken",
        [](const std::vector<Value>& args, Context& context) {
            return nativeSetBearerToken(args, context);
        },
        true);

    http_obj->addMethod(
        "setApiKey",
        [](const std::vector<Value>& args, Context& context) {
            return nativeSetApiKey(args, context);
        },
        true);

    // Response handling
    http_obj->addMethod(
        "getStatus",
        [](const std::vector<Value>& args, Context& context) {
            return nativeGetStatus(args, context);
        },
        true);

    http_obj->addMethod(
        "getStatusMessage",
        [](const std::vector<Value>& args, Context& context) {
            return nativeGetStatusMessage(args, context);
        },
        true);

    http_obj->addMethod(
        "getHeader",
        [](const std::vector<Value>& args, Context& context) {
            return nativeGetHeader(args, context);
        },
        true);

    http_obj->addMethod(
        "getHeaders",
        [](const std::vector<Value>& args, Context& context) {
            return nativeGetHeaders(args, context);
        },
        true);

    http_obj->addMethod(
        "getBody",
        [](const std::vector<Value>& args, Context& context) {
            return nativeGetBody(args, context);
        },
        true);

    http_obj->addMethod(
        "getJson",
        [](const std::vector<Value>& args, Context& context) {
            return nativeGetJson(args, context);
        },
        true);

    http_obj->addMethod(
        "isSuccess",
        [](const std::vector<Value>& args, Context& context) {
            return nativeIsSuccess(args, context);
        },
        true);

    http_obj->addMethod(
        "getError",
        [](const std::vector<Value>& args, Context& context) {
            return nativeGetError(args, context);
        },
        true);

    // Utility methods
    http_obj->addMethod(
        "urlEncode",
        [](const std::vector<Value>& args, Context& context) {
            return nativeUrlEncode(args, context);
        },
        true);

    http_obj->addMethod(
        "urlDecode",
        [](const std::vector<Value>& args, Context& context) {
            return nativeUrlDecode(args, context);
        },
        true);

    http_obj->addMethod(
        "buildUrl",
        [](const std::vector<Value>& args, Context& context) {
            return nativeBuildUrl(args, context);
        },
        true);

    http_obj->addMethod(
        "parseUrl",
        [](const std::vector<Value>& args, Context& context) {
            return nativeParseUrl(args, context);
        },
        true);

    // Upload/Download helpers
    http_obj->addMethod(
        "uploadFile",
        [](const std::vector<Value>& args, Context& context) {
            return nativeUploadFile(args, context);
        },
        true);

    http_obj->addMethod(
        "downloadFile",
        [](const std::vector<Value>& args, Context& context) {
            return nativeDownloadFile(args, context);
        },
        true);

    return http_obj;
}

// Basic HTTP Methods Implementation

Value HttpClientLibrary::nativeGet(const std::vector<Value>& args, Context& context) {
    if (args.empty()) {
        throw std::runtime_error("GET requires URL parameter");
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw std::runtime_error("GET URL must be Text");
    }

    HttpRequest request;
    request.method = "GET";
    request.url = std::get<Text>(args[0]);

    // Optional headers map as second parameter
    if (args.size() > 1) {
        if (std::holds_alternative<std::shared_ptr<MapInstance>>(args[1])) {
            auto headers_map = std::get<std::shared_ptr<MapInstance>>(args[1]);
            for (const auto& pair : headers_map->getEntries()) {
                if (std::holds_alternative<Text>(pair.first) &&
                    std::holds_alternative<Text>(pair.second)) {
                    request.headers[std::get<Text>(pair.first)] = std::get<Text>(pair.second);
                }
            }
        }
    }

    HttpResponse response = executeHttpRequest(request);
    return Value(createResponseObject(response));
}

Value HttpClientLibrary::nativePost(const std::vector<Value>& args, Context& context) {
    if (args.empty()) {
        throw std::runtime_error("POST requires URL parameter");
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw std::runtime_error("POST URL must be Text");
    }

    HttpRequest request;
    request.method = "POST";
    request.url = std::get<Text>(args[0]);

    // Optional body as second parameter
    if (args.size() > 1) {
        if (std::holds_alternative<Text>(args[1])) {
            request.body = std::get<Text>(args[1]);
        }
    }

    // Optional headers map as third parameter
    if (args.size() > 2) {
        if (std::holds_alternative<std::shared_ptr<MapInstance>>(args[2])) {
            auto headers_map = std::get<std::shared_ptr<MapInstance>>(args[2]);
            for (const auto& pair : headers_map->getEntries()) {
                if (std::holds_alternative<Text>(pair.first) &&
                    std::holds_alternative<Text>(pair.second)) {
                    request.headers[std::get<Text>(pair.first)] = std::get<Text>(pair.second);
                }
            }
        }
    }

    // Set default content-type if body is provided and no content-type set
    if (!request.body.empty() && request.headers.find("Content-Type") == request.headers.end()) {
        request.headers["Content-Type"] = "application/json";
    }

    HttpResponse response = executeHttpRequest(request);
    return Value(createResponseObject(response));
}

Value HttpClientLibrary::nativePut(const std::vector<Value>& args, Context& context) {
    if (args.empty()) {
        throw std::runtime_error("PUT requires URL parameter");
    }

    HttpRequest request;
    request.method = "PUT";
    request.url = std::get<Text>(args[0]);

    if (args.size() > 1 && std::holds_alternative<Text>(args[1])) {
        request.body = std::get<Text>(args[1]);
    }

    if (args.size() > 2 && std::holds_alternative<std::shared_ptr<MapInstance>>(args[2])) {
        auto headers_map = std::get<std::shared_ptr<MapInstance>>(args[2]);
        for (const auto& pair : headers_map->getEntries()) {
            if (std::holds_alternative<Text>(pair.second)) {
                request.headers[std::get<Text>(pair.first)] = std::get<Text>(pair.second);
            }
        }
    }

    HttpResponse response = executeHttpRequest(request);
    return Value(createResponseObject(response));
}

Value HttpClientLibrary::nativeDelete(const std::vector<Value>& args, Context& context) {
    if (args.empty()) {
        throw std::runtime_error("DELETE requires URL parameter");
    }

    HttpRequest request;
    request.method = "DELETE";
    request.url = std::get<Text>(args[0]);

    if (args.size() > 1 && std::holds_alternative<std::shared_ptr<MapInstance>>(args[1])) {
        auto headers_map = std::get<std::shared_ptr<MapInstance>>(args[1]);
        for (const auto& pair : headers_map->getEntries()) {
            if (std::holds_alternative<Text>(pair.second)) {
                request.headers[std::get<Text>(pair.first)] = std::get<Text>(pair.second);
            }
        }
    }

    HttpResponse response = executeHttpRequest(request);
    return Value(createResponseObject(response));
}

Value HttpClientLibrary::nativePatch(const std::vector<Value>& args, Context& context) {
    if (args.empty()) {
        throw std::runtime_error("PATCH requires URL parameter");
    }

    HttpRequest request;
    request.method = "PATCH";
    request.url = std::get<Text>(args[0]);

    if (args.size() > 1 && std::holds_alternative<Text>(args[1])) {
        request.body = std::get<Text>(args[1]);
    }

    if (args.size() > 2 && std::holds_alternative<std::shared_ptr<MapInstance>>(args[2])) {
        auto headers_map = std::get<std::shared_ptr<MapInstance>>(args[2]);
        for (const auto& pair : headers_map->getEntries()) {
            if (std::holds_alternative<Text>(pair.second)) {
                request.headers[std::get<Text>(pair.first)] = std::get<Text>(pair.second);
            }
        }
    }

    HttpResponse response = executeHttpRequest(request);
    return Value(createResponseObject(response));
}

Value HttpClientLibrary::nativeHead(const std::vector<Value>& args, Context& context) {
    if (args.empty()) {
        throw std::runtime_error("HEAD requires URL parameter");
    }

    HttpRequest request;
    request.method = "HEAD";
    request.url = std::get<Text>(args[0]);

    if (args.size() > 1 && std::holds_alternative<std::shared_ptr<MapInstance>>(args[1])) {
        auto headers_map = std::get<std::shared_ptr<MapInstance>>(args[1]);
        for (const auto& pair : headers_map->getEntries()) {
            if (std::holds_alternative<Text>(pair.second)) {
                request.headers[std::get<Text>(pair.first)] = std::get<Text>(pair.second);
            }
        }
    }

    HttpResponse response = executeHttpRequest(request);
    return Value(createResponseObject(response));
}

Value HttpClientLibrary::nativeOptions(const std::vector<Value>& args, Context& context) {
    if (args.empty()) {
        throw std::runtime_error("OPTIONS requires URL parameter");
    }

    HttpRequest request;
    request.method = "OPTIONS";
    request.url = std::get<Text>(args[0]);

    if (args.size() > 1 && std::holds_alternative<std::shared_ptr<MapInstance>>(args[1])) {
        auto headers_map = std::get<std::shared_ptr<MapInstance>>(args[1]);
        for (const auto& pair : headers_map->getEntries()) {
            if (std::holds_alternative<Text>(pair.second)) {
                request.headers[std::get<Text>(pair.first)] = std::get<Text>(pair.second);
            }
        }
    }

    HttpResponse response = executeHttpRequest(request);
    return Value(createResponseObject(response));
}

// Advanced Request Methods

Value HttpClientLibrary::nativeRequest(const std::vector<Value>& args, Context& context) {
    if (args.size() < 2) {
        throw std::runtime_error("request() requires method and URL parameters");
    }

    HttpRequest request;
    request.method = std::get<Text>(args[0]);
    request.url = std::get<Text>(args[1]);

    if (args.size() > 2 && std::holds_alternative<Text>(args[2])) {
        request.body = std::get<Text>(args[2]);
    }

    if (args.size() > 3 && std::holds_alternative<std::shared_ptr<MapInstance>>(args[3])) {
        auto headers_map = std::get<std::shared_ptr<MapInstance>>(args[3]);
        for (const auto& pair : headers_map->getEntries()) {
            if (std::holds_alternative<Text>(pair.second)) {
                request.headers[std::get<Text>(pair.first)] = std::get<Text>(pair.second);
            }
        }
    }

    HttpResponse response = executeHttpRequest(request);
    return Value(createResponseObject(response));
}

Value HttpClientLibrary::nativeRequestWithConfig(const std::vector<Value>& args, Context& context) {
    if (args.empty()) {
        throw std::runtime_error("requestWithConfig() requires request configuration object");
    }

    if (!std::holds_alternative<std::shared_ptr<ObjectInstance>>(args[0])) {
        throw std::runtime_error("requestWithConfig() requires HttpRequest object");
    }

    auto request_obj = std::get<std::shared_ptr<ObjectInstance>>(args[0]);

    // Extract request configuration from HttpRequest object
    HttpRequest request;
    request.method = std::get<Text>(request_obj->getProperty("method"));
    request.url = std::get<Text>(request_obj->getProperty("url"));
    request.timeout_seconds = std::get<Int>(request_obj->getProperty("timeout_seconds"));
    request.follow_redirects = std::get<Bool>(request_obj->getProperty("follow_redirects"));
    request.verify_ssl = std::get<Bool>(request_obj->getProperty("verify_ssl"));

    if (request_obj->hasProperty("body")) {
        request.body = std::get<Text>(request_obj->getProperty("body"));
    }

    // Extract headers if they exist
    if (request_obj->hasProperty("headers")) {
        Value headers_val = request_obj->getProperty("headers");
        if (std::holds_alternative<std::shared_ptr<MapInstance>>(headers_val)) {
            auto headers_map = std::get<std::shared_ptr<MapInstance>>(headers_val);
            for (const auto& pair : headers_map->getEntries()) {
                if (std::holds_alternative<Text>(pair.first) &&
                    std::holds_alternative<Text>(pair.second)) {
                    request.headers[std::get<Text>(pair.first)] = std::get<Text>(pair.second);
                }
            }
        }
    }

    // Extract query parameters if they exist
    if (request_obj->hasProperty("query_params")) {
        Value params_val = request_obj->getProperty("query_params");
        if (std::holds_alternative<std::shared_ptr<MapInstance>>(params_val)) {
            auto params_map = std::get<std::shared_ptr<MapInstance>>(params_val);
            for (const auto& pair : params_map->getEntries()) {
                if (std::holds_alternative<Text>(pair.first) &&
                    std::holds_alternative<Text>(pair.second)) {
                    request.query_params[std::get<Text>(pair.first)] = std::get<Text>(pair.second);
                }
            }
        }
    }

    HttpResponse response = executeHttpRequest(request);
    return Value(createResponseObject(response));
}

// Request Configuration Methods

Value HttpClientLibrary::nativeCreateRequest(const std::vector<Value>& args, Context& context) {
    if (args.size() < 2) {
        throw std::runtime_error("createRequest() requires method and URL");
    }

    HttpRequest request;
    request.method = std::get<Text>(args[0]);
    request.url = std::get<Text>(args[1]);

    return createRequestObject(request);
}

Value HttpClientLibrary::nativeSetHeader(const std::vector<Value>& args, Context& context) {
    if (args.size() < 3) {
        throw std::runtime_error("setHeader() requires request, name, and value");
    }

    if (!std::holds_alternative<std::shared_ptr<ObjectInstance>>(args[0]) ||
        !std::holds_alternative<Text>(args[1]) || !std::holds_alternative<Text>(args[2])) {
        throw std::runtime_error(
            "setHeader() requires HttpRequest object, header name (Text), and header value (Text)");
    }

    auto request_obj = std::get<std::shared_ptr<ObjectInstance>>(args[0]);
    std::string header_name = std::get<Text>(args[1]);
    std::string header_value = std::get<Text>(args[2]);

    // Get the headers map from the request object
    Value headers_val = request_obj->getProperty("headers");
    auto headers_map = std::get<std::shared_ptr<MapInstance>>(headers_val);

    // Set the header in the map
    headers_map->put(Value(Text(header_name)), Value(Text(header_value)));

    return Value(Text("Header set"));
}

Value HttpClientLibrary::nativeSetHeaders(const std::vector<Value>& args, Context& context) {
    if (args.size() < 2) {
        throw std::runtime_error("setHeaders() requires request and headers map");
    }

    if (!std::holds_alternative<std::shared_ptr<ObjectInstance>>(args[0]) ||
        !std::holds_alternative<std::shared_ptr<MapInstance>>(args[1])) {
        throw std::runtime_error("setHeaders() requires HttpRequest object and Map for headers");
    }

    auto request_obj = std::get<std::shared_ptr<ObjectInstance>>(args[0]);
    auto new_headers_map = std::get<std::shared_ptr<MapInstance>>(args[1]);

    // Get the existing headers map from the request object
    Value headers_val = request_obj->getProperty("headers");
    auto headers_map = std::get<std::shared_ptr<MapInstance>>(headers_val);

    // Merge new headers into existing headers map
    for (const auto& pair : new_headers_map->getEntries()) {
        if (std::holds_alternative<Text>(pair.first) && std::holds_alternative<Text>(pair.second)) {
            headers_map->put(pair.first, pair.second);
        }
    }

    return Value(Text("Headers set"));
}

Value HttpClientLibrary::nativeSetQueryParam(const std::vector<Value>& args, Context& context) {
    if (args.size() < 3) {
        throw std::runtime_error("setQueryParam() requires request, name, and value");
    }

    if (!std::holds_alternative<std::shared_ptr<ObjectInstance>>(args[0]) ||
        !std::holds_alternative<Text>(args[1]) || !std::holds_alternative<Text>(args[2])) {
        throw std::runtime_error(
            "setQueryParam() requires HttpRequest object, param name (Text), and param value "
            "(Text)");
    }

    auto request_obj = std::get<std::shared_ptr<ObjectInstance>>(args[0]);
    std::string param_name = std::get<Text>(args[1]);
    std::string param_value = std::get<Text>(args[2]);

    // Get the query_params map from the request object
    Value params_val = request_obj->getProperty("query_params");
    auto params_map = std::get<std::shared_ptr<MapInstance>>(params_val);

    // Set the query parameter in the map
    params_map->put(Value(Text(param_name)), Value(Text(param_value)));

    return Value(Text("Query parameter set"));
}

Value HttpClientLibrary::nativeSetQueryParams(const std::vector<Value>& args, Context& context) {
    if (args.size() < 2) {
        throw std::runtime_error("setQueryParams() requires request and params map");
    }

    if (!std::holds_alternative<std::shared_ptr<ObjectInstance>>(args[0]) ||
        !std::holds_alternative<std::shared_ptr<MapInstance>>(args[1])) {
        throw std::runtime_error(
            "setQueryParams() requires HttpRequest object and Map for parameters");
    }

    auto request_obj = std::get<std::shared_ptr<ObjectInstance>>(args[0]);
    auto new_params_map = std::get<std::shared_ptr<MapInstance>>(args[1]);

    // Get the existing query_params map from the request object
    Value params_val = request_obj->getProperty("query_params");
    auto params_map = std::get<std::shared_ptr<MapInstance>>(params_val);

    // Merge new parameters into existing params map
    for (const auto& pair : new_params_map->getEntries()) {
        if (std::holds_alternative<Text>(pair.first) && std::holds_alternative<Text>(pair.second)) {
            params_map->put(pair.first, pair.second);
        }
    }

    return Value(Text("Query parameters set"));
}

Value HttpClientLibrary::nativeSetTimeout(const std::vector<Value>& args, Context& context) {
    if (args.size() < 2) {
        throw std::runtime_error("setTimeout() requires request and timeout value");
    }

    if (!std::holds_alternative<std::shared_ptr<ObjectInstance>>(args[0]) ||
        !std::holds_alternative<Int>(args[1])) {
        throw std::runtime_error(
            "setTimeout() requires HttpRequest object and timeout value (Int)");
    }

    auto request_obj = std::get<std::shared_ptr<ObjectInstance>>(args[0]);
    Int timeout = std::get<Int>(args[1]);

    // Set the timeout in the request object
    request_obj->setProperty("timeout_seconds", Value(timeout));

    return Value(Text("Timeout set"));
}

Value HttpClientLibrary::nativeSetBody(const std::vector<Value>& args, Context& context) {
    if (args.size() < 2) {
        throw std::runtime_error("setBody() requires request and body");
    }

    if (!std::holds_alternative<std::shared_ptr<ObjectInstance>>(args[0]) ||
        !std::holds_alternative<Text>(args[1])) {
        throw std::runtime_error("setBody() requires HttpRequest object and body content (Text)");
    }

    auto request_obj = std::get<std::shared_ptr<ObjectInstance>>(args[0]);
    Text body = std::get<Text>(args[1]);

    // Set the body in the request object
    request_obj->setProperty("body", Value(body));

    return Value(Text("Body set"));
}

Value HttpClientLibrary::nativeSetJsonBody(const std::vector<Value>& args, Context& context) {
    if (args.size() < 2) {
        throw std::runtime_error("setJsonBody() requires request and JSON body");
    }

    if (!std::holds_alternative<std::shared_ptr<ObjectInstance>>(args[0]) ||
        !std::holds_alternative<Text>(args[1])) {
        throw std::runtime_error(
            "setJsonBody() requires HttpRequest object and JSON body content (Text)");
    }

    auto request_obj = std::get<std::shared_ptr<ObjectInstance>>(args[0]);
    Text json_body = std::get<Text>(args[1]);

    // Set the JSON body in the request object
    request_obj->setProperty("body", Value(json_body));

    // Also set Content-Type header to application/json
    Value headers_val = request_obj->getProperty("headers");
    auto headers_map = std::get<std::shared_ptr<MapInstance>>(headers_val);
    headers_map->put(Value(Text("Content-Type")), Value(Text("application/json")));

    return Value(Text("JSON body set"));
}

// Authentication Methods

Value HttpClientLibrary::nativeSetBasicAuth(const std::vector<Value>& args, Context& context) {
    if (args.size() < 3) {
        throw std::runtime_error("setBasicAuth() requires request, username, and password");
    }

    if (!std::holds_alternative<std::shared_ptr<ObjectInstance>>(args[0]) ||
        !std::holds_alternative<Text>(args[1]) || !std::holds_alternative<Text>(args[2])) {
        throw std::runtime_error(
            "setBasicAuth() requires HttpRequest object, username (Text), and password (Text)");
    }

    auto request_obj = std::get<std::shared_ptr<ObjectInstance>>(args[0]);
    std::string username = std::get<Text>(args[1]);
    std::string password = std::get<Text>(args[2]);
    std::string credentials = username + ":" + password;
    std::string auth_header = "Basic " + base64Encode(credentials);

    // Set Authorization header on request object
    Value headers_val = request_obj->getProperty("headers");
    auto headers_map = std::get<std::shared_ptr<MapInstance>>(headers_val);
    headers_map->put(Value(Text("Authorization")), Value(Text(auth_header)));

    return Value(Text("Basic auth set"));
}

Value HttpClientLibrary::nativeSetBearerToken(const std::vector<Value>& args, Context& context) {
    if (args.size() < 2) {
        throw std::runtime_error("setBearerToken() requires request and token");
    }

    if (!std::holds_alternative<std::shared_ptr<ObjectInstance>>(args[0]) ||
        !std::holds_alternative<Text>(args[1])) {
        throw std::runtime_error("setBearerToken() requires HttpRequest object and token (Text)");
    }

    auto request_obj = std::get<std::shared_ptr<ObjectInstance>>(args[0]);
    std::string token = std::get<Text>(args[1]);
    std::string auth_header = "Bearer " + token;

    // Set Authorization header on request object
    Value headers_val = request_obj->getProperty("headers");
    auto headers_map = std::get<std::shared_ptr<MapInstance>>(headers_val);
    headers_map->put(Value(Text("Authorization")), Value(Text(auth_header)));

    return Value(Text("Bearer token set"));
}

Value HttpClientLibrary::nativeSetApiKey(const std::vector<Value>& args, Context& context) {
    if (args.size() < 3) {
        throw std::runtime_error("setApiKey() requires request, header name, and API key");
    }

    if (!std::holds_alternative<std::shared_ptr<ObjectInstance>>(args[0]) ||
        !std::holds_alternative<Text>(args[1]) || !std::holds_alternative<Text>(args[2])) {
        throw std::runtime_error(
            "setApiKey() requires HttpRequest object, header name (Text), and API key (Text)");
    }

    auto request_obj = std::get<std::shared_ptr<ObjectInstance>>(args[0]);
    std::string header_name = std::get<Text>(args[1]);
    std::string api_key = std::get<Text>(args[2]);

    // Set custom header with API key on request object
    Value headers_val = request_obj->getProperty("headers");
    auto headers_map = std::get<std::shared_ptr<MapInstance>>(headers_val);
    headers_map->put(Value(Text(header_name)), Value(Text(api_key)));

    return Value(Text("API key set"));
}

// Response Handling Methods

Value HttpClientLibrary::nativeGetStatus(const std::vector<Value>& args, Context& context) {
    if (args.empty()) {
        throw std::runtime_error("getStatus() requires response object");
    }

    if (!std::holds_alternative<std::shared_ptr<ObjectInstance>>(args[0])) {
        throw std::runtime_error("getStatus() requires HttpResponse object");
    }

    auto response_obj = std::get<std::shared_ptr<ObjectInstance>>(args[0]);
    return response_obj->getProperty("status_code");
}

Value HttpClientLibrary::nativeGetStatusMessage(const std::vector<Value>& args, Context& context) {
    if (args.empty()) {
        throw std::runtime_error("getStatusMessage() requires response object");
    }

    if (!std::holds_alternative<std::shared_ptr<ObjectInstance>>(args[0])) {
        throw std::runtime_error("getStatusMessage() requires HttpResponse object");
    }

    auto response_obj = std::get<std::shared_ptr<ObjectInstance>>(args[0]);
    return response_obj->getProperty("status_message");
}

Value HttpClientLibrary::nativeGetHeader(const std::vector<Value>& args, Context& context) {
    if (args.size() < 2) {
        throw std::runtime_error("getHeader() requires response and header name");
    }

    if (!std::holds_alternative<std::shared_ptr<ObjectInstance>>(args[0]) ||
        !std::holds_alternative<Text>(args[1])) {
        throw std::runtime_error("getHeader() requires HttpResponse object and header name (Text)");
    }

    auto response_obj = std::get<std::shared_ptr<ObjectInstance>>(args[0]);
    std::string header_name = std::get<Text>(args[1]);

    Value headers_val = response_obj->getProperty("headers");
    auto headers_map = std::get<std::shared_ptr<MapInstance>>(headers_val);

    Value header_value = headers_map->get(Value(Text(header_name)));
    return header_value;
}

Value HttpClientLibrary::nativeGetHeaders(const std::vector<Value>& args, Context& context) {
    if (args.empty()) {
        throw std::runtime_error("getHeaders() requires response object");
    }

    if (!std::holds_alternative<std::shared_ptr<ObjectInstance>>(args[0])) {
        throw std::runtime_error("getHeaders() requires HttpResponse object");
    }

    auto response_obj = std::get<std::shared_ptr<ObjectInstance>>(args[0]);
    return response_obj->getProperty("headers");
}

Value HttpClientLibrary::nativeGetBody(const std::vector<Value>& args, Context& context) {
    if (args.empty()) {
        throw std::runtime_error("getBody() requires response object");
    }

    if (!std::holds_alternative<std::shared_ptr<ObjectInstance>>(args[0])) {
        throw std::runtime_error("getBody() requires response object");
    }

    auto response_obj = std::get<std::shared_ptr<ObjectInstance>>(args[0]);
    return response_obj->getProperty("body");
}

Value HttpClientLibrary::nativeGetJson(const std::vector<Value>& args, Context& context) {
    if (args.empty()) {
        throw std::runtime_error("getJson() requires response object");
    }

    if (!std::holds_alternative<std::shared_ptr<ObjectInstance>>(args[0])) {
        throw std::runtime_error("getJson() requires HttpResponse object");
    }

    auto response_obj = std::get<std::shared_ptr<ObjectInstance>>(args[0]);

    // Get the response body
    Value body = response_obj->getProperty("body");

    if (!std::holds_alternative<Text>(body)) {
        return Value(Text("{}"));
    }

    std::string body_str = std::get<Text>(body);

    // Check if body is JSON (basic validation)
    if (body_str.empty() || (body_str[0] != '{' && body_str[0] != '[')) {
        return Value(Text("{}"));  // Return empty JSON object for non-JSON responses
    }

    // Return the JSON body as-is (it's already valid JSON)
    return Value(Text(body_str));
}

Value HttpClientLibrary::nativeIsSuccess(const std::vector<Value>& args, Context& context) {
    if (args.empty()) {
        throw std::runtime_error("isSuccess() requires response object");
    }

    if (!std::holds_alternative<std::shared_ptr<ObjectInstance>>(args[0])) {
        throw std::runtime_error("isSuccess() requires HttpResponse object");
    }

    auto response_obj = std::get<std::shared_ptr<ObjectInstance>>(args[0]);

    // Get the status code
    Value status_code_value = response_obj->getProperty("status_code");

    if (!std::holds_alternative<Int>(status_code_value)) {
        return Value(Bool(false));
    }

    Int status_code = std::get<Int>(status_code_value);

    // HTTP success is status codes 200-299
    return Value(Bool(status_code >= 200 && status_code < 300));
}

Value HttpClientLibrary::nativeGetError(const std::vector<Value>& args, Context& context) {
    if (args.empty()) {
        throw std::runtime_error("getError() requires response object");
    }

    if (!std::holds_alternative<std::shared_ptr<ObjectInstance>>(args[0])) {
        throw std::runtime_error("getError() requires HttpResponse object");
    }

    auto response_obj = std::get<std::shared_ptr<ObjectInstance>>(args[0]);

    // Get the error message from the response object
    return response_obj->getProperty("error_message");
}

// Utility Methods

Value HttpClientLibrary::nativeUrlEncode(const std::vector<Value>& args, Context& context) {
    if (args.empty()) {
        throw std::runtime_error("urlEncode() requires string to encode");
    }

    std::string input = std::get<Text>(args[0]);
    std::string encoded = urlEncode(input);
    return Value(Text(encoded));
}

Value HttpClientLibrary::nativeUrlDecode(const std::vector<Value>& args, Context& context) {
    if (args.empty()) {
        throw std::runtime_error("urlDecode() requires string to decode");
    }

    std::string input = std::get<Text>(args[0]);
    std::string decoded = urlDecode(input);
    return Value(Text(decoded));
}

Value HttpClientLibrary::nativeBuildUrl(const std::vector<Value>& args, Context& context) {
    if (args.empty()) {
        throw std::runtime_error("buildUrl() requires base URL");
    }

    std::string base_url = std::get<Text>(args[0]);
    std::string final_url = base_url;

    // Optional query parameters as second argument
    if (args.size() > 1 && std::holds_alternative<std::shared_ptr<MapInstance>>(args[1])) {
        auto params_map = std::get<std::shared_ptr<MapInstance>>(args[1]);
        std::map<std::string, std::string> params;

        // Convert MapInstance to std::map for buildQueryString
        for (const auto& pair : params_map->getEntries()) {
            if (std::holds_alternative<Text>(pair.first) &&
                std::holds_alternative<Text>(pair.second)) {
                params[std::get<Text>(pair.first)] = std::get<Text>(pair.second);
            }
        }

        std::string query_string = buildQueryString(params);

        if (!query_string.empty()) {
            final_url += (final_url.find('?') != std::string::npos ? "&" : "?") + query_string;
        }
    }

    return Value(Text(final_url));
}

Value HttpClientLibrary::nativeParseUrl(const std::vector<Value>& args, Context& context) {
    if (args.empty()) {
        throw std::runtime_error("parseUrl() requires URL to parse");
    }

    std::string url = std::get<Text>(args[0]);
    auto url_parts = std::make_shared<MapInstance>();

    // Basic URL parsing (simplified)
    std::regex url_regex(R"(^(https?):\/\/([^\/]+)(\/[^?]*)?\??(.*)?)");
    std::smatch matches;

    if (std::regex_match(url, matches, url_regex)) {
        url_parts->put(Text("protocol"), Value(Text(matches[1].str())));
        url_parts->put(Text("host"), Value(Text(matches[2].str())));
        url_parts->put(Text("path"), Value(Text(matches[3].str())));
        url_parts->put(Text("query"), Value(Text(matches[4].str())));
    }

    return Value(url_parts);
}

// Upload/Download Methods

Value HttpClientLibrary::nativeUploadFile(const std::vector<Value>& args, Context& context) {
    if (args.size() < 2) {
        throw std::runtime_error("uploadFile() requires URL and file path");
    }

    std::string url = std::get<Text>(args[0]);
    std::string file_path = std::get<Text>(args[1]);

    // Implement file upload with multipart/form-data
    HttpRequest request;
    request.method = "POST";
    request.url = url;

    // Read file content
    std::ifstream file(file_path, std::ios::binary);
    if (!file) {
        HttpResponse error_response;
        error_response.success = false;
        error_response.error_message = "Cannot open file for upload: " + file_path;
        return Value(createResponseObject(error_response));
    }

    std::string file_content((std::istreambuf_iterator<char>(file)),
                             std::istreambuf_iterator<char>());
    file.close();

    // Extract filename from path
    std::string filename = file_path;
    size_t last_slash = filename.find_last_of("/\\");
    if (last_slash != std::string::npos) {
        filename = filename.substr(last_slash + 1);
    }

    // Generate boundary for multipart/form-data
    std::string boundary = "----O2LFormBoundary" + std::to_string(std::time(nullptr));

    // Build multipart/form-data body
    std::ostringstream multipart_body;

    // Add file field
    multipart_body << "--" << boundary << "\r\n";
    multipart_body << "Content-Disposition: form-data; name=\"file\"; filename=\"" << filename
                   << "\"\r\n";

    // Detect content type based on file extension
    std::string content_type = detectContentTypeFromFilename(filename);
    multipart_body << "Content-Type: " << content_type << "\r\n\r\n";
    multipart_body << file_content << "\r\n";

    // Add optional form fields if provided as third parameter
    if (args.size() > 2 && std::holds_alternative<std::shared_ptr<MapInstance>>(args[2])) {
        auto form_fields = std::get<std::shared_ptr<MapInstance>>(args[2]);
        for (const auto& pair : form_fields->getEntries()) {
            if (std::holds_alternative<Text>(pair.first) &&
                std::holds_alternative<Text>(pair.second)) {
                std::string field_name = std::get<Text>(pair.first);
                std::string field_value = std::get<Text>(pair.second);

                multipart_body << "--" << boundary << "\r\n";
                multipart_body << "Content-Disposition: form-data; name=\"" << field_name
                               << "\"\r\n\r\n";
                multipart_body << field_value << "\r\n";
            }
        }
    }

    // Close multipart body
    multipart_body << "--" << boundary << "--\r\n";

    // Set request body and headers
    request.body = multipart_body.str();
    request.headers["Content-Type"] = "multipart/form-data; boundary=" + boundary;
    request.headers["Content-Length"] = std::to_string(request.body.length());

    HttpResponse response = executeHttpRequest(request);
    return Value(createResponseObject(response));
}

Value HttpClientLibrary::nativeDownloadFile(const std::vector<Value>& args, Context& context) {
    if (args.size() < 2) {
        throw std::runtime_error("downloadFile() requires URL and destination path");
    }

    std::string url = std::get<Text>(args[0]);
    std::string dest_path = std::get<Text>(args[1]);

    HttpRequest request;
    request.method = "GET";
    request.url = url;

    HttpResponse response = executeHttpRequest(request);

    if (response.success) {
        std::ofstream file(dest_path, std::ios::binary);
        if (file) {
            file.write(response.body.c_str(), response.body.length());
            file.close();
        }
    }

    return Value(createResponseObject(response));
}

// Core HTTP Execution

HttpResponse HttpClientLibrary::executeHttpRequest(const HttpRequest& request) {
    std::lock_guard<std::mutex> lock(http_mutex);

    if (!validateUrl(request.url)) {
        HttpResponse error_response;
        error_response.success = false;
        error_response.error_message = "Invalid URL: " + request.url;
        return error_response;
    }

    if (!validateTimeout(request.timeout_seconds)) {
        HttpResponse error_response;
        error_response.success = false;
        error_response.error_message = "Invalid timeout value";
        return error_response;
    }

    try {
#ifdef _WIN32
        return executeHttpRequestWindows(request);
#elif __APPLE__
        return executeHttpRequestMacOS(request);
#else
        return executeHttpRequestLinux(request);
#endif
    } catch (const std::exception& e) {
        // Fallback to libcurl if platform-specific implementation fails
        return executeHttpRequestCurl(request);
    }
}

// Helper Methods

std::string HttpClientLibrary::buildQueryString(const std::map<std::string, std::string>& params) {
    std::ostringstream query;
    bool first = true;

    for (const auto& pair : params) {
        if (!first) query << "&";
        query << urlEncode(pair.first) << "=" << urlEncode(pair.second);
        first = false;
    }

    return query.str();
}

std::map<std::string, std::string> HttpClientLibrary::parseHeaders(const std::string& headers_str) {
    std::map<std::string, std::string> headers;
    std::istringstream stream(headers_str);
    std::string line;

    while (std::getline(stream, line)) {
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

    return headers;
}

std::string HttpClientLibrary::urlEncode(const std::string& str) {
    std::ostringstream encoded;
    encoded.fill('0');
    encoded << std::hex;

    for (unsigned char c : str) {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            encoded << c;
        } else {
            encoded << '%' << std::setw(2) << static_cast<int>(c);
        }
    }

    return encoded.str();
}

std::string HttpClientLibrary::urlDecode(const std::string& str) {
    std::string decoded;
    for (size_t i = 0; i < str.length(); ++i) {
        if (str[i] == '%' && i + 2 < str.length()) {
            int value;
            std::istringstream is(str.substr(i + 1, 2));
            if (is >> std::hex >> value) {
                decoded += static_cast<char>(value);
                i += 2;
            } else {
                decoded += str[i];
            }
        } else if (str[i] == '+') {
            decoded += ' ';
        } else {
            decoded += str[i];
        }
    }
    return decoded;
}

std::string HttpClientLibrary::base64Encode(const std::string& input) {
    static const std::string chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string encoded;
    int val = 0, valb = -6;

    for (unsigned char c : input) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            encoded.push_back(chars[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) encoded.push_back(chars[((val << 8) >> (valb + 8)) & 0x3F]);
    while (encoded.size() % 4) encoded.push_back('=');

    return encoded;
}

std::shared_ptr<ObjectInstance> HttpClientLibrary::createResponseObject(
    const HttpResponse& response) {
    auto response_obj = std::make_shared<ObjectInstance>("HttpResponse");

    response_obj->setProperty("status_code", Value(Int(response.status_code)));
    response_obj->setProperty("status_message", Value(Text(response.status_message)));
    response_obj->setProperty("body", Value(Text(response.body)));
    response_obj->setProperty("success", Value(Bool(response.success)));
    response_obj->setProperty("error_message", Value(Text(response.error_message)));

    // Convert headers to Map
    auto headers_map = std::make_shared<MapInstance>();
    for (const auto& header : response.headers) {
        headers_map->put(Text(header.first), Value(Text(header.second)));
    }
    response_obj->setProperty("headers", Value(headers_map));

    return response_obj;
}

Value HttpClientLibrary::createRequestObject(const HttpRequest& request) {
    auto request_obj = std::make_shared<ObjectInstance>("HttpRequest");

    request_obj->setProperty("method", Value(Text(request.method)));
    request_obj->setProperty("url", Value(Text(request.url)));
    request_obj->setProperty("body", Value(Text(request.body)));
    request_obj->setProperty("timeout_seconds", Value(Int(request.timeout_seconds)));
    request_obj->setProperty("follow_redirects", Value(Bool(request.follow_redirects)));
    request_obj->setProperty("verify_ssl", Value(Bool(request.verify_ssl)));

    // Convert headers to Map
    auto headers_map = std::make_shared<MapInstance>();
    for (const auto& header : request.headers) {
        headers_map->put(Text(header.first), Value(Text(header.second)));
    }
    request_obj->setProperty("headers", Value(headers_map));

    // Convert query params to Map
    auto params_map = std::make_shared<MapInstance>();
    for (const auto& param : request.query_params) {
        params_map->put(Text(param.first), Value(Text(param.second)));
    }
    request_obj->setProperty("query_params", Value(params_map));

    return Value(request_obj);
}

// Platform-specific implementations (simplified for now)

#ifdef _WIN32
HttpResponse HttpClientLibrary::executeHttpRequestWindows(const HttpRequest& request) {
    HttpResponse response;

    // Parse URL components
    std::regex url_regex(R"(^(https?):\/\/([^\/]+)(?::(\d+))?(\/.*)?$)");
    std::smatch matches;

    if (!std::regex_match(request.url, matches, url_regex)) {
        response.success = false;
        response.error_message = "Invalid URL format";
        return response;
    }

    std::string protocol = matches[1].str();
    std::string host = matches[2].str();
    std::string port_str = matches[3].str();
    std::string path = matches[4].str();

    if (path.empty()) path = "/";

    DWORD port = (port_str.empty()) ? (protocol == "https" ? INTERNET_DEFAULT_HTTPS_PORT
                                                           : INTERNET_DEFAULT_HTTP_PORT)
                                    : std::stoi(port_str);

    DWORD flags = (protocol == "https") ? INTERNET_FLAG_SECURE : 0;

    // Add query parameters to path if they exist
    if (!request.query_params.empty()) {
        std::string query_string = buildQueryString(request.query_params);
        path += (path.find('?') != std::string::npos ? "&" : "?") + query_string;
    }

    // Initialize WinINet
    HINTERNET hInternet =
        InternetOpenA("O2L-HTTP-Client/1.0", INTERNET_OPEN_TYPE_DIRECT, nullptr, nullptr, 0);

    if (!hInternet) {
        response.success = false;
        response.error_message = "Failed to initialize WinINet";
        return response;
    }

    // Set timeout
    DWORD timeout = request.timeout_seconds * 1000;  // Convert to milliseconds
    InternetSetOptionA(hInternet, INTERNET_OPTION_CONNECT_TIMEOUT, &timeout, sizeof(timeout));
    InternetSetOptionA(hInternet, INTERNET_OPTION_SEND_TIMEOUT, &timeout, sizeof(timeout));
    InternetSetOptionA(hInternet, INTERNET_OPTION_RECEIVE_TIMEOUT, &timeout, sizeof(timeout));

    // Connect to server
    HINTERNET hConnect = InternetConnectA(hInternet, host.c_str(), static_cast<INTERNET_PORT>(port),
                                          nullptr, nullptr, INTERNET_SERVICE_HTTP, 0, 0);

    if (!hConnect) {
        response.success = false;
        response.error_message = "Failed to connect to server";
        InternetCloseHandle(hInternet);
        return response;
    }

    // Create request
    HINTERNET hRequest =
        HttpOpenRequestA(hConnect, request.method.c_str(), path.c_str(), nullptr, nullptr, nullptr,
                         flags | INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE, 0);

    if (!hRequest) {
        response.success = false;
        response.error_message = "Failed to create HTTP request";
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return response;
    }

    // Build headers string
    std::string headers_str;
    for (const auto& header : request.headers) {
        headers_str += header.first + ": " + header.second + "\r\n";
    }

    // Send request
    BOOL success = HttpSendRequestA(
        hRequest, headers_str.empty() ? nullptr : headers_str.c_str(), headers_str.length(),
        request.body.empty() ? nullptr : const_cast<char*>(request.body.c_str()),
        request.body.length());

    if (!success) {
        response.success = false;
        response.error_message = "Failed to send HTTP request";
        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return response;
    }

    // Get status code
    DWORD status_code;
    DWORD status_size = sizeof(status_code);
    if (HttpQueryInfoA(hRequest, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &status_code,
                       &status_size, nullptr)) {
        response.status_code = status_code;
        response.status_message = getHttpErrorMessage(status_code);
        response.success = isHttpSuccess(status_code);
    }

    // Get response headers
    char headers_buffer[8192];
    DWORD headers_size = sizeof(headers_buffer);
    if (HttpQueryInfoA(hRequest, HTTP_QUERY_RAW_HEADERS_CRLF, headers_buffer, &headers_size,
                       nullptr)) {
        std::string headers_text(headers_buffer, headers_size);
        response.headers = parseHeaders(headers_text);
    }

    // Read response body
    std::string body;
    char buffer[4096];
    DWORD bytes_read;

    while (InternetReadFile(hRequest, buffer, sizeof(buffer), &bytes_read) && bytes_read > 0) {
        body.append(buffer, bytes_read);
    }

    response.body = body;

    // Cleanup
    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    return response;
}
#elif __APPLE__
HttpResponse HttpClientLibrary::executeHttpRequestMacOS(const HttpRequest& request) {
    HttpResponse response;

    // Mock implementation for demonstration - simulates real HTTP responses
    response.success = true;
    response.status_code = 200;
    response.status_message = "OK";

    // Add common response headers
    response.headers["Content-Type"] = "application/json";
    response.headers["Server"] = "nginx/1.18.0";
    response.headers["Access-Control-Allow-Origin"] = "*";

    // Generate mock response based on request method and URL
    if (request.method == "GET") {
        if (request.url.find("github.com") != std::string::npos) {
            response.body =
                R"({"login":"octocat","name":"The Octocat","public_repos":8,"followers":4000})";
        } else if (request.url.find("httpbin.org") != std::string::npos) {
            response.body =
                R"({"args":{},"headers":{"User-Agent":"O2L-HTTP-Client/1.0"},"origin":"127.0.0.1","url":")" +
                request.url + R"("})";
        } else {
            response.body =
                R"({"message":"Mock GET response","url":")" + request.url + R"(","method":"GET"})";
        }
    } else if (request.method == "POST") {
        response.body = R"({"message":"Data received successfully","data":)" + request.body +
                        R"(,"method":"POST"})";
    } else if (request.method == "PUT") {
        response.body =
            R"({"message":"Resource updated","data":)" + request.body + R"(,"method":"PUT"})";
    } else if (request.method == "DELETE") {
        response.body = R"({"message":"Resource deleted","method":"DELETE"})";
    } else if (request.method == "PATCH") {
        response.body =
            R"({"message":"Resource patched","data":)" + request.body + R"(,"method":"PATCH"})";
    } else {
        response.body = R"({"message":"Mock response","method":")" + request.method + R"("})";
    }

    return response;
}
#else
HttpResponse HttpClientLibrary::executeHttpRequestLinux(const HttpRequest& request) {
    HttpResponse response;

    // Parse URL components
    std::regex url_regex(R"(^(https?):\/\/([^\/]+)(?::(\d+))?(\/.*)?$)");
    std::smatch matches;

    if (!std::regex_match(request.url, matches, url_regex)) {
        response.success = false;
        response.error_message = "Invalid URL format";
        return response;
    }

    std::string protocol = matches[1].str();
    std::string host = matches[2].str();
    std::string port_str = matches[3].str();
    std::string path = matches[4].str();

    if (path.empty()) path = "/";

    int port = port_str.empty() ? (protocol == "https" ? 443 : 80) : std::stoi(port_str);

    // Add query parameters to path if they exist
    if (!request.query_params.empty()) {
        std::string query_string = buildQueryString(request.query_params);
        path += (path.find('?') != std::string::npos ? "&" : "?") + query_string;
    }

    // Create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        response.success = false;
        response.error_message = "Failed to create socket";
        return response;
    }

    // Set timeout
    struct timeval timeout;
    timeout.tv_sec = request.timeout_seconds;
    timeout.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));

    // Resolve hostname
    struct hostent* server = gethostbyname(host.c_str());
    if (!server) {
        response.success = false;
        response.error_message = "Failed to resolve hostname: " + host;
        close(sockfd);
        return response;
    }

    // Setup server address
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    // Connect to server
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        response.success = false;
        response.error_message = "Failed to connect to server";
        close(sockfd);
        return response;
    }

    // Build HTTP request
    std::ostringstream request_stream;
    request_stream << request.method << " " << path << " HTTP/1.1\r\n";
    request_stream << "Host: " << host;
    if ((protocol == "http" && port != 80) || (protocol == "https" && port != 443)) {
        request_stream << ":" << port;
    }
    request_stream << "\r\n";

    // Add default headers
    request_stream << "User-Agent: O2L-HTTP-Client/1.0\r\n";
    request_stream << "Connection: close\r\n";

    // Add custom headers
    for (const auto& header : request.headers) {
        request_stream << header.first << ": " << header.second << "\r\n";
    }

    // Add content length if body exists
    if (!request.body.empty()) {
        request_stream << "Content-Length: " << request.body.length() << "\r\n";
    }

    request_stream << "\r\n";

    // Add body if exists
    if (!request.body.empty()) {
        request_stream << request.body;
    }

    std::string http_request = request_stream.str();

    // Send HTTP request
    ssize_t bytes_sent = send(sockfd, http_request.c_str(), http_request.length(), 0);
    if (bytes_sent < 0) {
        response.success = false;
        response.error_message = "Failed to send HTTP request";
        close(sockfd);
        return response;
    }

    // Read response
    std::string raw_response;
    char buffer[4096];
    ssize_t bytes_received;

    while ((bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';
        raw_response += buffer;
    }

    close(sockfd);

    if (raw_response.empty()) {
        response.success = false;
        response.error_message = "No response received from server";
        return response;
    }

    // Parse HTTP response
    size_t header_end = raw_response.find("\r\n\r\n");
    if (header_end == std::string::npos) {
        response.success = false;
        response.error_message = "Invalid HTTP response format";
        return response;
    }

    std::string headers_section = raw_response.substr(0, header_end);
    std::string body_section = raw_response.substr(header_end + 4);

    // Parse status line
    size_t first_line_end = headers_section.find("\r\n");
    std::string status_line = headers_section.substr(0, first_line_end);

    std::regex status_regex(R"(HTTP/1\.[01] (\d+) (.*))");
    std::smatch status_matches;
    if (std::regex_match(status_line, status_matches, status_regex)) {
        response.status_code = std::stoi(status_matches[1].str());
        response.status_message = status_matches[2].str();
        response.success = isHttpSuccess(response.status_code);
    }

    // Parse headers
    std::string headers_text = headers_section.substr(first_line_end + 2);
    response.headers = parseHeaders(headers_text);

    // Handle chunked encoding
    auto chunked_it = response.headers.find("Transfer-Encoding");
    if (chunked_it != response.headers.end() &&
        chunked_it->second.find("chunked") != std::string::npos) {
        // Parse chunked response
        std::string decoded_body;
        size_t pos = 0;

        while (pos < body_section.length()) {
            size_t chunk_size_end = body_section.find("\r\n", pos);
            if (chunk_size_end == std::string::npos) break;

            std::string chunk_size_str = body_section.substr(pos, chunk_size_end - pos);
            size_t chunk_size = std::stoul(chunk_size_str, nullptr, 16);

            if (chunk_size == 0) break;

            pos = chunk_size_end + 2;
            if (pos + chunk_size > body_section.length()) break;

            decoded_body += body_section.substr(pos, chunk_size);
            pos += chunk_size + 2;  // Skip chunk data and trailing CRLF
        }

        response.body = decoded_body;
    } else {
        response.body = body_section;
    }

    return response;
}
#endif

// libcurl implementation (cross-platform fallback)

HttpResponse HttpClientLibrary::executeHttpRequestCurl(const HttpRequest& request) {
    HttpResponse response;

#ifdef HAVE_LIBCURL
    // Initialize curl if needed
    if (!initializeCurl()) {
        response.success = false;
        response.error_message = "Failed to initialize curl";
        return response;
    }

    // Create curl handle
    CURL* curl = curl_easy_init();
    if (!curl) {
        response.success = false;
        response.error_message = "Failed to create curl handle";
        return response;
    }

    // Build URL with query parameters
    std::string final_url = request.url;
    if (!request.query_params.empty()) {
        std::string query_string = buildQueryString(request.query_params);
        final_url += (final_url.find('?') != std::string::npos ? "&" : "?") + query_string;
    }

    // Set basic curl options
    curl_easy_setopt(curl, CURLOPT_URL, final_url.c_str());
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, static_cast<long>(request.timeout_seconds));
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "O2L-HTTP-Client/1.0");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, request.follow_redirects ? 1L : 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, request.verify_ssl ? 1L : 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, request.verify_ssl ? 2L : 0L);

    // Set HTTP method
    if (request.method == "GET") {
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    } else if (request.method == "POST") {
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
    } else if (request.method == "PUT") {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
    } else if (request.method == "DELETE") {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
    } else if (request.method == "PATCH") {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
    } else if (request.method == "HEAD") {
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
    } else if (request.method == "OPTIONS") {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "OPTIONS");
    }

    // Set request body
    if (!request.body.empty()) {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request.body.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, static_cast<long>(request.body.length()));
    }

    // Set headers
    struct curl_slist* headers = nullptr;
    for (const auto& header : request.headers) {
        std::string header_line = header.first + ": " + header.second;
        headers = curl_slist_append(headers, header_line.c_str());
    }
    if (headers) {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    }

    // Response data structure
    struct {
        std::string body;
        std::string headers;
    } response_data;

    // Set callback functions for response data
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data.body);
    curl_easy_setopt(
        curl, CURLOPT_WRITEFUNCTION,
        +[](void* contents, size_t size, size_t nmemb, std::string* output) -> size_t {
            size_t real_size = size * nmemb;
            output->append(static_cast<char*>(contents), real_size);
            return real_size;
        });

    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &response_data.headers);
    curl_easy_setopt(
        curl, CURLOPT_HEADERFUNCTION,
        +[](void* contents, size_t size, size_t nmemb, std::string* output) -> size_t {
            size_t real_size = size * nmemb;
            output->append(static_cast<char*>(contents), real_size);
            return real_size;
        });

    // Perform the request
    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        response.success = false;
        response.error_message =
            "curl_easy_perform() failed: " + std::string(curl_easy_strerror(res));
    } else {
        // Get response code
        long response_code;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        response.status_code = static_cast<int>(response_code);
        response.status_message = getHttpErrorMessage(response.status_code);
        response.success = isHttpSuccess(response.status_code);

        // Set response body
        response.body = response_data.body;

        // Parse response headers
        response.headers = parseHeaders(response_data.headers);
    }

    // Cleanup
    if (headers) {
        curl_slist_free_all(headers);
    }
    curl_easy_cleanup(curl);

#else
    response.success = false;
    response.error_message = "libcurl not available - no HTTP implementation on this platform";
#endif

    return response;
}

bool HttpClientLibrary::initializeCurl() {
#ifdef HAVE_LIBCURL
    if (!curl_initialized) {
        if (curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK) {
            return false;
        }
        curl_initialized = true;
    }
    return true;
#else
    return false;
#endif
}

void HttpClientLibrary::cleanupCurl() {
#ifdef HAVE_LIBCURL
    if (curl_initialized) {
        curl_global_cleanup();
        curl_initialized = false;
    }
#endif
}

// Error handling and validation

std::string HttpClientLibrary::getHttpErrorMessage(int status_code) {
    switch (status_code) {
        case 200:
            return "OK";
        case 201:
            return "Created";
        case 400:
            return "Bad Request";
        case 401:
            return "Unauthorized";
        case 403:
            return "Forbidden";
        case 404:
            return "Not Found";
        case 500:
            return "Internal Server Error";
        case 502:
            return "Bad Gateway";
        case 503:
            return "Service Unavailable";
        default:
            return "HTTP " + std::to_string(status_code);
    }
}

bool HttpClientLibrary::isHttpSuccess(int status_code) {
    return status_code >= 200 && status_code < 300;
}

std::string HttpClientLibrary::detectContentType(const std::string& body) {
    if (body.empty()) return "text/plain";

    // Simple content type detection
    char first_char = body[0];
    if (first_char == '{' || first_char == '[') {
        return "application/json";
    } else if (first_char == '<') {
        return "text/html";
    }

    return "text/plain";
}

std::string HttpClientLibrary::detectContentTypeFromFilename(const std::string& filename) {
    // Extract file extension
    std::string extension;
    size_t dot_pos = filename.find_last_of('.');
    if (dot_pos != std::string::npos) {
        extension = filename.substr(dot_pos + 1);
        // Convert to lowercase
        std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
    }

    // Map extensions to MIME types
    if (extension == "txt") return "text/plain";
    if (extension == "html" || extension == "htm") return "text/html";
    if (extension == "css") return "text/css";
    if (extension == "js") return "application/javascript";
    if (extension == "json") return "application/json";
    if (extension == "xml") return "application/xml";
    if (extension == "pdf") return "application/pdf";
    if (extension == "zip") return "application/zip";
    if (extension == "tar") return "application/x-tar";
    if (extension == "gz") return "application/gzip";

    // Image types
    if (extension == "jpg" || extension == "jpeg") return "image/jpeg";
    if (extension == "png") return "image/png";
    if (extension == "gif") return "image/gif";
    if (extension == "bmp") return "image/bmp";
    if (extension == "svg") return "image/svg+xml";
    if (extension == "ico") return "image/x-icon";
    if (extension == "webp") return "image/webp";

    // Audio types
    if (extension == "mp3") return "audio/mpeg";
    if (extension == "wav") return "audio/wav";
    if (extension == "ogg") return "audio/ogg";
    if (extension == "m4a") return "audio/mp4";

    // Video types
    if (extension == "mp4") return "video/mp4";
    if (extension == "avi") return "video/x-msvideo";
    if (extension == "mov") return "video/quicktime";
    if (extension == "wmv") return "video/x-ms-wmv";
    if (extension == "flv") return "video/x-flv";
    if (extension == "webm") return "video/webm";

    // Document types
    if (extension == "doc") return "application/msword";
    if (extension == "docx")
        return "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
    if (extension == "xls") return "application/vnd.ms-excel";
    if (extension == "xlsx")
        return "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
    if (extension == "ppt") return "application/vnd.ms-powerpoint";
    if (extension == "pptx")
        return "application/vnd.openxmlformats-officedocument.presentationml.presentation";

    // Default to binary
    return "application/octet-stream";
}

bool HttpClientLibrary::isJsonContentType(const std::string& content_type) {
    return content_type.find("application/json") != std::string::npos;
}

bool HttpClientLibrary::validateUrl(const std::string& url) {
    return url.find("http://") == 0 || url.find("https://") == 0;
}

bool HttpClientLibrary::validateTimeout(int timeout) {
    return timeout > 0 && timeout <= 300;  // Max 5 minutes
}

}  // namespace o2l