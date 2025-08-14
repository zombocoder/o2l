#pragma once

#include <map>
#include <memory>
#include <regex>
#include <string>
#include <vector>

#include "Context.hpp"
#include "ObjectInstance.hpp"
#include "Value.hpp"

namespace o2l {

// URL component structure for parsing
struct UrlComponents {
    std::string scheme;
    std::string username;
    std::string password;
    std::string host;
    std::string port;
    std::string path;
    std::string query;
    std::string fragment;
    bool valid = false;
};

class UrlLibrary {
   public:
    // Create the url module object
    static std::shared_ptr<ObjectInstance> createUrlObject();

    // URL parsing methods
    static Value nativeParse(const std::vector<Value>& args, Context& context);
    static Value nativeIsValid(const std::vector<Value>& args, Context& context);
    static Value nativeGetScheme(const std::vector<Value>& args, Context& context);
    static Value nativeGetHost(const std::vector<Value>& args, Context& context);
    static Value nativeGetPort(const std::vector<Value>& args, Context& context);
    static Value nativeGetPath(const std::vector<Value>& args, Context& context);
    static Value nativeGetQuery(const std::vector<Value>& args, Context& context);
    static Value nativeGetFragment(const std::vector<Value>& args, Context& context);

    // URL construction methods
    static Value nativeCreate(const std::vector<Value>& args, Context& context);
    static Value nativeSetScheme(const std::vector<Value>& args, Context& context);
    static Value nativeSetHost(const std::vector<Value>& args, Context& context);
    static Value nativeSetPort(const std::vector<Value>& args, Context& context);
    static Value nativeSetPath(const std::vector<Value>& args, Context& context);
    static Value nativeSetQuery(const std::vector<Value>& args, Context& context);
    static Value nativeSetFragment(const std::vector<Value>& args, Context& context);

    // Query parameter methods
    static Value nativeGetParam(const std::vector<Value>& args, Context& context);
    static Value nativeSetParam(const std::vector<Value>& args, Context& context);
    static Value nativeRemoveParam(const std::vector<Value>& args, Context& context);
    static Value nativeGetParams(const std::vector<Value>& args, Context& context);

    // URL manipulation methods
    static Value nativeNormalize(const std::vector<Value>& args, Context& context);
    static Value nativeResolve(const std::vector<Value>& args, Context& context);
    static Value nativeJoin(const std::vector<Value>& args, Context& context);
    static Value nativeEncode(const std::vector<Value>& args, Context& context);
    static Value nativeDecode(const std::vector<Value>& args, Context& context);

    // Utility methods
    static Value nativeGetDomain(const std::vector<Value>& args, Context& context);
    static Value nativeGetOrigin(const std::vector<Value>& args, Context& context);
    static Value nativeIsAbsolute(const std::vector<Value>& args, Context& context);
    static Value nativeIsRelative(const std::vector<Value>& args, Context& context);

   private:
    // Helper methods
    static UrlComponents parseUrl(const std::string& url);
    static std::string buildUrl(const UrlComponents& components);
    static std::map<std::string, std::string> parseQueryString(const std::string& query);
    static std::string buildQueryString(const std::map<std::string, std::string>& params);
    static std::string urlEncode(const std::string& value);
    static std::string urlDecode(const std::string& value);
    static std::string normalizePath(const std::string& path);
    static bool isValidScheme(const std::string& scheme);
    static bool isValidHost(const std::string& host);
    static int parsePort(const std::string& port);
};

}  // namespace o2l