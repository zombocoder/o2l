#pragma once

#include <chrono>
#include <ctime>
#include <iomanip>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "Context.hpp"
#include "ObjectInstance.hpp"
#include "Value.hpp"

namespace o2l {

// DateTime structure to hold date/time information
struct DateTime {
    std::chrono::system_clock::time_point time_point;

    DateTime() : time_point(std::chrono::system_clock::now()) {}
    DateTime(std::chrono::system_clock::time_point tp) : time_point(tp) {}

    // Convert to time_t for easier manipulation
    std::time_t toTimeT() const {
        return std::chrono::system_clock::to_time_t(time_point);
    }

    // Create from time_t
    static DateTime fromTimeT(std::time_t t) {
        return DateTime(std::chrono::system_clock::from_time_t(t));
    }
};

class DateTimeLibrary {
   public:
    // Create the datetime object with native methods
    static std::shared_ptr<ObjectInstance> createDateTimeObject();

    // Current date/time functions
    static Value now(const std::vector<Value>& args, Context& context);
    static Value nowUTC(const std::vector<Value>& args, Context& context);
    static Value today(const std::vector<Value>& args, Context& context);

    // Date/time creation functions
    static Value create(const std::vector<Value>& args, Context& context);
    static Value createDate(const std::vector<Value>& args, Context& context);
    static Value createTime(const std::vector<Value>& args, Context& context);
    static Value fromTimestamp(const std::vector<Value>& args, Context& context);
    static Value fromISOString(const std::vector<Value>& args, Context& context);

    // Parsing functions
    static Value parse(const std::vector<Value>& args, Context& context);
    static Value parseISO(const std::vector<Value>& args, Context& context);
    static Value parseFormat(const std::vector<Value>& args, Context& context);

    // Formatting functions
    static Value format(const std::vector<Value>& args, Context& context);
    static Value formatISO(const std::vector<Value>& args, Context& context);
    static Value formatLocal(const std::vector<Value>& args, Context& context);
    static Value toString(const std::vector<Value>& args, Context& context);
    static Value toDateString(const std::vector<Value>& args, Context& context);
    static Value toTimeString(const std::vector<Value>& args, Context& context);

    // Component extraction functions
    static Value getYear(const std::vector<Value>& args, Context& context);
    static Value getMonth(const std::vector<Value>& args, Context& context);
    static Value getDay(const std::vector<Value>& args, Context& context);
    static Value getHour(const std::vector<Value>& args, Context& context);
    static Value getMinute(const std::vector<Value>& args, Context& context);
    static Value getSecond(const std::vector<Value>& args, Context& context);
    static Value getMillisecond(const std::vector<Value>& args, Context& context);
    static Value getDayOfWeek(const std::vector<Value>& args, Context& context);
    static Value getDayOfYear(const std::vector<Value>& args, Context& context);
    static Value getWeekOfYear(const std::vector<Value>& args, Context& context);

    // Date/time modification functions
    static Value addYears(const std::vector<Value>& args, Context& context);
    static Value addMonths(const std::vector<Value>& args, Context& context);
    static Value addDays(const std::vector<Value>& args, Context& context);
    static Value addHours(const std::vector<Value>& args, Context& context);
    static Value addMinutes(const std::vector<Value>& args, Context& context);
    static Value addSeconds(const std::vector<Value>& args, Context& context);
    static Value addMilliseconds(const std::vector<Value>& args, Context& context);

    // Date/time arithmetic functions
    static Value subtract(const std::vector<Value>& args, Context& context);
    static Value difference(const std::vector<Value>& args, Context& context);
    static Value daysBetween(const std::vector<Value>& args, Context& context);
    static Value hoursBetween(const std::vector<Value>& args, Context& context);
    static Value minutesBetween(const std::vector<Value>& args, Context& context);
    static Value secondsBetween(const std::vector<Value>& args, Context& context);

    // Comparison functions
    static Value isEqual(const std::vector<Value>& args, Context& context);
    static Value isBefore(const std::vector<Value>& args, Context& context);
    static Value isAfter(const std::vector<Value>& args, Context& context);
    static Value isBetween(const std::vector<Value>& args, Context& context);

    // Utility functions
    static Value isLeapYear(const std::vector<Value>& args, Context& context);
    static Value daysInMonth(const std::vector<Value>& args, Context& context);
    static Value isWeekend(const std::vector<Value>& args, Context& context);
    static Value isWeekday(const std::vector<Value>& args, Context& context);
    static Value getTimestamp(const std::vector<Value>& args, Context& context);
    static Value getTimezone(const std::vector<Value>& args, Context& context);

    // Timezone functions
    static Value toUTC(const std::vector<Value>& args, Context& context);
    static Value toLocal(const std::vector<Value>& args, Context& context);
    static Value toTimezone(const std::vector<Value>& args, Context& context);

    // Calendar functions
    static Value startOfDay(const std::vector<Value>& args, Context& context);
    static Value endOfDay(const std::vector<Value>& args, Context& context);
    static Value startOfWeek(const std::vector<Value>& args, Context& context);
    static Value endOfWeek(const std::vector<Value>& args, Context& context);
    static Value startOfMonth(const std::vector<Value>& args, Context& context);
    static Value endOfMonth(const std::vector<Value>& args, Context& context);
    static Value startOfYear(const std::vector<Value>& args, Context& context);
    static Value endOfYear(const std::vector<Value>& args, Context& context);

   private:
    // Helper functions
    static DateTime extractDateTime(const Value& value, const std::string& function_name,
                                    Context& context);
    static Value createDateTimeResult(const DateTime& dt);
    static std::tm dateTimeToTm(const DateTime& dt);
    static DateTime tmToDateTime(const std::tm& tm);
    static std::string formatDateTime(const DateTime& dt, const std::string& format);
    static DateTime parseDateTime(const std::string& dateStr, const std::string& format);
    static bool isValidDateTime(int year, int month, int day, int hour = 0, int minute = 0,
                                int second = 0);
    static int calculateDaysInMonth(int year, int month);
    static bool calculateIsLeapYear(int year);
    static int calculateDayOfWeek(int year, int month, int day);
    static int calculateDayOfYear(int year, int month, int day);
    static int calculateWeekOfYear(int year, int month, int day);

    // DateTime encoding/decoding for Value storage
    static std::string encodeDateTime(const DateTime& dt);
    static DateTime decodeDateTime(const std::string& encoded);
};

}  // namespace o2l