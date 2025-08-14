#include <gtest/gtest.h>

#include <memory>

#include "../src/Common/Exceptions.hpp"
#include "../src/Runtime/Context.hpp"
#include "../src/Runtime/DateTimeLibrary.hpp"
#include "../src/Runtime/Value.hpp"

using namespace o2l;

class DateTimeLibraryTest : public ::testing::Test {
   protected:
    Context context;

    // Helper method to create datetime object and call methods
    Value callDateTimeMethod(const std::string& method_name, const std::vector<Value>& args = {}) {
        auto datetime_object = DateTimeLibrary::createDateTimeObject();
        EXPECT_TRUE(datetime_object->hasMethod(method_name))
            << "Method " << method_name << " not found";
        return datetime_object->callMethod(method_name, args, context);
    }

    // Helper to check boolean results
    void expectBool(const Value& result, Bool expected) {
        ASSERT_TRUE(std::holds_alternative<Bool>(result));
        EXPECT_EQ(std::get<Bool>(result), expected);
    }

    // Helper to check integer results
    void expectInt(const Value& result, Int expected) {
        ASSERT_TRUE(std::holds_alternative<Int>(result));
        EXPECT_EQ(std::get<Int>(result), expected);
    }

    // Helper to check text results
    void expectText(const Value& result, const std::string& expected) {
        ASSERT_TRUE(std::holds_alternative<Text>(result));
        EXPECT_EQ(std::get<Text>(result), expected);
    }

    // Helper to check if result is a datetime string
    bool isDateTimeResult(const Value& result) {
        if (!std::holds_alternative<Text>(result)) {
            return false;
        }
        std::string text = std::get<Text>(result);
        return text.substr(0, 3) == "DT:";
    }
};

// Test current date/time functions
TEST_F(DateTimeLibraryTest, CurrentDateTimeFunctions) {
    // Test now() - should return current datetime
    Value now_result = callDateTimeMethod("now");
    EXPECT_TRUE(isDateTimeResult(now_result));

    // Test nowUTC() - should return current UTC datetime
    Value nowUTC_result = callDateTimeMethod("nowUTC");
    EXPECT_TRUE(isDateTimeResult(nowUTC_result));

    // Test today() - should return today's date at midnight
    Value today_result = callDateTimeMethod("today");
    EXPECT_TRUE(isDateTimeResult(today_result));

    // Test argument validation
    EXPECT_THROW(callDateTimeMethod("now", {Value(Int(1))}), EvaluationError);
    EXPECT_THROW(callDateTimeMethod("nowUTC", {Value(Int(1))}), EvaluationError);
    EXPECT_THROW(callDateTimeMethod("today", {Value(Int(1))}), EvaluationError);
}

// Test date/time creation functions
TEST_F(DateTimeLibraryTest, DateTimeCreation) {
    // Test create() with full parameters
    Value dt1 = callDateTimeMethod("create", {Value(Int(2024)), Value(Int(1)), Value(Int(15)),
                                              Value(Int(14)), Value(Int(30)), Value(Int(45))});
    EXPECT_TRUE(isDateTimeResult(dt1));

    // Test create() with date only
    Value dt2 = callDateTimeMethod("create", {Value(Int(2024)), Value(Int(1)), Value(Int(15))});
    EXPECT_TRUE(isDateTimeResult(dt2));

    // Test createDate()
    Value date_result =
        callDateTimeMethod("createDate", {Value(Int(2024)), Value(Int(1)), Value(Int(15))});
    EXPECT_TRUE(isDateTimeResult(date_result));

    // Test createTime()
    Value time_result = callDateTimeMethod("createTime", {Value(Int(14)), Value(Int(30))});
    EXPECT_TRUE(isDateTimeResult(time_result));

    // Test fromTimestamp()
    Value timestamp_result = callDateTimeMethod("fromTimestamp", {Value(Int(1705327845))});
    EXPECT_TRUE(isDateTimeResult(timestamp_result));

    // Test argument validation
    EXPECT_THROW(callDateTimeMethod("create", {Value(Int(2024))}),
                 EvaluationError);  // Too few args
    EXPECT_THROW(callDateTimeMethod("create", {Value(Int(2024)), Value(Int(13)), Value(Int(1))}),
                 EvaluationError);  // Invalid month
    EXPECT_THROW(callDateTimeMethod("createDate", {Value(Int(2024)), Value(Int(1))}),
                 EvaluationError);  // Too few args
    EXPECT_THROW(callDateTimeMethod("createTime", {Value(Int(25))}),
                 EvaluationError);  // Invalid hour
}

// Test fromISOString function
TEST_F(DateTimeLibraryTest, FromISOString) {
    // Test valid ISO strings
    Value iso1 = callDateTimeMethod("fromISOString", {Value(Text("2024-01-15"))});
    EXPECT_TRUE(isDateTimeResult(iso1));

    Value iso2 = callDateTimeMethod("fromISOString", {Value(Text("2024-01-15T14:30:45"))});
    EXPECT_TRUE(isDateTimeResult(iso2));

    Value iso3 = callDateTimeMethod("fromISOString", {Value(Text("2024-01-15T14:30:45.123Z"))});
    EXPECT_TRUE(isDateTimeResult(iso3));

    // Test invalid ISO strings
    EXPECT_THROW(callDateTimeMethod("fromISOString", {Value(Text("invalid-date"))}),
                 EvaluationError);
    EXPECT_THROW(callDateTimeMethod("fromISOString", {Value(Text("2024-13-01"))}),
                 EvaluationError);  // Invalid month

    // Test argument validation
    EXPECT_THROW(callDateTimeMethod("fromISOString", {}), EvaluationError);  // No arguments
    EXPECT_THROW(callDateTimeMethod("fromISOString", {Value(Int(123))}),
                 EvaluationError);  // Non-text argument
}

// Test formatting functions
TEST_F(DateTimeLibraryTest, FormattingFunctions) {
    // Create a test datetime
    Value test_dt = callDateTimeMethod("create", {Value(Int(2024)), Value(Int(1)), Value(Int(15)),
                                                  Value(Int(14)), Value(Int(30)), Value(Int(45))});

    // Test formatISO()
    Value iso_result = callDateTimeMethod("formatISO", {test_dt});
    ASSERT_TRUE(std::holds_alternative<Text>(iso_result));
    std::string iso_str = std::get<Text>(iso_result);
    EXPECT_TRUE(iso_str.find("2024-01-15T14:30:45") != std::string::npos);

    // Test toString()
    Value str_result = callDateTimeMethod("toString", {test_dt});
    ASSERT_TRUE(std::holds_alternative<Text>(str_result));
    std::string str_val = std::get<Text>(str_result);
    EXPECT_TRUE(str_val.find("2024-01-15") != std::string::npos);
    EXPECT_TRUE(str_val.find("14:30:45") != std::string::npos);

    // Test toDateString()
    Value date_str = callDateTimeMethod("toDateString", {test_dt});
    expectText(date_str, "2024-01-15");

    // Test toTimeString()
    Value time_str = callDateTimeMethod("toTimeString", {test_dt});
    expectText(time_str, "14:30:45");

    // Test format() with custom format
    Value formatted = callDateTimeMethod("format", {test_dt, Value(Text("%Y-%m-%d %H:%M:%S"))});
    ASSERT_TRUE(std::holds_alternative<Text>(formatted));

    // Test argument validation
    EXPECT_THROW(callDateTimeMethod("formatISO", {}), EvaluationError);
    EXPECT_THROW(callDateTimeMethod("toString", {}), EvaluationError);
    EXPECT_THROW(callDateTimeMethod("format", {test_dt}), EvaluationError);  // Missing format
}

// Test component extraction functions
TEST_F(DateTimeLibraryTest, ComponentExtraction) {
    // Create a test datetime: 2024-01-15 14:30:45
    Value test_dt = callDateTimeMethod("create", {Value(Int(2024)), Value(Int(1)), Value(Int(15)),
                                                  Value(Int(14)), Value(Int(30)), Value(Int(45))});

    // Test component extraction
    Value year = callDateTimeMethod("getYear", {test_dt});
    expectInt(year, 2024);

    Value month = callDateTimeMethod("getMonth", {test_dt});
    expectInt(month, 1);

    Value day = callDateTimeMethod("getDay", {test_dt});
    expectInt(day, 15);

    Value hour = callDateTimeMethod("getHour", {test_dt});
    expectInt(hour, 14);

    Value minute = callDateTimeMethod("getMinute", {test_dt});
    expectInt(minute, 30);

    Value second = callDateTimeMethod("getSecond", {test_dt});
    expectInt(second, 45);

    Value millisecond = callDateTimeMethod("getMillisecond", {test_dt});
    expectInt(millisecond, 0);

    Value dayOfWeek = callDateTimeMethod("getDayOfWeek", {test_dt});
    ASSERT_TRUE(std::holds_alternative<Int>(dayOfWeek));
    Int dow = std::get<Int>(dayOfWeek);
    EXPECT_GE(dow, 0);
    EXPECT_LE(dow, 6);

    // Test argument validation
    EXPECT_THROW(callDateTimeMethod("getYear", {}), EvaluationError);
    EXPECT_THROW(callDateTimeMethod("getMonth", {}), EvaluationError);
    EXPECT_THROW(callDateTimeMethod("getDay", {}), EvaluationError);
    EXPECT_THROW(callDateTimeMethod("getHour", {}), EvaluationError);
    EXPECT_THROW(callDateTimeMethod("getMinute", {}), EvaluationError);
    EXPECT_THROW(callDateTimeMethod("getSecond", {}), EvaluationError);
}

// Test date/time modification functions
TEST_F(DateTimeLibraryTest, DateTimeModification) {
    // Create a test datetime
    Value test_dt = callDateTimeMethod("create", {Value(Int(2024)), Value(Int(1)), Value(Int(15)),
                                                  Value(Int(12)), Value(Int(0)), Value(Int(0))});

    // Test addDays()
    Value plus_days = callDateTimeMethod("addDays", {test_dt, Value(Int(5))});
    EXPECT_TRUE(isDateTimeResult(plus_days));

    // Test addHours()
    Value plus_hours = callDateTimeMethod("addHours", {test_dt, Value(Int(3))});
    EXPECT_TRUE(isDateTimeResult(plus_hours));

    // Test addMinutes()
    Value plus_minutes = callDateTimeMethod("addMinutes", {test_dt, Value(Int(45))});
    EXPECT_TRUE(isDateTimeResult(plus_minutes));

    // Test addSeconds()
    Value plus_seconds = callDateTimeMethod("addSeconds", {test_dt, Value(Int(30))});
    EXPECT_TRUE(isDateTimeResult(plus_seconds));

    // Test addMilliseconds()
    Value plus_millis = callDateTimeMethod("addMilliseconds", {test_dt, Value(Int(500))});
    EXPECT_TRUE(isDateTimeResult(plus_millis));

    // Test argument validation
    EXPECT_THROW(callDateTimeMethod("addDays", {test_dt}), EvaluationError);  // Missing days
    EXPECT_THROW(callDateTimeMethod("addHours", {test_dt, Value(Text("invalid"))}),
                 EvaluationError);                                        // Invalid type
    EXPECT_THROW(callDateTimeMethod("addMinutes", {}), EvaluationError);  // No arguments
}

// Test comparison functions
TEST_F(DateTimeLibraryTest, DateTimeComparison) {
    // Create test datetimes
    Value dt1 = callDateTimeMethod("create", {Value(Int(2024)), Value(Int(1)), Value(Int(15)),
                                              Value(Int(12)), Value(Int(0)), Value(Int(0))});

    Value dt2 = callDateTimeMethod("create", {Value(Int(2024)), Value(Int(1)), Value(Int(16)),
                                              Value(Int(12)), Value(Int(0)), Value(Int(0))});

    Value dt3 = callDateTimeMethod("create", {Value(Int(2024)), Value(Int(1)), Value(Int(15)),
                                              Value(Int(12)), Value(Int(0)), Value(Int(0))});

    // Test isEqual()
    Value equal1 = callDateTimeMethod("isEqual", {dt1, dt3});
    expectBool(equal1, true);

    Value equal2 = callDateTimeMethod("isEqual", {dt1, dt2});
    expectBool(equal2, false);

    // Test isBefore()
    Value before1 = callDateTimeMethod("isBefore", {dt1, dt2});
    expectBool(before1, true);

    Value before2 = callDateTimeMethod("isBefore", {dt2, dt1});
    expectBool(before2, false);

    // Test isAfter()
    Value after1 = callDateTimeMethod("isAfter", {dt2, dt1});
    expectBool(after1, true);

    Value after2 = callDateTimeMethod("isAfter", {dt1, dt2});
    expectBool(after2, false);

    // Test argument validation
    EXPECT_THROW(callDateTimeMethod("isEqual", {dt1}), EvaluationError);
    EXPECT_THROW(callDateTimeMethod("isBefore", {dt1}), EvaluationError);
    EXPECT_THROW(callDateTimeMethod("isAfter", {dt1}), EvaluationError);
}

// Test utility functions
TEST_F(DateTimeLibraryTest, UtilityFunctions) {
    // Test isLeapYear()
    Value leap2024 = callDateTimeMethod("isLeapYear", {Value(Int(2024))});
    expectBool(leap2024, true);

    Value leap2023 = callDateTimeMethod("isLeapYear", {Value(Int(2023))});
    expectBool(leap2023, false);

    Value leap2000 = callDateTimeMethod("isLeapYear", {Value(Int(2000))});
    expectBool(leap2000, true);

    Value leap1900 = callDateTimeMethod("isLeapYear", {Value(Int(1900))});
    expectBool(leap1900, false);

    // Test daysInMonth()
    Value days_jan = callDateTimeMethod("daysInMonth", {Value(Int(2024)), Value(Int(1))});
    expectInt(days_jan, 31);

    Value days_feb_leap = callDateTimeMethod("daysInMonth", {Value(Int(2024)), Value(Int(2))});
    expectInt(days_feb_leap, 29);

    Value days_feb_normal = callDateTimeMethod("daysInMonth", {Value(Int(2023)), Value(Int(2))});
    expectInt(days_feb_normal, 28);

    // Test getTimestamp()
    Value test_dt = callDateTimeMethod("create", {Value(Int(2024)), Value(Int(1)), Value(Int(15)),
                                                  Value(Int(12)), Value(Int(0)), Value(Int(0))});

    Value timestamp = callDateTimeMethod("getTimestamp", {test_dt});
    ASSERT_TRUE(std::holds_alternative<Int>(timestamp));

    // Test weekend/weekday detection
    Value weekend_dt =
        callDateTimeMethod("create", {
                                         Value(Int(2024)), Value(Int(1)), Value(Int(14))  // Sunday
                                     });

    Value is_weekend = callDateTimeMethod("isWeekend", {weekend_dt});
    expectBool(is_weekend, true);

    Value is_weekday = callDateTimeMethod("isWeekday", {weekend_dt});
    expectBool(is_weekday, false);

    // Test argument validation
    EXPECT_THROW(callDateTimeMethod("isLeapYear", {}), EvaluationError);
    EXPECT_THROW(callDateTimeMethod("daysInMonth", {Value(Int(2024))}), EvaluationError);
    EXPECT_THROW(callDateTimeMethod("getTimestamp", {}), EvaluationError);
}

// Test calendar functions
TEST_F(DateTimeLibraryTest, CalendarFunctions) {
    // Create a test datetime in the middle of a day/month/year
    Value test_dt = callDateTimeMethod("create", {Value(Int(2024)), Value(Int(6)), Value(Int(15)),
                                                  Value(Int(14)), Value(Int(30)), Value(Int(45))});

    // Test startOfDay()
    Value start_day = callDateTimeMethod("startOfDay", {test_dt});
    EXPECT_TRUE(isDateTimeResult(start_day));

    Value start_hour = callDateTimeMethod("getHour", {start_day});
    expectInt(start_hour, 0);

    // Test endOfDay()
    Value end_day = callDateTimeMethod("endOfDay", {test_dt});
    EXPECT_TRUE(isDateTimeResult(end_day));

    Value end_hour = callDateTimeMethod("getHour", {end_day});
    expectInt(end_hour, 23);

    // Test startOfMonth()
    Value start_month = callDateTimeMethod("startOfMonth", {test_dt});
    EXPECT_TRUE(isDateTimeResult(start_month));

    Value start_day_num = callDateTimeMethod("getDay", {start_month});
    expectInt(start_day_num, 1);

    // Test endOfMonth()
    Value end_month = callDateTimeMethod("endOfMonth", {test_dt});
    EXPECT_TRUE(isDateTimeResult(end_month));

    Value end_day_num = callDateTimeMethod("getDay", {end_month});
    expectInt(end_day_num, 30);  // June has 30 days

    // Test startOfYear()
    Value start_year = callDateTimeMethod("startOfYear", {test_dt});
    EXPECT_TRUE(isDateTimeResult(start_year));

    Value start_month_num = callDateTimeMethod("getMonth", {start_year});
    expectInt(start_month_num, 1);

    Value start_year_day = callDateTimeMethod("getDay", {start_year});
    expectInt(start_year_day, 1);

    // Test endOfYear()
    Value end_year = callDateTimeMethod("endOfYear", {test_dt});
    EXPECT_TRUE(isDateTimeResult(end_year));

    Value end_month_num = callDateTimeMethod("getMonth", {end_year});
    expectInt(end_month_num, 12);

    Value end_year_day = callDateTimeMethod("getDay", {end_year});
    expectInt(end_year_day, 31);

    // Test argument validation
    EXPECT_THROW(callDateTimeMethod("startOfDay", {}), EvaluationError);
    EXPECT_THROW(callDateTimeMethod("endOfDay", {}), EvaluationError);
    EXPECT_THROW(callDateTimeMethod("startOfMonth", {}), EvaluationError);
    EXPECT_THROW(callDateTimeMethod("endOfMonth", {}), EvaluationError);
}

// Test method availability
TEST_F(DateTimeLibraryTest, MethodAvailability) {
    auto datetime_object = DateTimeLibrary::createDateTimeObject();

    // Test that all expected methods are available
    std::vector<std::string> expected_methods = {
        "now",          "nowUTC",        "today",         "create",          "createDate",
        "createTime",   "fromTimestamp", "fromISOString", "parse",           "parseISO",
        "parseFormat",  "format",        "formatISO",     "formatLocal",     "toString",
        "toDateString", "toTimeString",  "getYear",       "getMonth",        "getDay",
        "getHour",      "getMinute",     "getSecond",     "getMillisecond",  "getDayOfWeek",
        "getDayOfYear", "getWeekOfYear", "addYears",      "addMonths",       "addDays",
        "addHours",     "addMinutes",    "addSeconds",    "addMilliseconds", "subtract",
        "difference",   "daysBetween",   "hoursBetween",  "minutesBetween",  "secondsBetween",
        "isEqual",      "isBefore",      "isAfter",       "isBetween",       "isLeapYear",
        "daysInMonth",  "isWeekend",     "isWeekday",     "getTimestamp",    "getTimezone",
        "toUTC",        "toLocal",       "toTimezone",    "startOfDay",      "endOfDay",
        "startOfWeek",  "endOfWeek",     "startOfMonth",  "endOfMonth",      "startOfYear",
        "endOfYear"};

    for (const auto& method : expected_methods) {
        EXPECT_TRUE(datetime_object->hasMethod(method)) << "Method " << method << " not found";
        EXPECT_TRUE(datetime_object->isMethodExternal(method))
            << "Method " << method << " not external";
    }
}

// Test error handling for not-yet-implemented methods
TEST_F(DateTimeLibraryTest, NotImplementedMethods) {
    Value test_dt = callDateTimeMethod("create", {Value(Int(2024)), Value(Int(1)), Value(Int(15))});

    // Test methods that should throw "not yet implemented" errors
    EXPECT_THROW(callDateTimeMethod("parse", {Value(Text("2024-01-15"))}), EvaluationError);
    EXPECT_THROW(
        callDateTimeMethod("parseFormat", {Value(Text("2024-01-15")), Value(Text("%Y-%m-%d"))}),
        EvaluationError);
    EXPECT_THROW(callDateTimeMethod("getDayOfYear", {test_dt}), EvaluationError);
    EXPECT_THROW(callDateTimeMethod("getWeekOfYear", {test_dt}), EvaluationError);
    EXPECT_THROW(callDateTimeMethod("addYears", {test_dt, Value(Int(1))}), EvaluationError);
    EXPECT_THROW(callDateTimeMethod("addMonths", {test_dt, Value(Int(1))}), EvaluationError);
    EXPECT_THROW(callDateTimeMethod("subtract", {test_dt, test_dt}), EvaluationError);
    EXPECT_THROW(callDateTimeMethod("difference", {test_dt, test_dt}), EvaluationError);
    EXPECT_THROW(callDateTimeMethod("daysBetween", {test_dt, test_dt}), EvaluationError);
    EXPECT_THROW(callDateTimeMethod("hoursBetween", {test_dt, test_dt}), EvaluationError);
    EXPECT_THROW(callDateTimeMethod("minutesBetween", {test_dt, test_dt}), EvaluationError);
    EXPECT_THROW(callDateTimeMethod("secondsBetween", {test_dt, test_dt}), EvaluationError);
    EXPECT_THROW(callDateTimeMethod("isBetween", {test_dt, test_dt, test_dt}), EvaluationError);
    EXPECT_THROW(callDateTimeMethod("getTimezone", {test_dt}), EvaluationError);
    EXPECT_THROW(callDateTimeMethod("toUTC", {test_dt}), EvaluationError);
    EXPECT_THROW(callDateTimeMethod("toLocal", {test_dt}), EvaluationError);
    EXPECT_THROW(callDateTimeMethod("toTimezone", {test_dt, Value(Text("UTC"))}), EvaluationError);
    EXPECT_THROW(callDateTimeMethod("startOfWeek", {test_dt}), EvaluationError);
    EXPECT_THROW(callDateTimeMethod("endOfWeek", {test_dt}), EvaluationError);
}

// Test edge cases and boundary conditions
TEST_F(DateTimeLibraryTest, EdgeCases) {
    // Test February 29th in leap year
    Value leap_day =
        callDateTimeMethod("create", {Value(Int(2024)), Value(Int(2)), Value(Int(29))});
    EXPECT_TRUE(isDateTimeResult(leap_day));

    // Test December 31st
    Value year_end =
        callDateTimeMethod("create", {Value(Int(2023)), Value(Int(12)), Value(Int(31))});
    EXPECT_TRUE(isDateTimeResult(year_end));

    // Test adding days across month boundaries
    Value month_end =
        callDateTimeMethod("create", {Value(Int(2024)), Value(Int(1)), Value(Int(31))});
    Value next_month = callDateTimeMethod("addDays", {month_end, Value(Int(1))});
    EXPECT_TRUE(isDateTimeResult(next_month));

    Value next_month_day = callDateTimeMethod("getDay", {next_month});
    expectInt(next_month_day, 1);

    Value next_month_month = callDateTimeMethod("getMonth", {next_month});
    expectInt(next_month_month, 2);

    // Test negative time additions
    Value base_time = callDateTimeMethod(
        "create", {Value(Int(2024)), Value(Int(1)), Value(Int(15)), Value(Int(12))});
    Value earlier_time = callDateTimeMethod("addHours", {base_time, Value(Int(-5))});
    EXPECT_TRUE(isDateTimeResult(earlier_time));

    Value earlier_hour = callDateTimeMethod("getHour", {earlier_time});
    expectInt(earlier_hour, 7);
}

// Test argument type validation
TEST_F(DateTimeLibraryTest, ArgumentTypeValidation) {
    // Test methods that require specific argument types
    EXPECT_THROW(callDateTimeMethod("fromTimestamp", {Value(Text("invalid"))}), EvaluationError);
    EXPECT_THROW(callDateTimeMethod("format", {Value(Text("DT:123:0")), Value(Int(123))}),
                 EvaluationError);
    EXPECT_THROW(callDateTimeMethod("isLeapYear", {Value(Text("2024"))}), EvaluationError);
    EXPECT_THROW(callDateTimeMethod("daysInMonth", {Value(Text("2024")), Value(Int(1))}),
                 EvaluationError);
    EXPECT_THROW(callDateTimeMethod("daysInMonth", {Value(Int(2024)), Value(Text("1"))}),
                 EvaluationError);
}