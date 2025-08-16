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

#include "RegexpLibrary.hpp"

#include <algorithm>
#include <sstream>

#include "../Common/Exceptions.hpp"
#include "ListInstance.hpp"

namespace o2l {

std::shared_ptr<ObjectInstance> RegexpLibrary::createRegexpObject() {
    auto regexp_object = std::make_shared<ObjectInstance>("regexp");

    // Core pattern matching methods
    Method match_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return RegexpLibrary::nativeMatch(args, ctx);
    };
    regexp_object->addMethod("match", match_method, true);  // external

    Method find_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return RegexpLibrary::nativeFind(args, ctx);
    };
    regexp_object->addMethod("find", find_method, true);  // external

    Method findAll_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return RegexpLibrary::nativeFindAll(args, ctx);
    };
    regexp_object->addMethod("findAll", findAll_method, true);  // external

    Method replace_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return RegexpLibrary::nativeReplace(args, ctx);
    };
    regexp_object->addMethod("replace", replace_method, true);  // external

    Method replaceAll_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return RegexpLibrary::nativeReplaceAll(args, ctx);
    };
    regexp_object->addMethod("replaceAll", replaceAll_method, true);  // external

    Method split_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return RegexpLibrary::nativeSplit(args, ctx);
    };
    regexp_object->addMethod("split", split_method, true);  // external

    // Advanced pattern methods
    Method groups_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return RegexpLibrary::nativeGroups(args, ctx);
    };
    regexp_object->addMethod("groups", groups_method, true);  // external

    Method test_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return RegexpLibrary::nativeTest(args, ctx);
    };
    regexp_object->addMethod("test", test_method, true);  // external

    Method escape_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return RegexpLibrary::nativeEscape(args, ctx);
    };
    regexp_object->addMethod("escape", escape_method, true);  // external

    // Utility methods
    Method isValid_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return RegexpLibrary::nativeIsValid(args, ctx);
    };
    regexp_object->addMethod("isValid", isValid_method, true);  // external

    Method count_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return RegexpLibrary::nativeCount(args, ctx);
    };
    regexp_object->addMethod("count", count_method, true);  // external

    Method extract_method = [](const std::vector<Value>& args, Context& ctx) -> Value {
        return RegexpLibrary::nativeExtract(args, ctx);
    };
    regexp_object->addMethod("extract", extract_method, true);  // external

    return regexp_object;
}

// Core pattern matching methods implementation

Value RegexpLibrary::nativeMatch(const std::vector<Value>& args, Context& context) {
    if (args.size() < 2 || args.size() > 3) {
        throw EvaluationError(
            "match() requires 2-3 arguments (text: Text, pattern: Text, flags?: Text)");
    }

    if (!std::holds_alternative<Text>(args[0]) || !std::holds_alternative<Text>(args[1])) {
        throw EvaluationError("match() first two arguments must be Text");
    }

    try {
        std::string text = std::get<Text>(args[0]);
        std::string pattern = std::get<Text>(args[1]);
        RegexFlags flags = RegexFlags::NONE;

        if (args.size() == 3) {
            if (!std::holds_alternative<Text>(args[2])) {
                throw EvaluationError("match() flags argument must be Text");
            }
            flags = parseFlags(std::get<Text>(args[2]));
        }

        validatePattern(pattern);
        std::regex regex = compilePattern(pattern, flags);

        bool matches = std::regex_search(text, regex);
        return Bool(matches);

    } catch (const std::regex_error& e) {
        throw EvaluationError("match() regex error: " + std::string(e.what()));
    } catch (const std::exception& e) {
        throw EvaluationError("match() error: " + std::string(e.what()));
    }
}

Value RegexpLibrary::nativeFind(const std::vector<Value>& args, Context& context) {
    if (args.size() < 2 || args.size() > 3) {
        throw EvaluationError(
            "find() requires 2-3 arguments (text: Text, pattern: Text, flags?: Text)");
    }

    if (!std::holds_alternative<Text>(args[0]) || !std::holds_alternative<Text>(args[1])) {
        throw EvaluationError("find() first two arguments must be Text");
    }

    try {
        std::string text = std::get<Text>(args[0]);
        std::string pattern = std::get<Text>(args[1]);
        RegexFlags flags = RegexFlags::NONE;

        if (args.size() == 3) {
            if (!std::holds_alternative<Text>(args[2])) {
                throw EvaluationError("find() flags argument must be Text");
            }
            flags = parseFlags(std::get<Text>(args[2]));
        }

        validatePattern(pattern);
        std::regex regex = compilePattern(pattern, flags);

        RegexMatch match = findFirstMatch(text, regex);
        if (match.position != std::string::npos) {
            return Text(match.matched_text);
        }

        return Text("");

    } catch (const std::regex_error& e) {
        throw EvaluationError("find() regex error: " + std::string(e.what()));
    } catch (const std::exception& e) {
        throw EvaluationError("find() error: " + std::string(e.what()));
    }
}

Value RegexpLibrary::nativeFindAll(const std::vector<Value>& args, Context& context) {
    if (args.size() < 2 || args.size() > 3) {
        throw EvaluationError(
            "findAll() requires 2-3 arguments (text: Text, pattern: Text, flags?: Text)");
    }

    if (!std::holds_alternative<Text>(args[0]) || !std::holds_alternative<Text>(args[1])) {
        throw EvaluationError("findAll() first two arguments must be Text");
    }

    try {
        std::string text = std::get<Text>(args[0]);
        std::string pattern = std::get<Text>(args[1]);
        RegexFlags flags = RegexFlags::NONE;

        if (args.size() == 3) {
            if (!std::holds_alternative<Text>(args[2])) {
                throw EvaluationError("findAll() flags argument must be Text");
            }
            flags = parseFlags(std::get<Text>(args[2]));
        }

        validatePattern(pattern);
        std::regex regex = compilePattern(pattern, flags);

        auto matches = findAllMatches(text, regex);
        auto list = std::make_shared<ListInstance>();

        for (const auto& match : matches) {
            list->add(Text(match.matched_text));
        }

        return Value(list);

    } catch (const std::regex_error& e) {
        throw EvaluationError("findAll() regex error: " + std::string(e.what()));
    } catch (const std::exception& e) {
        throw EvaluationError("findAll() error: " + std::string(e.what()));
    }
}

Value RegexpLibrary::nativeReplace(const std::vector<Value>& args, Context& context) {
    if (args.size() < 3 || args.size() > 4) {
        throw EvaluationError(
            "replace() requires 3-4 arguments (text: Text, pattern: Text, replacement: Text, "
            "flags?: Text)");
    }

    if (!std::holds_alternative<Text>(args[0]) || !std::holds_alternative<Text>(args[1]) ||
        !std::holds_alternative<Text>(args[2])) {
        throw EvaluationError("replace() first three arguments must be Text");
    }

    try {
        std::string text = std::get<Text>(args[0]);
        std::string pattern = std::get<Text>(args[1]);
        std::string replacement = std::get<Text>(args[2]);
        RegexFlags flags = RegexFlags::NONE;

        if (args.size() == 4) {
            if (!std::holds_alternative<Text>(args[3])) {
                throw EvaluationError("replace() flags argument must be Text");
            }
            flags = parseFlags(std::get<Text>(args[3]));
        }

        validatePattern(pattern);
        std::regex regex = compilePattern(pattern, flags);

        // Replace only first match
        std::string result =
            std::regex_replace(text, regex, replacement, std::regex_constants::format_first_only);
        return Text(result);

    } catch (const std::regex_error& e) {
        throw EvaluationError("replace() regex error: " + std::string(e.what()));
    } catch (const std::exception& e) {
        throw EvaluationError("replace() error: " + std::string(e.what()));
    }
}

Value RegexpLibrary::nativeReplaceAll(const std::vector<Value>& args, Context& context) {
    if (args.size() < 3 || args.size() > 4) {
        throw EvaluationError(
            "replaceAll() requires 3-4 arguments (text: Text, pattern: Text, replacement: Text, "
            "flags?: Text)");
    }

    if (!std::holds_alternative<Text>(args[0]) || !std::holds_alternative<Text>(args[1]) ||
        !std::holds_alternative<Text>(args[2])) {
        throw EvaluationError("replaceAll() first three arguments must be Text");
    }

    try {
        std::string text = std::get<Text>(args[0]);
        std::string pattern = std::get<Text>(args[1]);
        std::string replacement = std::get<Text>(args[2]);
        RegexFlags flags = RegexFlags::NONE;

        if (args.size() == 4) {
            if (!std::holds_alternative<Text>(args[3])) {
                throw EvaluationError("replaceAll() flags argument must be Text");
            }
            flags = parseFlags(std::get<Text>(args[3]));
        }

        validatePattern(pattern);
        std::regex regex = compilePattern(pattern, flags);

        // Replace all matches
        std::string result = std::regex_replace(text, regex, replacement);
        return Text(result);

    } catch (const std::regex_error& e) {
        throw EvaluationError("replaceAll() regex error: " + std::string(e.what()));
    } catch (const std::exception& e) {
        throw EvaluationError("replaceAll() error: " + std::string(e.what()));
    }
}

Value RegexpLibrary::nativeSplit(const std::vector<Value>& args, Context& context) {
    if (args.size() < 2 || args.size() > 3) {
        throw EvaluationError(
            "split() requires 2-3 arguments (text: Text, pattern: Text, flags?: Text)");
    }

    if (!std::holds_alternative<Text>(args[0]) || !std::holds_alternative<Text>(args[1])) {
        throw EvaluationError("split() first two arguments must be Text");
    }

    try {
        std::string text = std::get<Text>(args[0]);
        std::string pattern = std::get<Text>(args[1]);
        RegexFlags flags = RegexFlags::NONE;

        if (args.size() == 3) {
            if (!std::holds_alternative<Text>(args[2])) {
                throw EvaluationError("split() flags argument must be Text");
            }
            flags = parseFlags(std::get<Text>(args[2]));
        }

        validatePattern(pattern);
        std::regex regex = compilePattern(pattern, flags);

        auto list = std::make_shared<ListInstance>();

        // Use regex_token_iterator to split
        std::sregex_token_iterator iter(text.begin(), text.end(), regex, -1);
        std::sregex_token_iterator end;

        for (; iter != end; ++iter) {
            std::string token = iter->str();
            if (!token.empty() || list->size() == 0) {  // Include empty first/last tokens
                list->add(Text(token));
            }
        }

        // Handle case where no matches found
        if (list->size() == 0) {
            list->add(Text(text));
        }

        return Value(list);

    } catch (const std::regex_error& e) {
        throw EvaluationError("split() regex error: " + std::string(e.what()));
    } catch (const std::exception& e) {
        throw EvaluationError("split() error: " + std::string(e.what()));
    }
}

// Advanced pattern methods implementation

Value RegexpLibrary::nativeGroups(const std::vector<Value>& args, Context& context) {
    if (args.size() < 2 || args.size() > 3) {
        throw EvaluationError(
            "groups() requires 2-3 arguments (text: Text, pattern: Text, flags?: Text)");
    }

    if (!std::holds_alternative<Text>(args[0]) || !std::holds_alternative<Text>(args[1])) {
        throw EvaluationError("groups() first two arguments must be Text");
    }

    try {
        std::string text = std::get<Text>(args[0]);
        std::string pattern = std::get<Text>(args[1]);
        RegexFlags flags = RegexFlags::NONE;

        if (args.size() == 3) {
            if (!std::holds_alternative<Text>(args[2])) {
                throw EvaluationError("groups() flags argument must be Text");
            }
            flags = parseFlags(std::get<Text>(args[2]));
        }

        validatePattern(pattern);
        std::regex regex = compilePattern(pattern, flags);

        std::smatch match;
        auto list = std::make_shared<ListInstance>();

        if (std::regex_search(text, match, regex)) {
            // Add full match as first element
            list->add(Text(match[0].str()));

            // Add capture groups
            for (size_t i = 1; i < match.size(); ++i) {
                list->add(Text(match[i].str()));
            }
        }

        return Value(list);

    } catch (const std::regex_error& e) {
        throw EvaluationError("groups() regex error: " + std::string(e.what()));
    } catch (const std::exception& e) {
        throw EvaluationError("groups() error: " + std::string(e.what()));
    }
}

Value RegexpLibrary::nativeTest(const std::vector<Value>& args, Context& context) {
    if (args.size() < 1 || args.size() > 2) {
        throw EvaluationError("test() requires 1-2 arguments (pattern: Text, flags?: Text)");
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("test() pattern argument must be Text");
    }

    try {
        std::string pattern = std::get<Text>(args[0]);
        RegexFlags flags = RegexFlags::NONE;

        if (args.size() == 2) {
            if (!std::holds_alternative<Text>(args[1])) {
                throw EvaluationError("test() flags argument must be Text");
            }
            flags = parseFlags(std::get<Text>(args[1]));
        }

        validatePattern(pattern);
        std::regex regex = compilePattern(pattern, flags);

        // Return the pattern as Text (for now - could be enhanced to return regex object)
        return Text(pattern);

    } catch (const std::regex_error& e) {
        throw EvaluationError("test() regex error: " + std::string(e.what()));
    } catch (const std::exception& e) {
        throw EvaluationError("test() error: " + std::string(e.what()));
    }
}

Value RegexpLibrary::nativeEscape(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("escape() requires exactly 1 argument (text: Text)");
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("escape() argument must be Text");
    }

    try {
        std::string text = std::get<Text>(args[0]);
        std::string escaped = escapeRegexChars(text);
        return Text(escaped);

    } catch (const std::exception& e) {
        throw EvaluationError("escape() error: " + std::string(e.what()));
    }
}

// Utility methods implementation

Value RegexpLibrary::nativeIsValid(const std::vector<Value>& args, Context& context) {
    if (args.size() < 1 || args.size() > 2) {
        throw EvaluationError("isValid() requires 1-2 arguments (pattern: Text, flags?: Text)");
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("isValid() pattern argument must be Text");
    }

    try {
        std::string pattern = std::get<Text>(args[0]);
        RegexFlags flags = RegexFlags::NONE;

        if (args.size() == 2) {
            if (!std::holds_alternative<Text>(args[1])) {
                throw EvaluationError("isValid() flags argument must be Text");
            }
            flags = parseFlags(std::get<Text>(args[1]));
        }

        std::regex regex = compilePattern(pattern, flags);
        return Bool(true);

    } catch (const std::regex_error& e) {
        return Bool(false);
    } catch (const std::exception& e) {
        return Bool(false);
    }
}

Value RegexpLibrary::nativeCount(const std::vector<Value>& args, Context& context) {
    if (args.size() < 2 || args.size() > 3) {
        throw EvaluationError(
            "count() requires 2-3 arguments (text: Text, pattern: Text, flags?: Text)");
    }

    if (!std::holds_alternative<Text>(args[0]) || !std::holds_alternative<Text>(args[1])) {
        throw EvaluationError("count() first two arguments must be Text");
    }

    try {
        std::string text = std::get<Text>(args[0]);
        std::string pattern = std::get<Text>(args[1]);
        RegexFlags flags = RegexFlags::NONE;

        if (args.size() == 3) {
            if (!std::holds_alternative<Text>(args[2])) {
                throw EvaluationError("count() flags argument must be Text");
            }
            flags = parseFlags(std::get<Text>(args[2]));
        }

        validatePattern(pattern);
        std::regex regex = compilePattern(pattern, flags);

        auto matches = findAllMatches(text, regex);
        return Int(static_cast<int>(matches.size()));

    } catch (const std::regex_error& e) {
        throw EvaluationError("count() regex error: " + std::string(e.what()));
    } catch (const std::exception& e) {
        throw EvaluationError("count() error: " + std::string(e.what()));
    }
}

Value RegexpLibrary::nativeExtract(const std::vector<Value>& args, Context& context) {
    if (args.size() < 2 || args.size() > 3) {
        throw EvaluationError(
            "extract() requires 2-3 arguments (text: Text, pattern: Text, flags?: Text)");
    }

    if (!std::holds_alternative<Text>(args[0]) || !std::holds_alternative<Text>(args[1])) {
        throw EvaluationError("extract() first two arguments must be Text");
    }

    try {
        std::string text = std::get<Text>(args[0]);
        std::string pattern = std::get<Text>(args[1]);
        RegexFlags flags = RegexFlags::NONE;

        if (args.size() == 3) {
            if (!std::holds_alternative<Text>(args[2])) {
                throw EvaluationError("extract() flags argument must be Text");
            }
            flags = parseFlags(std::get<Text>(args[2]));
        }

        validatePattern(pattern);
        std::regex regex = compilePattern(pattern, flags);

        auto list = std::make_shared<ListInstance>();

        // Find all matches and extract just the captured groups (not full matches)
        std::sregex_iterator iter(text.begin(), text.end(), regex);
        std::sregex_iterator end;

        for (; iter != end; ++iter) {
            const std::smatch& match = *iter;
            // Skip match[0] (full match), only add capture groups
            for (size_t i = 1; i < match.size(); ++i) {
                list->add(Text(match[i].str()));
            }
        }

        return Value(list);

    } catch (const std::regex_error& e) {
        throw EvaluationError("extract() regex error: " + std::string(e.what()));
    } catch (const std::exception& e) {
        throw EvaluationError("extract() error: " + std::string(e.what()));
    }
}

// Helper functions implementation

RegexFlags RegexpLibrary::parseFlags(const std::string& flag_string) {
    RegexFlags flags = RegexFlags::NONE;

    for (char c : flag_string) {
        switch (c) {
            case 'i':
            case 'I':
                flags = flags | RegexFlags::CASE_INSENSITIVE;
                break;
            case 'm':
            case 'M':
                flags = flags | RegexFlags::MULTILINE;
                break;
            case 's':
            case 'S':
                flags = flags | RegexFlags::DOT_ALL;
                break;
            case 'x':
            case 'X':
                flags = flags | RegexFlags::EXTENDED;
                break;
            case 'g':
            case 'G':
                flags = flags | RegexFlags::GLOBAL;
                break;
            default:
                // Ignore unknown flags
                break;
        }
    }

    return flags;
}

std::regex_constants::syntax_option_type RegexpLibrary::getRegexOptions(RegexFlags flags) {
    std::regex_constants::syntax_option_type options = std::regex_constants::ECMAScript;

    if ((flags & RegexFlags::CASE_INSENSITIVE) != RegexFlags::NONE) {
        options |= std::regex_constants::icase;
    }

    if ((flags & RegexFlags::MULTILINE) != RegexFlags::NONE) {
        // Note: C++ regex doesn't have direct multiline support like other languages
        // This would need custom handling in the pattern
    }

    if ((flags & RegexFlags::EXTENDED) != RegexFlags::NONE) {
        // Extended syntax not directly supported in std::regex
    }

    return options;
}

std::regex_constants::match_flag_type RegexpLibrary::getMatchOptions(RegexFlags flags) {
    std::regex_constants::match_flag_type options = std::regex_constants::match_default;

    // Add match-specific options here if needed

    return options;
}

std::regex RegexpLibrary::compilePattern(const std::string& pattern, RegexFlags flags) {
    try {
        auto options = getRegexOptions(flags);
        return std::regex(pattern, options);
    } catch (const std::regex_error& e) {
        throw std::regex_error(e.code());
    }
}

RegexMatch RegexpLibrary::findFirstMatch(const std::string& text, const std::regex& regex) {
    std::smatch match;
    if (std::regex_search(text, match, regex)) {
        RegexMatch result(match[0].str(), match.position(), match.length());

        // Add capture groups
        for (size_t i = 0; i < match.size(); ++i) {
            result.groups.push_back(match[i].str());
        }

        return result;
    }

    return RegexMatch();  // No match found
}

std::vector<RegexMatch> RegexpLibrary::findAllMatches(const std::string& text,
                                                      const std::regex& regex) {
    std::vector<RegexMatch> matches;

    std::sregex_iterator iter(text.begin(), text.end(), regex);
    std::sregex_iterator end;

    for (; iter != end; ++iter) {
        const std::smatch& match = *iter;
        RegexMatch result(match[0].str(), match.position(), match.length());

        // Add capture groups
        for (size_t i = 0; i < match.size(); ++i) {
            result.groups.push_back(match[i].str());
        }

        matches.push_back(result);
    }

    return matches;
}

std::string RegexpLibrary::escapeRegexChars(const std::string& text) {
    // Characters that need escaping in regex
    const std::string special_chars = "\\^$.*+?()[]{}|";
    std::string escaped;

    for (char c : text) {
        if (special_chars.find(c) != std::string::npos) {
            escaped += "\\";
        }
        escaped += c;
    }

    return escaped;
}

void RegexpLibrary::validatePattern(const std::string& pattern) {
    if (pattern.empty()) {
        throw EvaluationError("Regex pattern cannot be empty");
    }
    // Additional validation could be added here
}

void RegexpLibrary::validateText(const std::string& text) {
    // Text validation if needed
    // For now, any text is valid
}

}  // namespace o2l