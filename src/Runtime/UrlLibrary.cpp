#include "UrlLibrary.hpp"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>

#include "../Common/Exceptions.hpp"

namespace o2l {

std::shared_ptr<ObjectInstance> UrlLibrary::createUrlObject() {
    auto urlObject = std::make_shared<ObjectInstance>("url");

    // URL parsing methods
    Method parse_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return UrlLibrary::nativeParse(args, ctx);
    };
    urlObject->addMethod("parse", parse_method, true);

    Method isValid_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return UrlLibrary::nativeIsValid(args, ctx);
    };
    urlObject->addMethod("isValid", isValid_method, true);

    Method getScheme_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return UrlLibrary::nativeGetScheme(args, ctx);
    };
    urlObject->addMethod("getScheme", getScheme_method, true);

    Method getHost_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return UrlLibrary::nativeGetHost(args, ctx);
    };
    urlObject->addMethod("getHost", getHost_method, true);

    Method getPort_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return UrlLibrary::nativeGetPort(args, ctx);
    };
    urlObject->addMethod("getPort", getPort_method, true);

    Method getPath_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return UrlLibrary::nativeGetPath(args, ctx);
    };
    urlObject->addMethod("getPath", getPath_method, true);

    Method getQuery_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return UrlLibrary::nativeGetQuery(args, ctx);
    };
    urlObject->addMethod("getQuery", getQuery_method, true);

    Method getFragment_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return UrlLibrary::nativeGetFragment(args, ctx);
    };
    urlObject->addMethod("getFragment", getFragment_method, true);

    // URL construction methods
    Method create_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return UrlLibrary::nativeCreate(args, ctx);
    };
    urlObject->addMethod("create", create_method, true);

    Method setScheme_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return UrlLibrary::nativeSetScheme(args, ctx);
    };
    urlObject->addMethod("setScheme", setScheme_method, true);

    Method setHost_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return UrlLibrary::nativeSetHost(args, ctx);
    };
    urlObject->addMethod("setHost", setHost_method, true);

    Method setPort_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return UrlLibrary::nativeSetPort(args, ctx);
    };
    urlObject->addMethod("setPort", setPort_method, true);

    Method setPath_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return UrlLibrary::nativeSetPath(args, ctx);
    };
    urlObject->addMethod("setPath", setPath_method, true);

    Method setQuery_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return UrlLibrary::nativeSetQuery(args, ctx);
    };
    urlObject->addMethod("setQuery", setQuery_method, true);

    Method setFragment_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return UrlLibrary::nativeSetFragment(args, ctx);
    };
    urlObject->addMethod("setFragment", setFragment_method, true);

    // Query parameter methods
    Method getParam_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return UrlLibrary::nativeGetParam(args, ctx);
    };
    urlObject->addMethod("getParam", getParam_method, true);

    Method setParam_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return UrlLibrary::nativeSetParam(args, ctx);
    };
    urlObject->addMethod("setParam", setParam_method, true);

    Method removeParam_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return UrlLibrary::nativeRemoveParam(args, ctx);
    };
    urlObject->addMethod("removeParam", removeParam_method, true);

    Method getParams_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return UrlLibrary::nativeGetParams(args, ctx);
    };
    urlObject->addMethod("getParams", getParams_method, true);

    // URL manipulation methods
    Method normalize_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return UrlLibrary::nativeNormalize(args, ctx);
    };
    urlObject->addMethod("normalize", normalize_method, true);

    Method resolve_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return UrlLibrary::nativeResolve(args, ctx);
    };
    urlObject->addMethod("resolve", resolve_method, true);

    Method join_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return UrlLibrary::nativeJoin(args, ctx);
    };
    urlObject->addMethod("join", join_method, true);

    Method encode_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return UrlLibrary::nativeEncode(args, ctx);
    };
    urlObject->addMethod("encode", encode_method, true);

    Method decode_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return UrlLibrary::nativeDecode(args, ctx);
    };
    urlObject->addMethod("decode", decode_method, true);

    // Utility methods
    Method getDomain_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return UrlLibrary::nativeGetDomain(args, ctx);
    };
    urlObject->addMethod("getDomain", getDomain_method, true);

    Method getOrigin_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return UrlLibrary::nativeGetOrigin(args, ctx);
    };
    urlObject->addMethod("getOrigin", getOrigin_method, true);

    Method isAbsolute_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return UrlLibrary::nativeIsAbsolute(args, ctx);
    };
    urlObject->addMethod("isAbsolute", isAbsolute_method, true);

    Method isRelative_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return UrlLibrary::nativeIsRelative(args, ctx);
    };
    urlObject->addMethod("isRelative", isRelative_method, true);

    return urlObject;
}

// URL parsing methods
Value UrlLibrary::nativeParse(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("url.parse() requires exactly 1 argument (url)", context);
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("url.parse() argument must be Text", context);
    }

    try {
        std::string url = std::get<Text>(args[0]);
        UrlComponents components = parseUrl(url);

        // Create a map-like object with URL components
        auto resultObj = std::make_shared<ObjectInstance>("UrlComponents");

        // Store components as properties (we'll simulate this with a map for now)
        // In a full implementation, this would return a proper object
        return Text(buildUrl(components));  // Return normalized URL for now

    } catch (const std::exception& e) {
        throw EvaluationError("URL parsing failed: " + std::string(e.what()), context);
    }
}

Value UrlLibrary::nativeIsValid(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("url.isValid() requires exactly 1 argument (url)", context);
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("url.isValid() argument must be Text", context);
    }

    try {
        std::string url = std::get<Text>(args[0]);
        UrlComponents components = parseUrl(url);
        return Bool(components.valid);
    } catch (const std::exception&) {
        return Bool(false);
    }
}

Value UrlLibrary::nativeGetScheme(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("url.getScheme() requires exactly 1 argument (url)", context);
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("url.getScheme() argument must be Text", context);
    }

    try {
        std::string url = std::get<Text>(args[0]);
        UrlComponents components = parseUrl(url);

        if (!components.valid) {
            throw EvaluationError("Invalid URL provided", context);
        }

        return Text(components.scheme);
    } catch (const EvaluationError&) {
        throw;
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to get scheme: " + std::string(e.what()), context);
    }
}

Value UrlLibrary::nativeGetHost(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("url.getHost() requires exactly 1 argument (url)", context);
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("url.getHost() argument must be Text", context);
    }

    try {
        std::string url = std::get<Text>(args[0]);
        UrlComponents components = parseUrl(url);

        if (!components.valid) {
            throw EvaluationError("Invalid URL provided", context);
        }

        return Text(components.host);
    } catch (const EvaluationError&) {
        throw;
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to get host: " + std::string(e.what()), context);
    }
}

Value UrlLibrary::nativeGetPort(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("url.getPort() requires exactly 1 argument (url)", context);
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("url.getPort() argument must be Text", context);
    }

    try {
        std::string url = std::get<Text>(args[0]);
        UrlComponents components = parseUrl(url);

        if (!components.valid) {
            throw EvaluationError("Invalid URL provided", context);
        }

        if (components.port.empty()) {
            return Text("");
        }

        return Text(components.port);
    } catch (const EvaluationError&) {
        throw;
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to get port: " + std::string(e.what()), context);
    }
}

Value UrlLibrary::nativeGetPath(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("url.getPath() requires exactly 1 argument (url)", context);
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("url.getPath() argument must be Text", context);
    }

    try {
        std::string url = std::get<Text>(args[0]);
        UrlComponents components = parseUrl(url);

        if (!components.valid) {
            throw EvaluationError("Invalid URL provided", context);
        }

        return Text(components.path);
    } catch (const EvaluationError&) {
        throw;
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to get path: " + std::string(e.what()), context);
    }
}

Value UrlLibrary::nativeGetQuery(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("url.getQuery() requires exactly 1 argument (url)", context);
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("url.getQuery() argument must be Text", context);
    }

    try {
        std::string url = std::get<Text>(args[0]);
        UrlComponents components = parseUrl(url);

        if (!components.valid) {
            throw EvaluationError("Invalid URL provided", context);
        }

        return Text(components.query);
    } catch (const EvaluationError&) {
        throw;
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to get query: " + std::string(e.what()), context);
    }
}

Value UrlLibrary::nativeGetFragment(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("url.getFragment() requires exactly 1 argument (url)", context);
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("url.getFragment() argument must be Text", context);
    }

    try {
        std::string url = std::get<Text>(args[0]);
        UrlComponents components = parseUrl(url);

        if (!components.valid) {
            throw EvaluationError("Invalid URL provided", context);
        }

        return Text(components.fragment);
    } catch (const EvaluationError&) {
        throw;
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to get fragment: " + std::string(e.what()), context);
    }
}

// URL construction methods
Value UrlLibrary::nativeCreate(const std::vector<Value>& args, Context& context) {
    if (args.size() < 2 || args.size() > 7) {
        throw EvaluationError(
            "url.create() requires 2-7 arguments (scheme, host, [port], [path], [query], "
            "[fragment])",
            context);
    }

    if (!std::holds_alternative<Text>(args[0]) || !std::holds_alternative<Text>(args[1])) {
        throw EvaluationError("url.create() scheme and host arguments must be Text", context);
    }

    try {
        UrlComponents components;
        components.scheme = std::get<Text>(args[0]);
        components.host = std::get<Text>(args[1]);

        if (args.size() > 2 && std::holds_alternative<Text>(args[2])) {
            components.port = std::get<Text>(args[2]);
        }
        if (args.size() > 3 && std::holds_alternative<Text>(args[3])) {
            components.path = std::get<Text>(args[3]);
        }
        if (args.size() > 4 && std::holds_alternative<Text>(args[4])) {
            components.query = std::get<Text>(args[4]);
        }
        if (args.size() > 5 && std::holds_alternative<Text>(args[5])) {
            components.fragment = std::get<Text>(args[5]);
        }

        // Validate components
        if (!isValidScheme(components.scheme)) {
            throw EvaluationError("Invalid scheme: " + components.scheme, context);
        }
        if (!isValidHost(components.host)) {
            throw EvaluationError("Invalid host: " + components.host, context);
        }

        components.valid = true;
        return Text(buildUrl(components));

    } catch (const EvaluationError&) {
        throw;
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to create URL: " + std::string(e.what()), context);
    }
}

Value UrlLibrary::nativeSetScheme(const std::vector<Value>& args, Context& context) {
    if (args.size() != 2) {
        throw EvaluationError("url.setScheme() requires exactly 2 arguments (url, scheme)",
                              context);
    }

    if (!std::holds_alternative<Text>(args[0]) || !std::holds_alternative<Text>(args[1])) {
        throw EvaluationError("url.setScheme() arguments must be Text", context);
    }

    try {
        std::string url = std::get<Text>(args[0]);
        std::string newScheme = std::get<Text>(args[1]);

        if (!isValidScheme(newScheme)) {
            throw EvaluationError("Invalid scheme: " + newScheme, context);
        }

        UrlComponents components = parseUrl(url);
        if (!components.valid) {
            throw EvaluationError("Invalid URL provided", context);
        }

        components.scheme = newScheme;
        return Text(buildUrl(components));

    } catch (const EvaluationError&) {
        throw;
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to set scheme: " + std::string(e.what()), context);
    }
}

Value UrlLibrary::nativeSetHost(const std::vector<Value>& args, Context& context) {
    if (args.size() != 2) {
        throw EvaluationError("url.setHost() requires exactly 2 arguments (url, host)", context);
    }

    if (!std::holds_alternative<Text>(args[0]) || !std::holds_alternative<Text>(args[1])) {
        throw EvaluationError("url.setHost() arguments must be Text", context);
    }

    try {
        std::string url = std::get<Text>(args[0]);
        std::string newHost = std::get<Text>(args[1]);

        if (!isValidHost(newHost)) {
            throw EvaluationError("Invalid host: " + newHost, context);
        }

        UrlComponents components = parseUrl(url);
        if (!components.valid) {
            throw EvaluationError("Invalid URL provided", context);
        }

        components.host = newHost;
        return Text(buildUrl(components));

    } catch (const EvaluationError&) {
        throw;
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to set host: " + std::string(e.what()), context);
    }
}

Value UrlLibrary::nativeSetPort(const std::vector<Value>& args, Context& context) {
    if (args.size() != 2) {
        throw EvaluationError("url.setPort() requires exactly 2 arguments (url, port)", context);
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("url.setPort() first argument must be Text", context);
    }

    try {
        std::string url = std::get<Text>(args[0]);

        UrlComponents components = parseUrl(url);
        if (!components.valid) {
            throw EvaluationError("Invalid URL provided", context);
        }

        if (std::holds_alternative<Text>(args[1])) {
            std::string portStr = std::get<Text>(args[1]);
            if (!portStr.empty()) {
                int port = parsePort(portStr);
                if (port < 1 || port > 65535) {
                    throw EvaluationError("Invalid port number: " + portStr, context);
                }
                components.port = portStr;
            } else {
                components.port = "";
            }
        } else if (std::holds_alternative<Int>(args[1])) {
            int port = std::get<Int>(args[1]);
            if (port < 1 || port > 65535) {
                throw EvaluationError("Invalid port number: " + std::to_string(port), context);
            }
            components.port = std::to_string(port);
        } else {
            throw EvaluationError("url.setPort() port argument must be Text or Int", context);
        }

        return Text(buildUrl(components));

    } catch (const EvaluationError&) {
        throw;
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to set port: " + std::string(e.what()), context);
    }
}

Value UrlLibrary::nativeSetPath(const std::vector<Value>& args, Context& context) {
    if (args.size() != 2) {
        throw EvaluationError("url.setPath() requires exactly 2 arguments (url, path)", context);
    }

    if (!std::holds_alternative<Text>(args[0]) || !std::holds_alternative<Text>(args[1])) {
        throw EvaluationError("url.setPath() arguments must be Text", context);
    }

    try {
        std::string url = std::get<Text>(args[0]);
        std::string newPath = std::get<Text>(args[1]);

        UrlComponents components = parseUrl(url);
        if (!components.valid) {
            throw EvaluationError("Invalid URL provided", context);
        }

        components.path = newPath;
        return Text(buildUrl(components));

    } catch (const EvaluationError&) {
        throw;
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to set path: " + std::string(e.what()), context);
    }
}

Value UrlLibrary::nativeSetQuery(const std::vector<Value>& args, Context& context) {
    if (args.size() != 2) {
        throw EvaluationError("url.setQuery() requires exactly 2 arguments (url, query)", context);
    }

    if (!std::holds_alternative<Text>(args[0]) || !std::holds_alternative<Text>(args[1])) {
        throw EvaluationError("url.setQuery() arguments must be Text", context);
    }

    try {
        std::string url = std::get<Text>(args[0]);
        std::string newQuery = std::get<Text>(args[1]);

        UrlComponents components = parseUrl(url);
        if (!components.valid) {
            throw EvaluationError("Invalid URL provided", context);
        }

        components.query = newQuery;
        return Text(buildUrl(components));

    } catch (const EvaluationError&) {
        throw;
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to set query: " + std::string(e.what()), context);
    }
}

Value UrlLibrary::nativeSetFragment(const std::vector<Value>& args, Context& context) {
    if (args.size() != 2) {
        throw EvaluationError("url.setFragment() requires exactly 2 arguments (url, fragment)",
                              context);
    }

    if (!std::holds_alternative<Text>(args[0]) || !std::holds_alternative<Text>(args[1])) {
        throw EvaluationError("url.setFragment() arguments must be Text", context);
    }

    try {
        std::string url = std::get<Text>(args[0]);
        std::string newFragment = std::get<Text>(args[1]);

        UrlComponents components = parseUrl(url);
        if (!components.valid) {
            throw EvaluationError("Invalid URL provided", context);
        }

        components.fragment = newFragment;
        return Text(buildUrl(components));

    } catch (const EvaluationError&) {
        throw;
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to set fragment: " + std::string(e.what()), context);
    }
}

// Query parameter methods
Value UrlLibrary::nativeGetParam(const std::vector<Value>& args, Context& context) {
    if (args.size() != 2) {
        throw EvaluationError("url.getParam() requires exactly 2 arguments (url, paramName)",
                              context);
    }

    if (!std::holds_alternative<Text>(args[0]) || !std::holds_alternative<Text>(args[1])) {
        throw EvaluationError("url.getParam() arguments must be Text", context);
    }

    try {
        std::string url = std::get<Text>(args[0]);
        std::string paramName = std::get<Text>(args[1]);

        UrlComponents components = parseUrl(url);
        if (!components.valid) {
            throw EvaluationError("Invalid URL provided", context);
        }

        auto params = parseQueryString(components.query);
        auto it = params.find(paramName);

        if (it != params.end()) {
            return Text(it->second);
        } else {
            return Text("");
        }

    } catch (const EvaluationError&) {
        throw;
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to get parameter: " + std::string(e.what()), context);
    }
}

Value UrlLibrary::nativeSetParam(const std::vector<Value>& args, Context& context) {
    if (args.size() != 3) {
        throw EvaluationError(
            "url.setParam() requires exactly 3 arguments (url, paramName, paramValue)", context);
    }

    if (!std::holds_alternative<Text>(args[0]) || !std::holds_alternative<Text>(args[1]) ||
        !std::holds_alternative<Text>(args[2])) {
        throw EvaluationError("url.setParam() arguments must be Text", context);
    }

    try {
        std::string url = std::get<Text>(args[0]);
        std::string paramName = std::get<Text>(args[1]);
        std::string paramValue = std::get<Text>(args[2]);

        UrlComponents components = parseUrl(url);
        if (!components.valid) {
            throw EvaluationError("Invalid URL provided", context);
        }

        auto params = parseQueryString(components.query);
        params[paramName] = paramValue;
        components.query = buildQueryString(params);

        return Text(buildUrl(components));

    } catch (const EvaluationError&) {
        throw;
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to set parameter: " + std::string(e.what()), context);
    }
}

Value UrlLibrary::nativeRemoveParam(const std::vector<Value>& args, Context& context) {
    if (args.size() != 2) {
        throw EvaluationError("url.removeParam() requires exactly 2 arguments (url, paramName)",
                              context);
    }

    if (!std::holds_alternative<Text>(args[0]) || !std::holds_alternative<Text>(args[1])) {
        throw EvaluationError("url.removeParam() arguments must be Text", context);
    }

    try {
        std::string url = std::get<Text>(args[0]);
        std::string paramName = std::get<Text>(args[1]);

        UrlComponents components = parseUrl(url);
        if (!components.valid) {
            throw EvaluationError("Invalid URL provided", context);
        }

        auto params = parseQueryString(components.query);
        params.erase(paramName);
        components.query = buildQueryString(params);

        return Text(buildUrl(components));

    } catch (const EvaluationError&) {
        throw;
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to remove parameter: " + std::string(e.what()), context);
    }
}

Value UrlLibrary::nativeGetParams(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("url.getParams() requires exactly 1 argument (url)", context);
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("url.getParams() argument must be Text", context);
    }

    try {
        std::string url = std::get<Text>(args[0]);

        UrlComponents components = parseUrl(url);
        if (!components.valid) {
            throw EvaluationError("Invalid URL provided", context);
        }

        auto params = parseQueryString(components.query);

        // Convert to a formatted string representation
        std::ostringstream result;
        result << "{";
        bool first = true;
        for (const auto& pair : params) {
            if (!first) result << ", ";
            result << "\"" << pair.first << "\": \"" << pair.second << "\"";
            first = false;
        }
        result << "}";

        return Text(result.str());

    } catch (const EvaluationError&) {
        throw;
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to get parameters: " + std::string(e.what()), context);
    }
}

// URL manipulation methods
Value UrlLibrary::nativeNormalize(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("url.normalize() requires exactly 1 argument (url)", context);
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("url.normalize() argument must be Text", context);
    }

    try {
        std::string url = std::get<Text>(args[0]);

        UrlComponents components = parseUrl(url);
        if (!components.valid) {
            throw EvaluationError("Invalid URL provided", context);
        }

        // Normalize path
        components.path = normalizePath(components.path);

        // Normalize scheme and host to lowercase
        std::transform(components.scheme.begin(), components.scheme.end(),
                       components.scheme.begin(), ::tolower);
        std::transform(components.host.begin(), components.host.end(), components.host.begin(),
                       ::tolower);

        return Text(buildUrl(components));

    } catch (const EvaluationError&) {
        throw;
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to normalize URL: " + std::string(e.what()), context);
    }
}

Value UrlLibrary::nativeResolve(const std::vector<Value>& args, Context& context) {
    if (args.size() != 2) {
        throw EvaluationError("url.resolve() requires exactly 2 arguments (baseUrl, relativeUrl)",
                              context);
    }

    if (!std::holds_alternative<Text>(args[0]) || !std::holds_alternative<Text>(args[1])) {
        throw EvaluationError("url.resolve() arguments must be Text", context);
    }

    try {
        std::string baseUrl = std::get<Text>(args[0]);
        std::string relativeUrl = std::get<Text>(args[1]);

        // If relative URL is actually absolute, return it as-is
        UrlComponents relativeComponents = parseUrl(relativeUrl);
        if (relativeComponents.valid && !relativeComponents.scheme.empty()) {
            return Text(relativeUrl);
        }

        UrlComponents baseComponents = parseUrl(baseUrl);
        if (!baseComponents.valid) {
            throw EvaluationError("Invalid base URL provided", context);
        }

        // Simple resolution: combine base URL with relative path
        UrlComponents resolved = baseComponents;

        if (relativeUrl.empty()) {
            return Text(buildUrl(resolved));
        }

        if (relativeUrl[0] == '/') {
            // Absolute path - replace everything after host:port
            resolved.path = relativeUrl;
            resolved.query = "";
            resolved.fragment = "";
        } else {
            // Relative path - resolve against base path
            std::string basePath = baseComponents.path;
            if (basePath.empty() || basePath.back() != '/') {
                size_t lastSlash = basePath.rfind('/');
                if (lastSlash != std::string::npos) {
                    basePath = basePath.substr(0, lastSlash + 1);
                } else {
                    basePath = "/";
                }
            }
            resolved.path = normalizePath(basePath + relativeUrl);
            resolved.query = "";
            resolved.fragment = "";
        }

        return Text(buildUrl(resolved));

    } catch (const EvaluationError&) {
        throw;
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to resolve URL: " + std::string(e.what()), context);
    }
}

Value UrlLibrary::nativeJoin(const std::vector<Value>& args, Context& context) {
    if (args.size() < 2) {
        throw EvaluationError("url.join() requires at least 2 arguments", context);
    }

    try {
        std::string result;
        for (size_t i = 0; i < args.size(); ++i) {
            if (!std::holds_alternative<Text>(args[i])) {
                throw EvaluationError("url.join() arguments must be Text", context);
            }

            std::string part = std::get<Text>(args[i]);

            if (i == 0) {
                result = part;
            } else {
                // Ensure proper path joining
                if (!result.empty() && result.back() != '/') {
                    result += "/";
                }
                if (!part.empty() && part[0] == '/') {
                    part = part.substr(1);
                }
                result += part;
            }
        }

        return Text(result);

    } catch (const EvaluationError&) {
        throw;
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to join URLs: " + std::string(e.what()), context);
    }
}

Value UrlLibrary::nativeEncode(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("url.encode() requires exactly 1 argument (text)", context);
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("url.encode() argument must be Text", context);
    }

    try {
        std::string text = std::get<Text>(args[0]);
        return Text(urlEncode(text));
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to encode URL: " + std::string(e.what()), context);
    }
}

Value UrlLibrary::nativeDecode(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("url.decode() requires exactly 1 argument (encodedText)", context);
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("url.decode() argument must be Text", context);
    }

    try {
        std::string encodedText = std::get<Text>(args[0]);
        return Text(urlDecode(encodedText));
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to decode URL: " + std::string(e.what()), context);
    }
}

// Utility methods
Value UrlLibrary::nativeGetDomain(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("url.getDomain() requires exactly 1 argument (url)", context);
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("url.getDomain() argument must be Text", context);
    }

    try {
        std::string url = std::get<Text>(args[0]);
        UrlComponents components = parseUrl(url);

        if (!components.valid) {
            throw EvaluationError("Invalid URL provided", context);
        }

        // Extract domain from host (remove subdomains if needed)
        std::string host = components.host;

        // Simple domain extraction - in real implementation would handle TLD parsing
        size_t dotCount = std::count(host.begin(), host.end(), '.');
        if (dotCount >= 2) {
            // Find last two parts (domain.tld)
            size_t lastDot = host.rfind('.');
            if (lastDot != std::string::npos) {
                size_t secondLastDot = host.rfind('.', lastDot - 1);
                if (secondLastDot != std::string::npos) {
                    return Text(host.substr(secondLastDot + 1));
                }
            }
        }

        return Text(host);

    } catch (const EvaluationError&) {
        throw;
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to get domain: " + std::string(e.what()), context);
    }
}

Value UrlLibrary::nativeGetOrigin(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("url.getOrigin() requires exactly 1 argument (url)", context);
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("url.getOrigin() argument must be Text", context);
    }

    try {
        std::string url = std::get<Text>(args[0]);
        UrlComponents components = parseUrl(url);

        if (!components.valid) {
            throw EvaluationError("Invalid URL provided", context);
        }

        // Origin is scheme + host + port (if not default)
        std::string origin = components.scheme + "://" + components.host;

        if (!components.port.empty()) {
            // Check if port is not default for scheme
            bool isDefaultPort = false;
            if (components.scheme == "http" && components.port == "80") isDefaultPort = true;
            if (components.scheme == "https" && components.port == "443") isDefaultPort = true;
            if (components.scheme == "ftp" && components.port == "21") isDefaultPort = true;

            if (!isDefaultPort) {
                origin += ":" + components.port;
            }
        }

        return Text(origin);

    } catch (const EvaluationError&) {
        throw;
    } catch (const std::exception& e) {
        throw EvaluationError("Failed to get origin: " + std::string(e.what()), context);
    }
}

Value UrlLibrary::nativeIsAbsolute(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("url.isAbsolute() requires exactly 1 argument (url)", context);
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("url.isAbsolute() argument must be Text", context);
    }

    try {
        std::string url = std::get<Text>(args[0]);
        UrlComponents components = parseUrl(url);

        return Bool(components.valid && !components.scheme.empty());

    } catch (const std::exception&) {
        return Bool(false);
    }
}

Value UrlLibrary::nativeIsRelative(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("url.isRelative() requires exactly 1 argument (url)", context);
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("url.isRelative() argument must be Text", context);
    }

    try {
        std::string url = std::get<Text>(args[0]);
        UrlComponents components = parseUrl(url);

        // Relative if it doesn't have a scheme or if it starts with / or ./
        return Bool(!components.valid || components.scheme.empty() || url[0] == '/' ||
                    url.substr(0, 2) == "./" || url.substr(0, 3) == "../");

    } catch (const std::exception&) {
        return Bool(true);
    }
}

// Helper methods implementation
UrlComponents UrlLibrary::parseUrl(const std::string& url) {
    UrlComponents components;

    if (url.empty()) {
        return components;
    }

    std::string remaining = url;

    // Parse scheme
    size_t schemeEnd = remaining.find("://");
    if (schemeEnd != std::string::npos) {
        components.scheme = remaining.substr(0, schemeEnd);
        remaining = remaining.substr(schemeEnd + 3);

        // For absolute URLs, we need scheme and host to be valid
        // Parse fragment first (at the end)
        size_t fragmentPos = remaining.rfind('#');
        if (fragmentPos != std::string::npos) {
            components.fragment = remaining.substr(fragmentPos + 1);
            remaining = remaining.substr(0, fragmentPos);
        }

        // Parse query
        size_t queryPos = remaining.rfind('?');
        if (queryPos != std::string::npos) {
            components.query = remaining.substr(queryPos + 1);
            remaining = remaining.substr(0, queryPos);
        }

        // Parse path
        size_t pathPos = remaining.find('/');
        if (pathPos != std::string::npos) {
            components.path = remaining.substr(pathPos);
            remaining = remaining.substr(0, pathPos);
        } else {
            components.path = "/";
        }

        // Parse host and port
        if (!remaining.empty()) {
            // Check for port
            size_t portPos = remaining.rfind(':');
            if (portPos != std::string::npos) {
                std::string portStr = remaining.substr(portPos + 1);
                // Verify it's actually a port (all digits)
                if (!portStr.empty() && std::all_of(portStr.begin(), portStr.end(), ::isdigit)) {
                    components.port = portStr;
                    remaining = remaining.substr(0, portPos);
                }
            }

            components.host = remaining;
        }

        // Validate absolute URL
        components.valid = isValidScheme(components.scheme) && isValidHost(components.host);
    } else {
        // No scheme found - this is either a relative URL or invalid
        // For validation purposes, if it doesn't have a scheme, it's not a valid absolute URL
        components.valid = false;

        // Still parse it as a relative URL for other operations
        components.path = url;  // Treat the whole thing as path
    }

    return components;
}

std::string UrlLibrary::buildUrl(const UrlComponents& components) {
    std::string url;

    if (!components.scheme.empty()) {
        url += components.scheme + "://";
    }

    if (!components.host.empty()) {
        url += components.host;
    }

    if (!components.port.empty()) {
        // Check if port is not default for scheme
        bool isDefaultPort = false;
        if (components.scheme == "http" && components.port == "80") isDefaultPort = true;
        if (components.scheme == "https" && components.port == "443") isDefaultPort = true;
        if (components.scheme == "ftp" && components.port == "21") isDefaultPort = true;

        if (!isDefaultPort) {
            url += ":" + components.port;
        }
    }

    if (!components.path.empty()) {
        if (components.path[0] != '/' && !url.empty()) {
            url += "/";
        }
        url += components.path;
    } else if (!url.empty()) {
        // Add trailing slash for URLs with scheme/host but no path
        url += "/";
    }

    if (!components.query.empty()) {
        url += "?" + components.query;
    }

    if (!components.fragment.empty()) {
        url += "#" + components.fragment;
    }

    return url;
}

std::map<std::string, std::string> UrlLibrary::parseQueryString(const std::string& query) {
    std::map<std::string, std::string> params;

    if (query.empty()) {
        return params;
    }

    std::istringstream stream(query);
    std::string param;

    while (std::getline(stream, param, '&')) {
        size_t eqPos = param.find('=');
        if (eqPos != std::string::npos) {
            std::string key = urlDecode(param.substr(0, eqPos));
            std::string value = urlDecode(param.substr(eqPos + 1));
            params[key] = value;
        } else {
            params[urlDecode(param)] = "";
        }
    }

    return params;
}

std::string UrlLibrary::buildQueryString(const std::map<std::string, std::string>& params) {
    std::ostringstream query;
    bool first = true;

    for (const auto& pair : params) {
        if (!first) {
            query << "&";
        }
        query << urlEncode(pair.first);
        if (!pair.second.empty()) {
            query << "=" << urlEncode(pair.second);
        }
        first = false;
    }

    return query.str();
}

std::string UrlLibrary::urlEncode(const std::string& value) {
    std::ostringstream encoded;

    for (char c : value) {
        // Keep alphanumeric and some special chars
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            encoded << c;
        } else {
            encoded << '%' << std::uppercase << std::hex << std::setfill('0') << std::setw(2)
                    << (static_cast<unsigned char>(c) & 0xFF);
        }
    }

    return encoded.str();
}

std::string UrlLibrary::urlDecode(const std::string& value) {
    std::ostringstream decoded;

    for (size_t i = 0; i < value.length(); ++i) {
        if (value[i] == '%' && i + 2 < value.length()) {
            std::string hex = value.substr(i + 1, 2);
            try {
                int val = std::stoi(hex, nullptr, 16);
                decoded << static_cast<char>(val);
                i += 2;
            } catch (const std::exception&) {
                decoded << value[i];
            }
        } else if (value[i] == '+') {
            decoded << ' ';
        } else {
            decoded << value[i];
        }
    }

    return decoded.str();
}

std::string UrlLibrary::normalizePath(const std::string& path) {
    if (path.empty()) {
        return "/";
    }

    std::vector<std::string> parts;
    std::istringstream stream(path);
    std::string part;

    while (std::getline(stream, part, '/')) {
        if (part.empty() || part == ".") {
            continue;
        } else if (part == "..") {
            if (!parts.empty() && parts.back() != "..") {
                parts.pop_back();
            } else if (path[0] != '/') {
                parts.push_back("..");
            }
        } else {
            parts.push_back(part);
        }
    }

    std::string normalized;
    if (path[0] == '/') {
        normalized = "/";
    }

    for (size_t i = 0; i < parts.size(); ++i) {
        if (i > 0 || normalized != "/") {
            normalized += "/";
        }
        normalized += parts[i];
    }

    return normalized.empty() ? "/" : normalized;
}

bool UrlLibrary::isValidScheme(const std::string& scheme) {
    if (scheme.empty()) {
        return false;
    }

    // Scheme must start with letter
    if (!isalpha(scheme[0])) {
        return false;
    }

    // Rest can be letters, digits, +, -, .
    for (size_t i = 1; i < scheme.length(); ++i) {
        char c = scheme[i];
        if (!isalnum(c) && c != '+' && c != '-' && c != '.') {
            return false;
        }
    }

    return true;
}

bool UrlLibrary::isValidHost(const std::string& host) {
    if (host.empty()) {
        return false;
    }

    // Very basic validation - in real implementation would be more comprehensive
    for (char c : host) {
        if (!isalnum(c) && c != '.' && c != '-' && c != ':' && c != '[' && c != ']') {
            return false;
        }
    }

    return true;
}

int UrlLibrary::parsePort(const std::string& port) {
    if (port.empty()) {
        return 0;
    }

    try {
        return std::stoi(port);
    } catch (const std::exception&) {
        return -1;
    }
}

}  // namespace o2l