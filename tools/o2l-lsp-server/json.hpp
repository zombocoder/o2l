// Lightweight JSON header for the LSP server
// This is a simplified version - for production use nlohmann/json
#pragma once

#include <string>
#include <map>
#include <vector>
#include <memory>
#include <sstream>
#include <initializer_list>
#include <cctype>

namespace nlohmann {
    
class json {
private:
    enum Type { NULL_TYPE, BOOL_TYPE, INT_TYPE, DOUBLE_TYPE, STRING_TYPE, ARRAY_TYPE, OBJECT_TYPE };
    
    Type type_;
    union {
        bool bool_value;
        int int_value;
        double double_value;
    };
    std::string string_value;
    std::vector<json> array_value;
    std::map<std::string, json> object_value;

public:
    // Constructors
    json() : type_(NULL_TYPE) {}
    json(bool value) : type_(BOOL_TYPE), bool_value(value) {}
    json(int value) : type_(INT_TYPE), int_value(value) {}
    json(double value) : type_(DOUBLE_TYPE), double_value(value) {}
    json(const char* value) : type_(STRING_TYPE), string_value(value) {}
    json(const std::string& value) : type_(STRING_TYPE), string_value(value) {}
    
    // Initializer list constructor for objects
    json(std::initializer_list<std::pair<std::string, json>> init_list) : type_(OBJECT_TYPE) {
        for (const auto& pair : init_list) {
            object_value[pair.first] = pair.second;
        }
    }
    
    // Static constructors
    static json array() { 
        json j; 
        j.type_ = ARRAY_TYPE; 
        return j; 
    }
    
    static json object() { 
        json j; 
        j.type_ = OBJECT_TYPE; 
        return j; 
    }
    
    // Type checks
    bool is_null() const { return type_ == NULL_TYPE; }
    bool is_bool() const { return type_ == BOOL_TYPE; }
    bool is_number() const { return type_ == INT_TYPE || type_ == DOUBLE_TYPE; }
    bool is_string() const { return type_ == STRING_TYPE; }
    bool is_array() const { return type_ == ARRAY_TYPE; }
    bool is_object() const { return type_ == OBJECT_TYPE; }
    
    // Array operations
    void push_back(const json& item) {
        if (type_ != ARRAY_TYPE) {
            type_ = ARRAY_TYPE;
            array_value.clear();
        }
        array_value.push_back(item);
    }
    
    // Object operations
    json& operator[](const std::string& key) {
        if (type_ != OBJECT_TYPE) {
            type_ = OBJECT_TYPE;
            object_value.clear();
        }
        return object_value[key];
    }
    
    const json& operator[](const std::string& key) const {
        static json null_json;
        if (type_ == OBJECT_TYPE) {
            auto it = object_value.find(key);
            if (it != object_value.end()) {
                return it->second;
            }
        }
        return null_json;
    }
    
    bool contains(const std::string& key) const {
        return type_ == OBJECT_TYPE && object_value.find(key) != object_value.end();
    }
    
    // Iterator support for range-based for loops
    auto items() const {
        return object_value;
    }
    
    // Iterator support for arrays and objects
    class iterator {
    private:
        Type type_;
        std::vector<json>::iterator array_it_;
        std::map<std::string, json>::iterator object_it_;
        
    public:
        iterator(std::vector<json>::iterator it) : type_(ARRAY_TYPE), array_it_(it) {}
        iterator(std::map<std::string, json>::iterator it) : type_(OBJECT_TYPE), object_it_(it) {}
        
        json& operator*() {
            if (type_ == ARRAY_TYPE) {
                return *array_it_;
            } else {
                return object_it_->second;
            }
        }
        
        iterator& operator++() {
            if (type_ == ARRAY_TYPE) {
                ++array_it_;
            } else {
                ++object_it_;
            }
            return *this;
        }
        
        bool operator!=(const iterator& other) const {
            if (type_ != other.type_) return true;
            if (type_ == ARRAY_TYPE) {
                return array_it_ != other.array_it_;
            } else {
                return object_it_ != other.object_it_;
            }
        }
    };
    
    class const_iterator {
    private:
        Type type_;
        std::vector<json>::const_iterator array_it_;
        std::map<std::string, json>::const_iterator object_it_;
        
    public:
        const_iterator(std::vector<json>::const_iterator it) : type_(ARRAY_TYPE), array_it_(it) {}
        const_iterator(std::map<std::string, json>::const_iterator it) : type_(OBJECT_TYPE), object_it_(it) {}
        
        const json& operator*() const {
            if (type_ == ARRAY_TYPE) {
                return *array_it_;
            } else {
                return object_it_->second;
            }
        }
        
        const_iterator& operator++() {
            if (type_ == ARRAY_TYPE) {
                ++array_it_;
            } else {
                ++object_it_;
            }
            return *this;
        }
        
        bool operator!=(const const_iterator& other) const {
            if (type_ != other.type_) return true;
            if (type_ == ARRAY_TYPE) {
                return array_it_ != other.array_it_;
            } else {
                return object_it_ != other.object_it_;
            }
        }
    };
    
    iterator begin() {
        if (type_ == ARRAY_TYPE) {
            return iterator(array_value.begin());
        } else if (type_ == OBJECT_TYPE) {
            return iterator(object_value.begin());
        } else {
            // Return a dummy iterator for other types
            static std::vector<json> empty_array;
            return iterator(empty_array.begin());
        }
    }
    
    iterator end() {
        if (type_ == ARRAY_TYPE) {
            return iterator(array_value.end());
        } else if (type_ == OBJECT_TYPE) {
            return iterator(object_value.end());
        } else {
            static std::vector<json> empty_array;
            return iterator(empty_array.end());
        }
    }
    
    const_iterator begin() const {
        if (type_ == ARRAY_TYPE) {
            return const_iterator(array_value.begin());
        } else if (type_ == OBJECT_TYPE) {
            return const_iterator(object_value.begin());
        } else {
            static const std::vector<json> empty_array;
            return const_iterator(empty_array.begin());
        }
    }
    
    const_iterator end() const {
        if (type_ == ARRAY_TYPE) {
            return const_iterator(array_value.end());
        } else if (type_ == OBJECT_TYPE) {
            return const_iterator(object_value.end());
        } else {
            static const std::vector<json> empty_array;
            return const_iterator(empty_array.end());
        }
    }
    
    // Value access with defaults
    template<typename T>
    T value(const std::string& key, const T& default_value) const {
        if (type_ == OBJECT_TYPE) {
            auto it = object_value.find(key);
            if (it != object_value.end()) {
                return it->second.get<T>();
            }
        }
        return default_value;
    }
    
    // Specialized for string literals
    std::string value(const std::string& key, const char* default_value) const {
        if (type_ == OBJECT_TYPE) {
            auto it = object_value.find(key);
            if (it != object_value.end()) {
                return it->second.get_string();
            }
        }
        return std::string(default_value);
    }
    
    // Get value
    template<typename T>
    T get() const;
    
    // Template specializations declared here
    bool get_bool() const;
    int get_int() const;
    double get_double() const;
    std::string get_string() const;
    
    // Serialization
    std::string dump() const {
        switch (type_) {
            case NULL_TYPE: return "null";
            case BOOL_TYPE: return bool_value ? "true" : "false";
            case INT_TYPE: return std::to_string(int_value);
            case DOUBLE_TYPE: return std::to_string(double_value);
            case STRING_TYPE: return "\"" + escape_string(string_value) + "\"";
            case ARRAY_TYPE: {
                std::string result = "[";
                for (size_t i = 0; i < array_value.size(); ++i) {
                    if (i > 0) result += ",";
                    result += array_value[i].dump();
                }
                result += "]";
                return result;
            }
            case OBJECT_TYPE: {
                std::string result = "{";
                bool first = true;
                for (const auto& pair : object_value) {
                    if (!first) result += ",";
                    result += "\"" + escape_string(pair.first) + "\":" + pair.second.dump();
                    first = false;
                }
                result += "}";
                return result;
            }
        }
        return "null";
    }
    
    // JSON parsing implementation
    static json parse(const std::string& str) {
        size_t pos = 0;
        return parse_value(str, pos);
    }

private:
    static json parse_value(const std::string& str, size_t& pos) {
        skip_whitespace(str, pos);
        
        if (pos >= str.length()) {
            throw parse_error("Unexpected end of input");
        }
        
        char ch = str[pos];
        
        if (ch == '"') {
            return parse_string(str, pos);
        } else if (ch == '{') {
            return parse_object(str, pos);
        } else if (ch == '[') {
            return parse_array(str, pos);
        } else if (ch == 't' || ch == 'f') {
            return parse_boolean(str, pos);
        } else if (ch == 'n') {
            return parse_null(str, pos);
        } else if (ch == '-' || (ch >= '0' && ch <= '9')) {
            return parse_number(str, pos);
        } else {
            throw parse_error("Unexpected character: " + std::string(1, ch));
        }
    }
    
    static void skip_whitespace(const std::string& str, size_t& pos) {
        while (pos < str.length() && std::isspace(str[pos])) {
            pos++;
        }
    }
    
    static json parse_string(const std::string& str, size_t& pos) {
        if (str[pos] != '"') {
            throw parse_error("Expected '\"'");
        }
        pos++; // Skip opening quote
        
        std::string value;
        while (pos < str.length() && str[pos] != '"') {
            if (str[pos] == '\\' && pos + 1 < str.length()) {
                pos++; // Skip backslash
                char escaped = str[pos];
                switch (escaped) {
                    case '"': value += '"'; break;
                    case '\\': value += '\\'; break;
                    case '/': value += '/'; break;
                    case 'b': value += '\b'; break;
                    case 'f': value += '\f'; break;
                    case 'n': value += '\n'; break;
                    case 'r': value += '\r'; break;
                    case 't': value += '\t'; break;
                    default:
                        value += escaped; // Just add the character
                        break;
                }
            } else {
                value += str[pos];
            }
            pos++;
        }
        
        if (pos >= str.length() || str[pos] != '"') {
            throw parse_error("Unterminated string");
        }
        pos++; // Skip closing quote
        
        return json(value);
    }
    
    static json parse_object(const std::string& str, size_t& pos) {
        if (str[pos] != '{') {
            throw parse_error("Expected '{'");
        }
        pos++; // Skip opening brace
        
        json obj = json::object();
        skip_whitespace(str, pos);
        
        if (pos < str.length() && str[pos] == '}') {
            pos++; // Skip closing brace for empty object
            return obj;
        }
        
        while (pos < str.length()) {
            skip_whitespace(str, pos);
            
            // Parse key
            json key = parse_string(str, pos);
            std::string key_str = key.get_string();
            
            skip_whitespace(str, pos);
            
            if (pos >= str.length() || str[pos] != ':') {
                throw parse_error("Expected ':'");
            }
            pos++; // Skip colon
            
            skip_whitespace(str, pos);
            
            // Parse value
            json value = parse_value(str, pos);
            obj[key_str] = value;
            
            skip_whitespace(str, pos);
            
            if (pos >= str.length()) {
                throw parse_error("Unexpected end of input");
            }
            
            if (str[pos] == '}') {
                pos++; // Skip closing brace
                break;
            } else if (str[pos] == ',') {
                pos++; // Skip comma
                continue;
            } else {
                throw parse_error("Expected ',' or '}'");
            }
        }
        
        return obj;
    }
    
    static json parse_array(const std::string& str, size_t& pos) {
        if (str[pos] != '[') {
            throw parse_error("Expected '['");
        }
        pos++; // Skip opening bracket
        
        json arr = json::array();
        skip_whitespace(str, pos);
        
        if (pos < str.length() && str[pos] == ']') {
            pos++; // Skip closing bracket for empty array
            return arr;
        }
        
        while (pos < str.length()) {
            skip_whitespace(str, pos);
            
            json value = parse_value(str, pos);
            arr.push_back(value);
            
            skip_whitespace(str, pos);
            
            if (pos >= str.length()) {
                throw parse_error("Unexpected end of input");
            }
            
            if (str[pos] == ']') {
                pos++; // Skip closing bracket
                break;
            } else if (str[pos] == ',') {
                pos++; // Skip comma
                continue;
            } else {
                throw parse_error("Expected ',' or ']'");
            }
        }
        
        return arr;
    }
    
    static json parse_boolean(const std::string& str, size_t& pos) {
        if (pos + 4 <= str.length() && str.substr(pos, 4) == "true") {
            pos += 4;
            return json(true);
        } else if (pos + 5 <= str.length() && str.substr(pos, 5) == "false") {
            pos += 5;
            return json(false);
        } else {
            throw parse_error("Invalid boolean value");
        }
    }
    
    static json parse_null(const std::string& str, size_t& pos) {
        if (pos + 4 <= str.length() && str.substr(pos, 4) == "null") {
            pos += 4;
            return json();
        } else {
            throw parse_error("Invalid null value");
        }
    }
    
    static json parse_number(const std::string& str, size_t& pos) {
        size_t start = pos;
        
        if (str[pos] == '-') {
            pos++;
        }
        
        if (pos >= str.length() || !std::isdigit(str[pos])) {
            throw parse_error("Invalid number");
        }
        
        // Skip digits
        while (pos < str.length() && std::isdigit(str[pos])) {
            pos++;
        }
        
        bool is_double = false;
        
        // Check for decimal point
        if (pos < str.length() && str[pos] == '.') {
            is_double = true;
            pos++;
            
            if (pos >= str.length() || !std::isdigit(str[pos])) {
                throw parse_error("Invalid number");
            }
            
            while (pos < str.length() && std::isdigit(str[pos])) {
                pos++;
            }
        }
        
        // Check for exponent
        if (pos < str.length() && (str[pos] == 'e' || str[pos] == 'E')) {
            is_double = true;
            pos++;
            
            if (pos < str.length() && (str[pos] == '+' || str[pos] == '-')) {
                pos++;
            }
            
            if (pos >= str.length() || !std::isdigit(str[pos])) {
                throw parse_error("Invalid number");
            }
            
            while (pos < str.length() && std::isdigit(str[pos])) {
                pos++;
            }
        }
        
        std::string num_str = str.substr(start, pos - start);
        
        try {
            if (is_double) {
                return json(std::stod(num_str));
            } else {
                return json(std::stoi(num_str));
            }
        } catch (...) {
            throw parse_error("Invalid number format");
        }
    }

public:
    
    class parse_error : public std::exception {
        std::string message_;
    public:
        parse_error(const std::string& msg) : message_(msg) {}
        const char* what() const noexcept override { return message_.c_str(); }
    };

private:
    std::string escape_string(const std::string& str) const {
        std::string result;
        for (char c : str) {
            switch (c) {
                case '"': result += "\\\""; break;
                case '\\': result += "\\\\"; break;
                case '\n': result += "\\n"; break;
                case '\r': result += "\\r"; break;
                case '\t': result += "\\t"; break;
                default: result += c; break;
            }
        }
        return result;
    }
};

// Template specializations implementation
inline bool json::get_bool() const {
    if (type_ == BOOL_TYPE) return bool_value;
    return false;
}

inline int json::get_int() const {
    if (type_ == INT_TYPE) return int_value;
    if (type_ == DOUBLE_TYPE) return static_cast<int>(double_value);
    return 0;
}

inline double json::get_double() const {
    if (type_ == DOUBLE_TYPE) return double_value;
    if (type_ == INT_TYPE) return static_cast<double>(int_value);
    return 0.0;
}

inline std::string json::get_string() const {
    if (type_ == STRING_TYPE) return string_value;
    return "";
}

// Template specializations for get()
template<>
inline bool json::get<bool>() const {
    return get_bool();
}

template<>
inline int json::get<int>() const {
    return get_int();
}

template<>
inline double json::get<double>() const {
    return get_double();
}

template<>
inline std::string json::get<std::string>() const {
    return get_string();
}

template<>
inline json json::get<json>() const {
    return *this;
}

} // namespace nlohmann