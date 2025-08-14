#pragma once

#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "Context.hpp"
#include "ObjectInstance.hpp"
#include "Value.hpp"

namespace o2l {

// HTTP Response structure
struct HttpResponse {
    int status_code;
    std::string status_message;
    std::map<std::string, std::string> headers;
    std::string body;
    bool success;
    std::string error_message;

    HttpResponse() : status_code(0), success(false) {}
};

// HTTP Request configuration
struct HttpRequest {
    std::string method;
    std::string url;
    std::map<std::string, std::string> headers;
    std::map<std::string, std::string> query_params;
    std::string body;
    int timeout_seconds;
    bool follow_redirects;
    bool verify_ssl;

    HttpRequest() : timeout_seconds(30), follow_redirects(true), verify_ssl(true) {}
};

class HttpClientLibrary {
   public:
    // Create the http.client module object
    static std::shared_ptr<ObjectInstance> createHttpClientObject();

    // Basic HTTP methods
    static Value nativeGet(const std::vector<Value>& args, Context& context);
    static Value nativePost(const std::vector<Value>& args, Context& context);
    static Value nativePut(const std::vector<Value>& args, Context& context);
    static Value nativeDelete(const std::vector<Value>& args, Context& context);
    static Value nativePatch(const std::vector<Value>& args, Context& context);
    static Value nativeHead(const std::vector<Value>& args, Context& context);
    static Value nativeOptions(const std::vector<Value>& args, Context& context);

    // Advanced request methods
    static Value nativeRequest(const std::vector<Value>& args, Context& context);
    static Value nativeRequestWithConfig(const std::vector<Value>& args, Context& context);

    // Request configuration
    static Value nativeCreateRequest(const std::vector<Value>& args, Context& context);
    static Value nativeSetHeader(const std::vector<Value>& args, Context& context);
    static Value nativeSetHeaders(const std::vector<Value>& args, Context& context);
    static Value nativeSetQueryParam(const std::vector<Value>& args, Context& context);
    static Value nativeSetQueryParams(const std::vector<Value>& args, Context& context);
    static Value nativeSetTimeout(const std::vector<Value>& args, Context& context);
    static Value nativeSetBody(const std::vector<Value>& args, Context& context);
    static Value nativeSetJsonBody(const std::vector<Value>& args, Context& context);

    // Authentication helpers
    static Value nativeSetBasicAuth(const std::vector<Value>& args, Context& context);
    static Value nativeSetBearerToken(const std::vector<Value>& args, Context& context);
    static Value nativeSetApiKey(const std::vector<Value>& args, Context& context);

    // Response handling
    static Value nativeGetStatus(const std::vector<Value>& args, Context& context);
    static Value nativeGetStatusMessage(const std::vector<Value>& args, Context& context);
    static Value nativeGetHeader(const std::vector<Value>& args, Context& context);
    static Value nativeGetHeaders(const std::vector<Value>& args, Context& context);
    static Value nativeGetBody(const std::vector<Value>& args, Context& context);
    static Value nativeGetJson(const std::vector<Value>& args, Context& context);
    static Value nativeIsSuccess(const std::vector<Value>& args, Context& context);
    static Value nativeGetError(const std::vector<Value>& args, Context& context);

    // Utility methods
    static Value nativeUrlEncode(const std::vector<Value>& args, Context& context);
    static Value nativeUrlDecode(const std::vector<Value>& args, Context& context);
    static Value nativeBuildUrl(const std::vector<Value>& args, Context& context);
    static Value nativeParseUrl(const std::vector<Value>& args, Context& context);

    // Upload/Download helpers
    static Value nativeUploadFile(const std::vector<Value>& args, Context& context);
    static Value nativeDownloadFile(const std::vector<Value>& args, Context& context);

   private:
    // Core HTTP execution
    static HttpResponse executeHttpRequest(const HttpRequest& request);

    // Helper methods
    static std::string buildQueryString(const std::map<std::string, std::string>& params);
    static std::map<std::string, std::string> parseHeaders(const std::string& headers_str);
    static std::string urlEncode(const std::string& str);
    static std::string urlDecode(const std::string& str);
    static std::string base64Encode(const std::string& input);

    // Response creation helpers
    static std::shared_ptr<ObjectInstance> createResponseObject(const HttpResponse& response);
    static Value createRequestObject(const HttpRequest& request);

    // Platform-specific HTTP implementation
#ifdef _WIN32
    static HttpResponse executeHttpRequestWindows(const HttpRequest& request);
#elif __APPLE__
    static HttpResponse executeHttpRequestMacOS(const HttpRequest& request);
#else
    static HttpResponse executeHttpRequestLinux(const HttpRequest& request);
#endif

    // libcurl-based implementation (cross-platform fallback)
    static HttpResponse executeHttpRequestCurl(const HttpRequest& request);
    static bool initializeCurl();
    static void cleanupCurl();

    // Error handling
    static std::string getHttpErrorMessage(int status_code);
    static bool isHttpSuccess(int status_code);

    // Content type helpers
    static std::string detectContentType(const std::string& body);
    static std::string detectContentTypeFromFilename(const std::string& filename);
    static bool isJsonContentType(const std::string& content_type);

    // Configuration validation
    static bool validateUrl(const std::string& url);
    static bool validateTimeout(int timeout);

    // Thread safety
    static std::mutex http_mutex;
    static bool curl_initialized;
};

}  // namespace o2l