#include "DateTimeLibrary.hpp"

#include <algorithm>
#include <regex>

#include "../Common/Exceptions.hpp"

namespace o2l {

std::shared_ptr<ObjectInstance> DateTimeLibrary::createDateTimeObject() {
    auto datetime_obj = std::make_shared<ObjectInstance>("datetime");

    // Current date/time functions
    datetime_obj->addMethod("now", now, true);
    datetime_obj->addMethod("nowUTC", nowUTC, true);
    datetime_obj->addMethod("today", today, true);

    // Date/time creation functions
    datetime_obj->addMethod("create", create, true);
    datetime_obj->addMethod("createDate", createDate, true);
    datetime_obj->addMethod("createTime", createTime, true);
    datetime_obj->addMethod("fromTimestamp", fromTimestamp, true);
    datetime_obj->addMethod("fromISOString", fromISOString, true);

    // Parsing functions
    datetime_obj->addMethod("parse", parse, true);
    datetime_obj->addMethod("parseISO", parseISO, true);
    datetime_obj->addMethod("parseFormat", parseFormat, true);

    // Formatting functions
    datetime_obj->addMethod("format", format, true);
    datetime_obj->addMethod("formatISO", formatISO, true);
    datetime_obj->addMethod("formatLocal", formatLocal, true);
    datetime_obj->addMethod("toString", toString, true);
    datetime_obj->addMethod("toDateString", toDateString, true);
    datetime_obj->addMethod("toTimeString", toTimeString, true);

    // Component extraction functions
    datetime_obj->addMethod("getYear", getYear, true);
    datetime_obj->addMethod("getMonth", getMonth, true);
    datetime_obj->addMethod("getDay", getDay, true);
    datetime_obj->addMethod("getHour", getHour, true);
    datetime_obj->addMethod("getMinute", getMinute, true);
    datetime_obj->addMethod("getSecond", getSecond, true);
    datetime_obj->addMethod("getMillisecond", getMillisecond, true);
    datetime_obj->addMethod("getDayOfWeek", getDayOfWeek, true);
    datetime_obj->addMethod("getDayOfYear", getDayOfYear, true);
    datetime_obj->addMethod("getWeekOfYear", getWeekOfYear, true);

    // Date/time modification functions
    datetime_obj->addMethod("addYears", addYears, true);
    datetime_obj->addMethod("addMonths", addMonths, true);
    datetime_obj->addMethod("addDays", addDays, true);
    datetime_obj->addMethod("addHours", addHours, true);
    datetime_obj->addMethod("addMinutes", addMinutes, true);
    datetime_obj->addMethod("addSeconds", addSeconds, true);
    datetime_obj->addMethod("addMilliseconds", addMilliseconds, true);

    // Date/time arithmetic functions
    datetime_obj->addMethod("subtract", subtract, true);
    datetime_obj->addMethod("difference", difference, true);
    datetime_obj->addMethod("daysBetween", daysBetween, true);
    datetime_obj->addMethod("hoursBetween", hoursBetween, true);
    datetime_obj->addMethod("minutesBetween", minutesBetween, true);
    datetime_obj->addMethod("secondsBetween", secondsBetween, true);

    // Comparison functions
    datetime_obj->addMethod("isEqual", isEqual, true);
    datetime_obj->addMethod("isBefore", isBefore, true);
    datetime_obj->addMethod("isAfter", isAfter, true);
    datetime_obj->addMethod("isBetween", isBetween, true);

    // Utility functions
    datetime_obj->addMethod("isLeapYear", isLeapYear, true);
    datetime_obj->addMethod("daysInMonth", daysInMonth, true);
    datetime_obj->addMethod("isWeekend", isWeekend, true);
    datetime_obj->addMethod("isWeekday", isWeekday, true);
    datetime_obj->addMethod("getTimestamp", getTimestamp, true);
    datetime_obj->addMethod("getTimezone", getTimezone, true);

    // Timezone functions
    datetime_obj->addMethod("toUTC", toUTC, true);
    datetime_obj->addMethod("toLocal", toLocal, true);
    datetime_obj->addMethod("toTimezone", toTimezone, true);

    // Calendar functions
    datetime_obj->addMethod("startOfDay", startOfDay, true);
    datetime_obj->addMethod("endOfDay", endOfDay, true);
    datetime_obj->addMethod("startOfWeek", startOfWeek, true);
    datetime_obj->addMethod("endOfWeek", endOfWeek, true);
    datetime_obj->addMethod("startOfMonth", startOfMonth, true);
    datetime_obj->addMethod("endOfMonth", endOfMonth, true);
    datetime_obj->addMethod("startOfYear", startOfYear, true);
    datetime_obj->addMethod("endOfYear", endOfYear, true);

    return datetime_obj;
}

// Current date/time functions
Value DateTimeLibrary::now(const std::vector<Value>& args, Context& context) {
    if (args.size() != 0) {
        throw EvaluationError("datetime.now() requires no arguments", context);
    }

    DateTime now_dt;
    return createDateTimeResult(now_dt);
}

Value DateTimeLibrary::nowUTC(const std::vector<Value>& args, Context& context) {
    if (args.size() != 0) {
        throw EvaluationError("datetime.nowUTC() requires no arguments", context);
    }

    DateTime now_dt;
    return createDateTimeResult(now_dt);
}

Value DateTimeLibrary::today(const std::vector<Value>& args, Context& context) {
    if (args.size() != 0) {
        throw EvaluationError("datetime.today() requires no arguments", context);
    }

    DateTime now_dt;
    auto tm = dateTimeToTm(now_dt);
    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;

    DateTime today_dt = tmToDateTime(tm);
    return createDateTimeResult(today_dt);
}

// Date/time creation functions
Value DateTimeLibrary::create(const std::vector<Value>& args, Context& context) {
    if (args.size() < 3 || args.size() > 7) {
        throw EvaluationError(
            "datetime.create() requires 3-7 arguments (year, month, day, [hour], [minute], "
            "[second], [millisecond])",
            context);
    }

    int year = static_cast<int>(std::get<Int>(args[0]));
    int month = static_cast<int>(std::get<Int>(args[1]));
    int day = static_cast<int>(std::get<Int>(args[2]));
    int hour = args.size() > 3 ? static_cast<int>(std::get<Int>(args[3])) : 0;
    int minute = args.size() > 4 ? static_cast<int>(std::get<Int>(args[4])) : 0;
    int second = args.size() > 5 ? static_cast<int>(std::get<Int>(args[5])) : 0;
    int millisecond = args.size() > 6 ? static_cast<int>(std::get<Int>(args[6])) : 0;

    if (!isValidDateTime(year, month, day, hour, minute, second)) {
        throw EvaluationError("Invalid date/time values provided to datetime.create()", context);
    }

    std::tm tm = {};
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    tm.tm_hour = hour;
    tm.tm_min = minute;
    tm.tm_sec = second;

    DateTime dt = tmToDateTime(tm);

    // Add milliseconds
    if (millisecond > 0) {
        dt.time_point += std::chrono::milliseconds(millisecond);
    }

    return createDateTimeResult(dt);
}

Value DateTimeLibrary::createDate(const std::vector<Value>& args, Context& context) {
    if (args.size() != 3) {
        throw EvaluationError("datetime.createDate() requires 3 arguments (year, month, day)",
                              context);
    }

    int year = static_cast<int>(std::get<Int>(args[0]));
    int month = static_cast<int>(std::get<Int>(args[1]));
    int day = static_cast<int>(std::get<Int>(args[2]));

    if (!isValidDateTime(year, month, day)) {
        throw EvaluationError("Invalid date values provided to datetime.createDate()", context);
    }

    std::tm tm = {};
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;

    DateTime dt = tmToDateTime(tm);
    return createDateTimeResult(dt);
}

Value DateTimeLibrary::createTime(const std::vector<Value>& args, Context& context) {
    if (args.size() < 2 || args.size() > 4) {
        throw EvaluationError(
            "datetime.createTime() requires 2-4 arguments (hour, minute, [second], [millisecond])",
            context);
    }

    int hour = static_cast<int>(std::get<Int>(args[0]));
    int minute = static_cast<int>(std::get<Int>(args[1]));
    int second = args.size() > 2 ? static_cast<int>(std::get<Int>(args[2])) : 0;
    int millisecond = args.size() > 3 ? static_cast<int>(std::get<Int>(args[3])) : 0;

    if (hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59 ||
        millisecond < 0 || millisecond > 999) {
        throw EvaluationError("Invalid time values provided to datetime.createTime()", context);
    }

    // Use today's date with specified time
    DateTime today_dt;
    auto tm = dateTimeToTm(today_dt);
    tm.tm_hour = hour;
    tm.tm_min = minute;
    tm.tm_sec = second;

    DateTime dt = tmToDateTime(tm);

    // Add milliseconds
    if (millisecond > 0) {
        dt.time_point += std::chrono::milliseconds(millisecond);
    }

    return createDateTimeResult(dt);
}

Value DateTimeLibrary::fromTimestamp(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("datetime.fromTimestamp() requires 1 argument (timestamp)", context);
    }

    double timestamp;
    if (std::holds_alternative<Int>(args[0])) {
        timestamp = static_cast<double>(std::get<Int>(args[0]));
    } else if (std::holds_alternative<Double>(args[0])) {
        timestamp = std::get<Double>(args[0]);
    } else if (std::holds_alternative<Float>(args[0])) {
        timestamp = static_cast<double>(std::get<Float>(args[0]));
    } else {
        throw EvaluationError("datetime.fromTimestamp() requires numeric argument", context);
    }

    auto time_point = std::chrono::system_clock::from_time_t(static_cast<std::time_t>(timestamp));
    DateTime dt(time_point);

    return createDateTimeResult(dt);
}

Value DateTimeLibrary::fromISOString(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("datetime.fromISOString() requires 1 argument (ISO string)", context);
    }

    if (!std::holds_alternative<Text>(args[0])) {
        throw EvaluationError("datetime.fromISOString() requires Text argument", context);
    }

    std::string isoStr = std::get<Text>(args[0]);

    // Parse ISO format: YYYY-MM-DDTHH:MM:SS[.sss][Z]
    std::regex iso_regex(
        R"((\d{4})-(\d{2})-(\d{2})(?:T(\d{2}):(\d{2}):(\d{2})(?:\.(\d{3}))?(?:Z)?)?)");
    std::smatch match;

    if (!std::regex_match(isoStr, match, iso_regex)) {
        throw EvaluationError("Invalid ISO string format in datetime.fromISOString()", context);
    }

    int year = std::stoi(match[1]);
    int month = std::stoi(match[2]);
    int day = std::stoi(match[3]);
    int hour = match[4].matched ? std::stoi(match[4]) : 0;
    int minute = match[5].matched ? std::stoi(match[5]) : 0;
    int second = match[6].matched ? std::stoi(match[6]) : 0;
    int millisecond = match[7].matched ? std::stoi(match[7]) : 0;

    if (!isValidDateTime(year, month, day, hour, minute, second)) {
        throw EvaluationError("Invalid date/time values in ISO string", context);
    }

    std::tm tm = {};
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    tm.tm_hour = hour;
    tm.tm_min = minute;
    tm.tm_sec = second;

    DateTime dt = tmToDateTime(tm);

    // Add milliseconds
    if (millisecond > 0) {
        dt.time_point += std::chrono::milliseconds(millisecond);
    }

    return createDateTimeResult(dt);
}

// Formatting functions
Value DateTimeLibrary::format(const std::vector<Value>& args, Context& context) {
    if (args.size() != 2) {
        throw EvaluationError("datetime.format() requires 2 arguments (datetime, format)", context);
    }

    DateTime dt = extractDateTime(args[0], "datetime.format", context);

    if (!std::holds_alternative<Text>(args[1])) {
        throw EvaluationError("datetime.format() requires Text format argument", context);
    }

    std::string format_str = std::get<Text>(args[1]);
    std::string formatted = formatDateTime(dt, format_str);

    return Value(Text(formatted));
}

Value DateTimeLibrary::formatISO(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("datetime.formatISO() requires 1 argument (datetime)", context);
    }

    DateTime dt = extractDateTime(args[0], "datetime.formatISO", context);
    auto tm = dateTimeToTm(dt);

    // Get milliseconds
    auto duration = dt.time_point.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration) % 1000;

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%dT%H:%M:%S");
    if (millis.count() > 0) {
        oss << "." << std::setfill('0') << std::setw(3) << millis.count();
    }
    oss << "Z";

    return Value(Text(oss.str()));
}

Value DateTimeLibrary::toString(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("datetime.toString() requires 1 argument (datetime)", context);
    }

    DateTime dt = extractDateTime(args[0], "datetime.toString", context);
    auto tm = dateTimeToTm(dt);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

    return Value(Text(oss.str()));
}

Value DateTimeLibrary::toDateString(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("datetime.toDateString() requires 1 argument (datetime)", context);
    }

    DateTime dt = extractDateTime(args[0], "datetime.toDateString", context);
    auto tm = dateTimeToTm(dt);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d");

    return Value(Text(oss.str()));
}

Value DateTimeLibrary::toTimeString(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("datetime.toTimeString() requires 1 argument (datetime)", context);
    }

    DateTime dt = extractDateTime(args[0], "datetime.toTimeString", context);
    auto tm = dateTimeToTm(dt);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%H:%M:%S");

    return Value(Text(oss.str()));
}

// Component extraction functions
Value DateTimeLibrary::getYear(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("datetime.getYear() requires 1 argument (datetime)", context);
    }

    DateTime dt = extractDateTime(args[0], "datetime.getYear", context);
    auto tm = dateTimeToTm(dt);

    return Value(Int(tm.tm_year + 1900));
}

Value DateTimeLibrary::getMonth(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("datetime.getMonth() requires 1 argument (datetime)", context);
    }

    DateTime dt = extractDateTime(args[0], "datetime.getMonth", context);
    auto tm = dateTimeToTm(dt);

    return Value(Int(tm.tm_mon + 1));  // Convert from 0-based to 1-based
}

Value DateTimeLibrary::getDay(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("datetime.getDay() requires 1 argument (datetime)", context);
    }

    DateTime dt = extractDateTime(args[0], "datetime.getDay", context);
    auto tm = dateTimeToTm(dt);

    return Value(Int(tm.tm_mday));
}

Value DateTimeLibrary::getHour(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("datetime.getHour() requires 1 argument (datetime)", context);
    }

    DateTime dt = extractDateTime(args[0], "datetime.getHour", context);
    auto tm = dateTimeToTm(dt);

    return Value(Int(tm.tm_hour));
}

Value DateTimeLibrary::getMinute(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("datetime.getMinute() requires 1 argument (datetime)", context);
    }

    DateTime dt = extractDateTime(args[0], "datetime.getMinute", context);
    auto tm = dateTimeToTm(dt);

    return Value(Int(tm.tm_min));
}

Value DateTimeLibrary::getSecond(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("datetime.getSecond() requires 1 argument (datetime)", context);
    }

    DateTime dt = extractDateTime(args[0], "datetime.getSecond", context);
    auto tm = dateTimeToTm(dt);

    return Value(Int(tm.tm_sec));
}

Value DateTimeLibrary::getMillisecond(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("datetime.getMillisecond() requires 1 argument (datetime)", context);
    }

    DateTime dt = extractDateTime(args[0], "datetime.getMillisecond", context);

    auto duration = dt.time_point.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration) % 1000;

    return Value(Int(static_cast<Int>(millis.count())));
}

Value DateTimeLibrary::getDayOfWeek(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("datetime.getDayOfWeek() requires 1 argument (datetime)", context);
    }

    DateTime dt = extractDateTime(args[0], "datetime.getDayOfWeek", context);
    auto tm = dateTimeToTm(dt);

    return Value(Int(tm.tm_wday));  // 0 = Sunday, 1 = Monday, etc.
}

// Date/time modification functions
Value DateTimeLibrary::addDays(const std::vector<Value>& args, Context& context) {
    if (args.size() != 2) {
        throw EvaluationError("datetime.addDays() requires 2 arguments (datetime, days)", context);
    }

    DateTime dt = extractDateTime(args[0], "datetime.addDays", context);

    if (!std::holds_alternative<Int>(args[1])) {
        throw EvaluationError("datetime.addDays() requires Int days argument", context);
    }

    Int days = std::get<Int>(args[1]);
    dt.time_point += std::chrono::hours(24 * days);

    return createDateTimeResult(dt);
}

Value DateTimeLibrary::addHours(const std::vector<Value>& args, Context& context) {
    if (args.size() != 2) {
        throw EvaluationError("datetime.addHours() requires 2 arguments (datetime, hours)",
                              context);
    }

    DateTime dt = extractDateTime(args[0], "datetime.addHours", context);

    if (!std::holds_alternative<Int>(args[1])) {
        throw EvaluationError("datetime.addHours() requires Int hours argument", context);
    }

    Int hours = std::get<Int>(args[1]);
    dt.time_point += std::chrono::hours(hours);

    return createDateTimeResult(dt);
}

Value DateTimeLibrary::addMinutes(const std::vector<Value>& args, Context& context) {
    if (args.size() != 2) {
        throw EvaluationError("datetime.addMinutes() requires 2 arguments (datetime, minutes)",
                              context);
    }

    DateTime dt = extractDateTime(args[0], "datetime.addMinutes", context);

    if (!std::holds_alternative<Int>(args[1])) {
        throw EvaluationError("datetime.addMinutes() requires Int minutes argument", context);
    }

    Int minutes = std::get<Int>(args[1]);
    dt.time_point += std::chrono::minutes(minutes);

    return createDateTimeResult(dt);
}

Value DateTimeLibrary::addSeconds(const std::vector<Value>& args, Context& context) {
    if (args.size() != 2) {
        throw EvaluationError("datetime.addSeconds() requires 2 arguments (datetime, seconds)",
                              context);
    }

    DateTime dt = extractDateTime(args[0], "datetime.addSeconds", context);

    if (!std::holds_alternative<Int>(args[1])) {
        throw EvaluationError("datetime.addSeconds() requires Int seconds argument", context);
    }

    Int seconds = std::get<Int>(args[1]);
    dt.time_point += std::chrono::seconds(seconds);

    return createDateTimeResult(dt);
}

// Comparison functions
Value DateTimeLibrary::isEqual(const std::vector<Value>& args, Context& context) {
    if (args.size() != 2) {
        throw EvaluationError("datetime.isEqual() requires 2 arguments (datetime1, datetime2)",
                              context);
    }

    DateTime dt1 = extractDateTime(args[0], "datetime.isEqual", context);
    DateTime dt2 = extractDateTime(args[1], "datetime.isEqual", context);

    return Value(Bool(dt1.time_point == dt2.time_point));
}

Value DateTimeLibrary::isBefore(const std::vector<Value>& args, Context& context) {
    if (args.size() != 2) {
        throw EvaluationError("datetime.isBefore() requires 2 arguments (datetime1, datetime2)",
                              context);
    }

    DateTime dt1 = extractDateTime(args[0], "datetime.isBefore", context);
    DateTime dt2 = extractDateTime(args[1], "datetime.isBefore", context);

    return Value(Bool(dt1.time_point < dt2.time_point));
}

Value DateTimeLibrary::isAfter(const std::vector<Value>& args, Context& context) {
    if (args.size() != 2) {
        throw EvaluationError("datetime.isAfter() requires 2 arguments (datetime1, datetime2)",
                              context);
    }

    DateTime dt1 = extractDateTime(args[0], "datetime.isAfter", context);
    DateTime dt2 = extractDateTime(args[1], "datetime.isAfter", context);

    return Value(Bool(dt1.time_point > dt2.time_point));
}

// Utility functions
Value DateTimeLibrary::isLeapYear(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("datetime.isLeapYear() requires 1 argument (year)", context);
    }

    if (!std::holds_alternative<Int>(args[0])) {
        throw EvaluationError("datetime.isLeapYear() requires Int year argument", context);
    }

    Int year = std::get<Int>(args[0]);
    bool is_leap = calculateIsLeapYear(static_cast<int>(year));

    return Value(Bool(is_leap));
}

Value DateTimeLibrary::getTimestamp(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("datetime.getTimestamp() requires 1 argument (datetime)", context);
    }

    DateTime dt = extractDateTime(args[0], "datetime.getTimestamp", context);
    std::time_t timestamp = dt.toTimeT();

    return Value(Int(static_cast<Int>(timestamp)));
}

// Helper functions
DateTime DateTimeLibrary::extractDateTime(const Value& value, const std::string& function_name,
                                          Context& context) {
    if (std::holds_alternative<Text>(value)) {
        // Assume it's an encoded datetime string
        std::string encoded = std::get<Text>(value);
        return decodeDateTime(encoded);
    }

    throw EvaluationError(function_name + " requires datetime argument", context);
}

Value DateTimeLibrary::createDateTimeResult(const DateTime& dt) {
    std::string encoded = encodeDateTime(dt);
    return Value(Text(encoded));
}

std::tm DateTimeLibrary::dateTimeToTm(const DateTime& dt) {
    std::time_t time_t_val = dt.toTimeT();
    return *std::gmtime(&time_t_val);  // Use UTC instead of local time
}

DateTime DateTimeLibrary::tmToDateTime(const std::tm& tm) {
    // Convert tm to time_t using UTC instead of local time
    std::tm tm_copy = tm;  // Make a copy since timegm might modify it

    // Manual calculation to avoid timezone issues
    // This is a simplified UTC conversion
    int year = tm_copy.tm_year + 1900;
    int month = tm_copy.tm_mon + 1;
    int day = tm_copy.tm_mday;
    int hour = tm_copy.tm_hour;
    int minute = tm_copy.tm_min;
    int second = tm_copy.tm_sec;

    // Calculate days since Unix epoch (1970-01-01)
    int days_since_epoch = 0;

    // Add days for complete years
    for (int y = 1970; y < year; y++) {
        days_since_epoch += calculateIsLeapYear(y) ? 366 : 365;
    }

    // Add days for complete months in current year
    for (int m = 1; m < month; m++) {
        days_since_epoch += calculateDaysInMonth(year, m);
    }

    // Add remaining days
    days_since_epoch += day - 1;  // -1 because day is 1-based

    // Calculate total seconds
    std::time_t total_seconds =
        days_since_epoch * 24 * 60 * 60 + hour * 60 * 60 + minute * 60 + second;

    return DateTime::fromTimeT(total_seconds);
}

std::string DateTimeLibrary::formatDateTime(const DateTime& dt, const std::string& format) {
    auto tm = dateTimeToTm(dt);
    std::ostringstream oss;
    oss << std::put_time(&tm, format.c_str());
    return oss.str();
}

bool DateTimeLibrary::isValidDateTime(int year, int month, int day, int hour, int minute,
                                      int second) {
    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > calculateDaysInMonth(year, month)) return false;
    if (hour < 0 || hour > 23) return false;
    if (minute < 0 || minute > 59) return false;
    if (second < 0 || second > 59) return false;

    return true;
}

int DateTimeLibrary::calculateDaysInMonth(int year, int month) {
    static const int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (month == 2 && calculateIsLeapYear(year)) {
        return 29;
    }

    return days_in_month[month - 1];
}

bool DateTimeLibrary::calculateIsLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

std::string DateTimeLibrary::encodeDateTime(const DateTime& dt) {
    std::time_t timestamp = dt.toTimeT();

    // Get milliseconds
    auto duration = dt.time_point.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration) % 1000;

    return "DT:" + std::to_string(timestamp) + ":" + std::to_string(millis.count());
}

DateTime DateTimeLibrary::decodeDateTime(const std::string& encoded) {
    if (encoded.substr(0, 3) != "DT:") {
        throw std::runtime_error("Invalid datetime encoding");
    }

    size_t first_colon = encoded.find(':', 3);
    if (first_colon == std::string::npos) {
        throw std::runtime_error("Invalid datetime encoding format");
    }

    std::string timestamp_str = encoded.substr(3, first_colon - 3);
    std::string millis_str = encoded.substr(first_colon + 1);

    std::time_t timestamp = std::stoll(timestamp_str);
    int millis = std::stoi(millis_str);

    DateTime dt = DateTime::fromTimeT(timestamp);
    dt.time_point += std::chrono::milliseconds(millis);

    return dt;
}

// Stub implementations for remaining functions
Value DateTimeLibrary::parse(const std::vector<Value>& args, Context& context) {
    throw EvaluationError("datetime.parse() not yet implemented", context);
}

Value DateTimeLibrary::parseISO(const std::vector<Value>& args, Context& context) {
    return fromISOString(args, context);  // Same as fromISOString
}

Value DateTimeLibrary::parseFormat(const std::vector<Value>& args, Context& context) {
    throw EvaluationError("datetime.parseFormat() not yet implemented", context);
}

Value DateTimeLibrary::formatLocal(const std::vector<Value>& args, Context& context) {
    return toString(args, context);  // Same as toString for now
}

Value DateTimeLibrary::getDayOfYear(const std::vector<Value>& args, Context& context) {
    throw EvaluationError("datetime.getDayOfYear() not yet implemented", context);
}

Value DateTimeLibrary::getWeekOfYear(const std::vector<Value>& args, Context& context) {
    throw EvaluationError("datetime.getWeekOfYear() not yet implemented", context);
}

Value DateTimeLibrary::addYears(const std::vector<Value>& args, Context& context) {
    throw EvaluationError("datetime.addYears() not yet implemented", context);
}

Value DateTimeLibrary::addMonths(const std::vector<Value>& args, Context& context) {
    throw EvaluationError("datetime.addMonths() not yet implemented", context);
}

Value DateTimeLibrary::addMilliseconds(const std::vector<Value>& args, Context& context) {
    if (args.size() != 2) {
        throw EvaluationError(
            "datetime.addMilliseconds() requires 2 arguments (datetime, milliseconds)", context);
    }

    DateTime dt = extractDateTime(args[0], "datetime.addMilliseconds", context);

    if (!std::holds_alternative<Int>(args[1])) {
        throw EvaluationError("datetime.addMilliseconds() requires Int milliseconds argument",
                              context);
    }

    Int milliseconds = std::get<Int>(args[1]);
    dt.time_point += std::chrono::milliseconds(milliseconds);

    return createDateTimeResult(dt);
}

Value DateTimeLibrary::subtract(const std::vector<Value>& args, Context& context) {
    throw EvaluationError("datetime.subtract() not yet implemented", context);
}

Value DateTimeLibrary::difference(const std::vector<Value>& args, Context& context) {
    throw EvaluationError("datetime.difference() not yet implemented", context);
}

Value DateTimeLibrary::daysBetween(const std::vector<Value>& args, Context& context) {
    throw EvaluationError("datetime.daysBetween() not yet implemented", context);
}

Value DateTimeLibrary::hoursBetween(const std::vector<Value>& args, Context& context) {
    throw EvaluationError("datetime.hoursBetween() not yet implemented", context);
}

Value DateTimeLibrary::minutesBetween(const std::vector<Value>& args, Context& context) {
    throw EvaluationError("datetime.minutesBetween() not yet implemented", context);
}

Value DateTimeLibrary::secondsBetween(const std::vector<Value>& args, Context& context) {
    throw EvaluationError("datetime.secondsBetween() not yet implemented", context);
}

Value DateTimeLibrary::isBetween(const std::vector<Value>& args, Context& context) {
    throw EvaluationError("datetime.isBetween() not yet implemented", context);
}

Value DateTimeLibrary::daysInMonth(const std::vector<Value>& args, Context& context) {
    if (args.size() != 2) {
        throw EvaluationError("datetime.daysInMonth() requires 2 arguments (year, month)", context);
    }

    if (!std::holds_alternative<Int>(args[0]) || !std::holds_alternative<Int>(args[1])) {
        throw EvaluationError("datetime.daysInMonth() requires Int arguments", context);
    }

    Int year = std::get<Int>(args[0]);
    Int month = std::get<Int>(args[1]);

    int days = calculateDaysInMonth(static_cast<int>(year), static_cast<int>(month));
    return Value(Int(days));
}

Value DateTimeLibrary::isWeekend(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("datetime.isWeekend() requires 1 argument (datetime)", context);
    }

    DateTime dt = extractDateTime(args[0], "datetime.isWeekend", context);
    auto tm = dateTimeToTm(dt);

    // tm_wday: 0 = Sunday, 6 = Saturday
    bool is_weekend = (tm.tm_wday == 0 || tm.tm_wday == 6);
    return Value(Bool(is_weekend));
}

Value DateTimeLibrary::isWeekday(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("datetime.isWeekday() requires 1 argument (datetime)", context);
    }

    DateTime dt = extractDateTime(args[0], "datetime.isWeekday", context);
    auto tm = dateTimeToTm(dt);

    // tm_wday: 1-5 = Monday-Friday
    bool is_weekday = (tm.tm_wday >= 1 && tm.tm_wday <= 5);
    return Value(Bool(is_weekday));
}

Value DateTimeLibrary::getTimezone(const std::vector<Value>& args, Context& context) {
    throw EvaluationError("datetime.getTimezone() not yet implemented", context);
}

Value DateTimeLibrary::toUTC(const std::vector<Value>& args, Context& context) {
    throw EvaluationError("datetime.toUTC() not yet implemented", context);
}

Value DateTimeLibrary::toLocal(const std::vector<Value>& args, Context& context) {
    throw EvaluationError("datetime.toLocal() not yet implemented", context);
}

Value DateTimeLibrary::toTimezone(const std::vector<Value>& args, Context& context) {
    throw EvaluationError("datetime.toTimezone() not yet implemented", context);
}

Value DateTimeLibrary::startOfDay(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("datetime.startOfDay() requires 1 argument (datetime)", context);
    }

    DateTime dt = extractDateTime(args[0], "datetime.startOfDay", context);
    auto tm = dateTimeToTm(dt);

    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;

    DateTime result = tmToDateTime(tm);
    return createDateTimeResult(result);
}

Value DateTimeLibrary::endOfDay(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("datetime.endOfDay() requires 1 argument (datetime)", context);
    }

    DateTime dt = extractDateTime(args[0], "datetime.endOfDay", context);
    auto tm = dateTimeToTm(dt);

    tm.tm_hour = 23;
    tm.tm_min = 59;
    tm.tm_sec = 59;

    DateTime result = tmToDateTime(tm);
    return createDateTimeResult(result);
}

Value DateTimeLibrary::startOfWeek(const std::vector<Value>& args, Context& context) {
    throw EvaluationError("datetime.startOfWeek() not yet implemented", context);
}

Value DateTimeLibrary::endOfWeek(const std::vector<Value>& args, Context& context) {
    throw EvaluationError("datetime.endOfWeek() not yet implemented", context);
}

Value DateTimeLibrary::startOfMonth(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("datetime.startOfMonth() requires 1 argument (datetime)", context);
    }

    DateTime dt = extractDateTime(args[0], "datetime.startOfMonth", context);
    auto tm = dateTimeToTm(dt);

    tm.tm_mday = 1;
    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;

    DateTime result = tmToDateTime(tm);
    return createDateTimeResult(result);
}

Value DateTimeLibrary::endOfMonth(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("datetime.endOfMonth() requires 1 argument (datetime)", context);
    }

    DateTime dt = extractDateTime(args[0], "datetime.endOfMonth", context);
    auto tm = dateTimeToTm(dt);

    int year = tm.tm_year + 1900;
    int month = tm.tm_mon + 1;
    int last_day = calculateDaysInMonth(year, month);

    tm.tm_mday = last_day;
    tm.tm_hour = 23;
    tm.tm_min = 59;
    tm.tm_sec = 59;

    DateTime result = tmToDateTime(tm);
    return createDateTimeResult(result);
}

Value DateTimeLibrary::startOfYear(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("datetime.startOfYear() requires 1 argument (datetime)", context);
    }

    DateTime dt = extractDateTime(args[0], "datetime.startOfYear", context);
    auto tm = dateTimeToTm(dt);

    tm.tm_mon = 0;   // January
    tm.tm_mday = 1;  // 1st
    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;

    DateTime result = tmToDateTime(tm);
    return createDateTimeResult(result);
}

Value DateTimeLibrary::endOfYear(const std::vector<Value>& args, Context& context) {
    if (args.size() != 1) {
        throw EvaluationError("datetime.endOfYear() requires 1 argument (datetime)", context);
    }

    DateTime dt = extractDateTime(args[0], "datetime.endOfYear", context);
    auto tm = dateTimeToTm(dt);

    tm.tm_mon = 11;   // December
    tm.tm_mday = 31;  // 31st
    tm.tm_hour = 23;
    tm.tm_min = 59;
    tm.tm_sec = 59;

    DateTime result = tmToDateTime(tm);
    return createDateTimeResult(result);
}

}  // namespace o2l