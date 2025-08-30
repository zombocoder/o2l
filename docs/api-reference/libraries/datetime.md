# DateTime Library

The DateTime library provides comprehensive date and time manipulation capabilities for O²L applications.

## Import

```obq
import datetime
```

## Core Functions

### Current Date and Time

#### `now() -> Text`
Returns the current date and time.

```obq
current_dt: Text = datetime.now()
io.print("Current time: %s", datetime.toString(current_dt))
```

#### `today() -> Text`
Returns the current date at midnight (00:00:00).

```obq
today_dt: Text = datetime.today()
io.print("Today at midnight: %s", datetime.toString(today_dt))
```

### Date Creation

#### `create(year: Int, month: Int, day: Int) -> Text`
Creates a date at midnight.

```obq
date: Text = datetime.create(2024, 6, 15)
io.print("Date: %s", datetime.toString(date))
```

#### `create(year: Int, month: Int, day: Int, hour: Int, minute: Int, second: Int) -> Text`
Creates a date and time with full specification.

```obq
dt: Text = datetime.create(2024, 6, 15, 14, 30, 45)
io.print("DateTime: %s", datetime.toString(dt))
```

### Component Extraction

#### `getYear(datetime: Text) -> Int`
Extracts the year from a datetime.

```obq
dt: Text = datetime.create(2024, 6, 15)
year: Int = datetime.getYear(dt)  # Returns 2024
```

#### `getMonth(datetime: Text) -> Int`
Extracts the month (1-12) from a datetime.

```obq
month: Int = datetime.getMonth(dt)  # Returns 6
```

#### `getDay(datetime: Text) -> Int`
Extracts the day of month from a datetime.

```obq
day: Int = datetime.getDay(dt)  # Returns 15
```

#### `getHour(datetime: Text) -> Int`
Extracts the hour (0-23) from a datetime.

```obq
dt: Text = datetime.create(2024, 6, 15, 14, 30, 45)
hour: Int = datetime.getHour(dt)  # Returns 14
```

#### `getMinute(datetime: Text) -> Int`
Extracts the minute (0-59) from a datetime.

```obq
minute: Int = datetime.getMinute(dt)  # Returns 30
```

#### `getSecond(datetime: Text) -> Int`
Extracts the second (0-59) from a datetime.

```obq
second: Int = datetime.getSecond(dt)  # Returns 45
```

## Formatting

### `toString(datetime: Text) -> Text`
Converts datetime to a human-readable string.

```obq
dt: Text = datetime.create(2024, 6, 15, 14, 30, 45)
str: Text = datetime.toString(dt)  # "2024-06-15 14:30:45"
```

### `toDateString(datetime: Text) -> Text`
Returns only the date portion as a string.

```obq
date_str: Text = datetime.toDateString(dt)  # "2024-06-15"
```

### `toTimeString(datetime: Text) -> Text`
Returns only the time portion as a string.

```obq
time_str: Text = datetime.toTimeString(dt)  # "14:30:45"
```

### `formatISO(datetime: Text) -> Text`
Formats datetime as an ISO 8601 string.

```obq
iso: Text = datetime.formatISO(dt)  # "2024-06-15T14:30:45.000Z"
```

## Parsing

### `fromISOString(iso: Text) -> Text`
Parses an ISO 8601 formatted string into a datetime.

```obq
# Full ISO format
dt1: Text = datetime.fromISOString("2024-06-15T14:30:45.123Z")

# Simple date format
dt2: Text = datetime.fromISOString("2024-06-15")
```

### `fromTimestamp(timestamp: Int) -> Text`
Creates a datetime from a Unix timestamp.

```obq
dt: Text = datetime.fromTimestamp(1718464245)
```

### `getTimestamp(datetime: Text) -> Int`
Converts datetime to a Unix timestamp.

```obq
dt: Text = datetime.create(2024, 6, 15, 14, 30, 45)
timestamp: Int = datetime.getTimestamp(dt)
```

## Date Arithmetic

### `addDays(datetime: Text, days: Int) -> Text`
Adds or subtracts days from a datetime.

```obq
dt: Text = datetime.create(2024, 6, 15)
future: Text = datetime.addDays(dt, 7)   # 7 days later
past: Text = datetime.addDays(dt, -3)    # 3 days ago
```

### `addHours(datetime: Text, hours: Int) -> Text`
Adds or subtracts hours from a datetime.

```obq
dt: Text = datetime.create(2024, 6, 15, 10, 0, 0)
later: Text = datetime.addHours(dt, 5)   # 5 hours later
```

### `addMinutes(datetime: Text, minutes: Int) -> Text`
Adds or subtracts minutes from a datetime.

```obq
dt: Text = datetime.create(2024, 6, 15, 10, 30, 0)
later: Text = datetime.addMinutes(dt, 15)  # 15 minutes later
```

### `addSeconds(datetime: Text, seconds: Int) -> Text`
Adds or subtracts seconds from a datetime.

```obq
dt: Text = datetime.create(2024, 6, 15, 10, 30, 30)
later: Text = datetime.addSeconds(dt, 45)  # 45 seconds later
```

## Comparison

### `isEqual(dt1: Text, dt2: Text) -> Bool`
Checks if two datetimes are equal.

```obq
dt1: Text = datetime.create(2024, 6, 15, 14, 30, 45)
dt2: Text = datetime.create(2024, 6, 15, 14, 30, 45)
equal: Bool = datetime.isEqual(dt1, dt2)  # true
```

### `isBefore(dt1: Text, dt2: Text) -> Bool`
Checks if the first datetime is before the second.

```obq
early: Text = datetime.create(2024, 6, 15)
late: Text = datetime.create(2024, 6, 16)
before: Bool = datetime.isBefore(early, late)  # true
```

### `isAfter(dt1: Text, dt2: Text) -> Bool`
Checks if the first datetime is after the second.

```obq
late: Text = datetime.create(2024, 6, 16)
early: Text = datetime.create(2024, 6, 15)
after: Bool = datetime.isAfter(late, early)  # true
```

## Calendar Functions

### `startOfDay(datetime: Text) -> Text`
Returns the start of the day (00:00:00) for the given date.

```obq
dt: Text = datetime.create(2024, 6, 15, 14, 30, 45)
start: Text = datetime.startOfDay(dt)  # 2024-06-15 00:00:00
```

### `endOfDay(datetime: Text) -> Text`
Returns the end of the day (23:59:59) for the given date.

```obq
dt: Text = datetime.create(2024, 6, 15, 14, 30, 45)
end: Text = datetime.endOfDay(dt)  # 2024-06-15 23:59:59
```

### `startOfMonth(datetime: Text) -> Text`
Returns the first day of the month at midnight.

```obq
dt: Text = datetime.create(2024, 6, 15)
start: Text = datetime.startOfMonth(dt)  # 2024-06-01 00:00:00
```

### `endOfMonth(datetime: Text) -> Text`
Returns the last day of the month at 23:59:59.

```obq
dt: Text = datetime.create(2024, 6, 15)
end: Text = datetime.endOfMonth(dt)  # 2024-06-30 23:59:59
```

### `startOfYear(datetime: Text) -> Text`
Returns January 1st of the year at midnight.

```obq
dt: Text = datetime.create(2024, 6, 15)
start: Text = datetime.startOfYear(dt)  # 2024-01-01 00:00:00
```

### `endOfYear(datetime: Text) -> Text`
Returns December 31st of the year at 23:59:59.

```obq
dt: Text = datetime.create(2024, 6, 15)
end: Text = datetime.endOfYear(dt)  # 2024-12-31 23:59:59
```

## Utility Functions

### `isLeapYear(year: Int) -> Bool`
Checks if a year is a leap year.

```obq
leap_2024: Bool = datetime.isLeapYear(2024)  # true
leap_2023: Bool = datetime.isLeapYear(2023)  # false
```

### `daysInMonth(year: Int, month: Int) -> Int`
Returns the number of days in a specific month and year.

```obq
days_feb_2024: Int = datetime.daysInMonth(2024, 2)  # 29
days_feb_2023: Int = datetime.daysInMonth(2023, 2)  # 28
days_jan: Int = datetime.daysInMonth(2024, 1)       # 31
```

### `isWeekend(datetime: Text) -> Bool`
Checks if the datetime falls on a weekend (Saturday or Sunday).

```obq
sunday: Text = datetime.create(2024, 6, 16)  # June 16, 2024 is Sunday
weekend: Bool = datetime.isWeekend(sunday)   # true
```

### `isWeekday(datetime: Text) -> Bool`
Checks if the datetime falls on a weekday (Monday through Friday).

```obq
monday: Text = datetime.create(2024, 6, 17)  # June 17, 2024 is Monday
weekday: Bool = datetime.isWeekday(monday)   # true
```

## Complete Example

```obq
import datetime
import system.io

Object DateTimeExample {
    method demonstrateDateTime(): Text {
        io.print("=== DateTime Library Example ===")
        
        # Current date and time
        now: Text = datetime.now()
        io.print("Current time: %s", datetime.toString(now))
        
        # Create specific datetime
        birthday: Text = datetime.create(1995, 6, 15, 14, 30, 0)
        io.print("Birthday: %s", datetime.formatISO(birthday))
        
        # Date arithmetic
        future: Text = datetime.addDays(now, 30)
        io.print("30 days from now: %s", datetime.toDateString(future))
        
        # Comparisons
        is_before: Bool = datetime.isBefore(birthday, now)
        io.print("Birthday is before now: %s", is_before)
        
        # Utility functions
        leap_year: Bool = datetime.isLeapYear(2024)
        days_feb: Int = datetime.daysInMonth(2024, 2)
        io.print("2024 is leap year: %s (Feb has %d days)", leap_year, days_feb)
        
        # Calendar boundaries
        start_day: Text = datetime.startOfDay(now)
        end_month: Text = datetime.endOfMonth(now)
        io.print("Start of today: %s", datetime.toString(start_day))
        io.print("End of this month: %s", datetime.toString(end_month))
        
        return "DateTime demonstration completed"
    }
}
```

## Error Handling

DateTime functions may throw errors for invalid inputs:
- Invalid dates (e.g., February 30th)
- Invalid time components (e.g., hour > 23)
- Malformed ISO strings
- Invalid timestamps

Always validate input parameters when working with user-provided date/time data.

## Time Zones

The DateTime library works with UTC time by default. For timezone-specific operations, convert times appropriately before using these functions.