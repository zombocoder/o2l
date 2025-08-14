#pragma once

#include <memory>
#include <regex>
#include <string>
#include <vector>

#include "Context.hpp"
#include "ObjectInstance.hpp"
#include "Value.hpp"

namespace o2l {

// Regex flags for pattern compilation
enum class RegexFlags {
    NONE = 0,
    CASE_INSENSITIVE = 1 << 0,  // i flag
    MULTILINE = 1 << 1,         // m flag
    DOT_ALL = 1 << 2,           // s flag
    EXTENDED = 1 << 3,          // x flag (ignore whitespace)
    GLOBAL = 1 << 4             // g flag (for replace operations)
};

// Helper to combine flags
inline RegexFlags operator|(RegexFlags a, RegexFlags b) {
    return static_cast<RegexFlags>(static_cast<int>(a) | static_cast<int>(b));
}

inline RegexFlags operator&(RegexFlags a, RegexFlags b) {
    return static_cast<RegexFlags>(static_cast<int>(a) & static_cast<int>(b));
}

// Match result structure
struct RegexMatch {
    std::string matched_text;
    std::vector<std::string> groups;
    size_t position;
    size_t length;

    RegexMatch() : position(std::string::npos), length(0) {}
    RegexMatch(const std::string& text, size_t pos, size_t len)
        : matched_text(text), position(pos), length(len) {}
};

class RegexpLibrary {
   public:
    // Create the regexp object with native methods
    static std::shared_ptr<ObjectInstance> createRegexpObject();

    // Core pattern matching methods
    static Value nativeMatch(const std::vector<Value>& args, Context& context);
    static Value nativeFind(const std::vector<Value>& args, Context& context);
    static Value nativeFindAll(const std::vector<Value>& args, Context& context);
    static Value nativeReplace(const std::vector<Value>& args, Context& context);
    static Value nativeReplaceAll(const std::vector<Value>& args, Context& context);
    static Value nativeSplit(const std::vector<Value>& args, Context& context);

    // Advanced pattern methods
    static Value nativeGroups(const std::vector<Value>& args, Context& context);
    static Value nativeTest(const std::vector<Value>& args, Context& context);
    static Value nativeEscape(const std::vector<Value>& args, Context& context);

    // Utility methods
    static Value nativeIsValid(const std::vector<Value>& args, Context& context);
    static Value nativeCount(const std::vector<Value>& args, Context& context);
    static Value nativeExtract(const std::vector<Value>& args, Context& context);

   private:
    // Helper functions
    static RegexFlags parseFlags(const std::string& flag_string);
    static std::regex_constants::syntax_option_type getRegexOptions(RegexFlags flags);
    static std::regex_constants::match_flag_type getMatchOptions(RegexFlags flags);
    static std::regex compilePattern(const std::string& pattern, RegexFlags flags);
    static RegexMatch findFirstMatch(const std::string& text, const std::regex& regex);
    static std::vector<RegexMatch> findAllMatches(const std::string& text, const std::regex& regex);
    static std::string escapeRegexChars(const std::string& text);

    // Error handling
    static void validatePattern(const std::string& pattern);
    static void validateText(const std::string& text);
};

}  // namespace o2l