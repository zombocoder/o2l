#pragma once

#include <string>

namespace o2l {

// Source location information for debugging and error reporting
struct SourceLocation {
    std::string filename;
    int line_number;
    int column_number;

    SourceLocation(const std::string& file = "", int line = 0, int column = 0)
        : filename(file), line_number(line), column_number(column) {}

    std::string toString() const {
        if (filename.empty()) {
            if (line_number > 0) {
                return "line " + std::to_string(line_number) + ":" + std::to_string(column_number);
            }
            return "unknown location";
        }
        return filename + ":" + std::to_string(line_number) + ":" + std::to_string(column_number);
    }
};

}  // namespace o2l