# system.os API Reference

## Overview

The system.os module provides comprehensive operating system interaction capabilities for O²L programs. It offers system information queries, process management, resource monitoring, command execution, and environment access through a clean, cross-platform API. All operations are designed to work consistently across different operating systems.

## Import

```o2l
import system.os
# Or import all functions
import system.os.*
```

## Quick Example

```o2l
import system.os
import system.io

# Get basic system information
os_name: Text = os.getOSName()
username: Text = os.getUsername()
cpu_count: Int = os.getCPUCount()

io.print("Running on: %s", os_name)
io.print("User: %s", username)
io.print("CPU cores: %d", cpu_count)

# Execute a command
exit_code: Int = os.execute("echo 'Hello from O²L!'")
output: Text = os.executeWithOutput("date")
io.print("Current time: %s", output)
```

---

## Command Line Arguments

### `args() → List<Text>`

Returns the command line arguments passed to the program as a list of strings.

```o2l
# Access command line arguments
args: List<Text> = os.args()
argc: Int = os.argc()

io.print("Program received %d arguments:", argc)

# Print all arguments with their indices
iterator: ListIterator = args.iterator()
index: Int = 0
while (iterator.hasNext()) {
    arg: Text = iterator.next()
    io.print("args[%d] = '%s'", index, arg)
    index = index + 1
}

# Access specific arguments safely
if (argc > 0) {
    program_name: Text = args.get(0)  # Usually the program name
    io.print("Program name: %s", program_name)
}

if (argc > 1) {
    first_arg: Text = args.get(1)     # First user argument
    io.print("First argument: %s", first_arg)
}
```

### `argc() → Int`

Returns the number of command line arguments (including the program name).

```o2l
# Command line argument processing
argc: Int = os.argc()

if (argc < 2) {
    io.print("Usage: program <input_file> [options]")
    return 1
}

args: List<Text> = os.args()
input_file: Text = args.get(1)

# Process optional arguments
i: Int = 2
verbose: Bool = false
output_file: Text = ""

while (i < argc) {
    arg: Text = args.get(i)

    if (arg == "--verbose" || arg == "-v") {
        verbose = true
    } else if (arg == "--output" || arg == "-o") {
        if (i + 1 < argc) {
            i = i + 1
            output_file = args.get(i)
        }
    }

    i = i + 1
}

io.print("Processing file: %s", input_file)
if (verbose) {
    io.print("Verbose mode enabled")
}
if (output_file != "") {
    io.print("Output will be written to: %s", output_file)
}
```

---

## Environment Variables

### `getEnv(name: Text) → Text`

Retrieves the value of an environment variable by name.

```o2l
# Access common environment variables
home_dir: Text = os.getEnv("HOME")      # Unix/Linux/macOS
user_profile: Text = os.getEnv("USERPROFILE")  # Windows
path_var: Text = os.getEnv("PATH")
shell: Text = os.getEnv("SHELL")
lang: Text = os.getEnv("LANG")

io.print("Home directory: %s", home_dir)
io.print("System PATH: %s", path_var)
io.print("Default shell: %s", shell)
io.print("Language setting: %s", lang)

# Handle missing environment variables
custom_var: Text = os.getEnv("MY_CUSTOM_VAR")
if (custom_var == "") {
    io.print("MY_CUSTOM_VAR is not set")
} else {
    io.print("MY_CUSTOM_VAR = %s", custom_var)
}

# Common patterns for different platforms
method getConfigDir(): Text {
    # Try different platform-specific locations
    xdg_config: Text = os.getEnv("XDG_CONFIG_HOME")
    if (xdg_config != "") {
        return xdg_config
    }

    home: Text = os.getEnv("HOME")
    if (home != "") {
        return home + "/.config"
    }

    # Windows fallback
    appdata: Text = os.getEnv("APPDATA")
    if (appdata != "") {
        return appdata
    }

    return "/tmp"  # Ultimate fallback
}

config_dir: Text = getConfigDir()
io.print("Configuration directory: %s", config_dir)
```

---

## System Information

### `getOSName() → Text`

Returns the operating system name.

```o2l
# Get operating system information
os_name: Text = os.getOSName()
io.print("Operating System: %s", os_name)

# Platform-specific behavior
if (os_name == "Linux") {
    io.print("Running on Linux")
} else if (os_name == "Darwin") {
    io.print("Running on macOS")
} else if (os_name == "Windows") {
    io.print("Running on Windows")
} else {
    io.print("Running on unknown OS: %s", os_name)
}

# Use for conditional compilation/behavior
method getPlatformSpecificPath(): Text {
    os_name: Text = os.getOSName()

    if (os_name == "Windows") {
        return "C:\\Program Files\\MyApp"
    } else {
        return "/usr/local/bin/myapp"
    }
}
```

### `getOSVersion() → Text`

Returns the operating system version string.

```o2l
# Get detailed OS version information
os_version: Text = os.getOSVersion()
io.print("OS Version: %s", os_version)

# Parse version for compatibility checks
method checkOSCompatibility(): Bool {
    os_name: Text = os.getOSName()
    version: Text = os.getOSVersion()

    if (os_name == "Linux") {
        # Check for minimum kernel version
        return true  # Simplified check
    } else if (os_name == "Darwin") {
        # Check for minimum macOS version
        return true  # Simplified check
    }

    return true  # Default to compatible
}

compatible: Bool = checkOSCompatibility()
io.print("System compatibility: %o", compatible)
```

### `getArchitecture() → Text`

Returns the system architecture (e.g., "x86_64", "arm64").

```o2l
# Get system architecture
architecture: Text = os.getArchitecture()
io.print("System Architecture: %s", architecture)

# Architecture-specific optimizations
method chooseOptimization(): Text {
    arch: Text = os.getArchitecture()

    if (arch == "x86_64") {
        return "Intel x64 optimizations enabled"
    } else if (arch == "arm64" || arch == "aarch64") {
        return "ARM64 optimizations enabled"
    } else if (arch.contains("arm")) {
        return "ARM optimizations enabled"
    } else {
        return "Generic optimizations enabled"
    }
}

optimization_info: Text = chooseOptimization()
io.print(optimization_info)
```

### `getHostname() → Text`

Returns the system hostname.

```o2l
# Get system hostname
hostname: Text = os.getHostname()
io.print("Hostname: %s", hostname)

# Use hostname for logging or identification
method createLogEntry(message: Text): Text {
    hostname: Text = os.getHostname()
    username: Text = os.getUsername()
    pid: Int = os.getProcessId()

    # Format: [hostname:username:pid] message
    return "[" + hostname + ":" + username + ":" + pid.toString() + "] " + message
}

log_entry: Text = createLogEntry("Application started")
io.print("Log entry: %s", log_entry)
```

### `getUsername() → Text`

Returns the current username.

```o2l
# Get current user information
username: Text = os.getUsername()
io.print("Current user: %s", username)

# Create user-specific paths
method getUserDataDir(): Text {
    username: Text = os.getUsername()
    home: Text = os.getEnv("HOME")

    if (home != "") {
        return home + "/." + "myapp"
    } else {
        return "/tmp/" + username + "_myapp"
    }
}

user_data_dir: Text = getUserDataDir()
io.print("User data directory: %s", user_data_dir)

# User permissions check
method requireSpecificUser(required_user: Text): Bool {
    current_user: Text = os.getUsername()
    if (current_user != required_user) {
        io.error("This program must be run as user: %s", required_user)
        io.error("Current user: %s", current_user)
        return false
    }
    return true
}

# Example: require root user for system operations
if (!requireSpecificUser("root")) {
    io.print("Exiting due to insufficient privileges")
}
```

### `getCurrentDir() → Text`

Returns the current working directory path.

```o2l
# Get current working directory
current_dir: Text = os.getCurrentDir()
io.print("Current directory: %s", current_dir)

# Use for relative path operations
method resolveRelativePath(relative_path: Text): Text {
    current_dir: Text = os.getCurrentDir()

    # Simple path joining (in real implementation, use proper path utilities)
    if (current_dir.endsWith("/")) {
        return current_dir + relative_path
    } else {
        return current_dir + "/" + relative_path
    }
}

config_file: Text = resolveRelativePath("config/settings.ini")
io.print("Config file path: %s", config_file)

# Directory-based logic
method isInProjectDirectory(): Bool {
    current_dir: Text = os.getCurrentDir()
    return current_dir.contains("myproject") || current_dir.endsWith("/myproject")
}

in_project: Bool = isInProjectDirectory()
io.print("In project directory: %o", in_project)
```

---

## Process Information

### `getProcessId() → Int`

Returns the current process ID (PID).

```o2l
# Get process information
pid: Int = os.getProcessId()
ppid: Int = os.getParentProcessId()

io.print("Process ID: %d", pid)
io.print("Parent Process ID: %d", ppid)

# Create PID file for daemon processes
method createPidFile(): Bool {
    pid: Int = os.getProcessId()
    pid_file_path: Text = "/tmp/myapp.pid"

    # Write PID to file (using command execution)
    command: Text = "echo " + pid.toString() + " > " + pid_file_path
    exit_code: Int = os.execute(command)

    return exit_code == 0
}

pid_created: Bool = createPidFile()
io.print("PID file created: %o", pid_created)

# Process monitoring
method isProcessRunning(target_pid: Int): Bool {
    check_command: Text = "ps -p " + target_pid.toString()
    exit_code: Int = os.execute(check_command)
    return exit_code == 0
}

# Check if parent is still running
parent_running: Bool = isProcessRunning(ppid)
io.print("Parent process running: %o", parent_running)
```

### `getParentProcessId() → Int`

Returns the parent process ID (PPID).

```o2l
# Process hierarchy information
current_pid: Int = os.getProcessId()
parent_pid: Int = os.getParentProcessId()

io.print("Current process: %d", current_pid)
io.print("Parent process: %d", parent_pid)

# Check if we're running under a specific parent
method isRunningUnderShell(): Bool {
    # Get parent process name (simplified)
    parent_pid: Int = os.getParentProcessId()
    ps_command: Text = "ps -p " + parent_pid.toString() + " -o comm="
    parent_name: Text = os.executeWithOutput(ps_command)

    return parent_name.contains("bash") || parent_name.contains("zsh") ||
           parent_name.contains("sh") || parent_name.contains("cmd")
}

under_shell: Bool = isRunningUnderShell()
io.print("Running under shell: %o", under_shell)

# Orphan process detection
method checkOrphanStatus(): Text {
    ppid: Int = os.getParentProcessId()

    if (ppid == 1) {
        return "Process is orphaned (parent is init)"
    } else {
        return "Process has normal parent: " + ppid.toString()
    }
}

orphan_status: Text = checkOrphanStatus()
io.print("Orphan status: %s", orphan_status)
```

### `getUptime() → Long`

Returns the system uptime in seconds.

```o2l
# Get system uptime
uptime_seconds: Long = os.getUptime()
io.print("System uptime: %s seconds", uptime_seconds.toString())

# Convert to human-readable format
method formatUptime(seconds: Long): Text {
    days: Long = seconds / 86400
    hours: Long = (seconds % 86400) / 3600
    minutes: Long = (seconds % 3600) / 60
    secs: Long = seconds % 60

    result: Text = ""
    if (days > 0) {
        result = result + days.toString() + " days, "
    }
    if (hours > 0) {
        result = result + hours.toString() + " hours, "
    }
    if (minutes > 0) {
        result = result + minutes.toString() + " minutes, "
    }
    result = result + secs.toString() + " seconds"

    return result
}

formatted_uptime: Text = formatUptime(uptime_seconds)
io.print("System has been running for: %s", formatted_uptime)

# Uptime-based decisions
method isSystemFreshlyBooted(): Bool {
    uptime: Long = os.getUptime()
    return uptime < 300  # Less than 5 minutes
}

fresh_boot: Bool = isSystemFreshlyBooted()
if (fresh_boot) {
    io.print("System recently booted - running initialization")
} else {
    io.print("System has been running for a while")
}
```

---

## Memory Information

### `getTotalMemory() → Long`

Returns the total system memory in bytes.

```o2l
# Get memory information
total_memory: Long = os.getTotalMemory()
available_memory: Long = os.getAvailableMemory()
used_memory: Long = os.getUsedMemory()

io.print("Total Memory: %s bytes", total_memory.toString())
io.print("Available Memory: %s bytes", available_memory.toString())
io.print("Used Memory: %s bytes", used_memory.toString())

# Convert to human-readable units
method formatBytes(bytes: Long): Text {
    if (bytes >= 1073741824) {  # GB
        gb: Double = bytes.toDouble() / 1073741824.0
        return gb.toString() + " GB"
    } else if (bytes >= 1048576) {  # MB
        mb: Double = bytes.toDouble() / 1048576.0
        return mb.toString() + " MB"
    } else if (bytes >= 1024) {  # KB
        kb: Double = bytes.toDouble() / 1024.0
        return kb.toString() + " KB"
    } else {
        return bytes.toString() + " bytes"
    }
}

total_formatted: Text = formatBytes(total_memory)
available_formatted: Text = formatBytes(available_memory)
used_formatted: Text = formatBytes(used_memory)

io.print("Total Memory: %s", total_formatted)
io.print("Available Memory: %s", available_formatted)
io.print("Used Memory: %s", used_formatted)
```

### `getAvailableMemory() → Long`

Returns the available system memory in bytes.

```o2l
# Memory availability checking
available_memory: Long = os.getAvailableMemory()
required_memory: Long = 1073741824  # 1 GB

method checkMemoryRequirements(required: Long): Bool {
    available: Long = os.getAvailableMemory()

    if (available < required) {
        available_gb: Double = available.toDouble() / 1073741824.0
        required_gb: Double = required.toDouble() / 1073741824.0

        io.error("Insufficient memory available")
        io.error("Required: %.2f GB", required_gb)
        io.error("Available: %.2f GB", available_gb)
        return false
    }

    return true
}

memory_ok: Bool = checkMemoryRequirements(required_memory)
if (memory_ok) {
    io.print("Sufficient memory available for operation")
} else {
    io.print("Cannot proceed due to insufficient memory")
}

# Memory pressure detection
method detectMemoryPressure(): Text {
    total: Long = os.getTotalMemory()
    available: Long = os.getAvailableMemory()

    pressure_ratio: Double = available.toDouble() / total.toDouble()

    if (pressure_ratio < 0.05) {  # Less than 5% available
        return "Critical memory pressure"
    } else if (pressure_ratio < 0.15) {  # Less than 15% available
        return "High memory pressure"
    } else if (pressure_ratio < 0.30) {  # Less than 30% available
        return "Moderate memory pressure"
    } else {
        return "Normal memory usage"
    }
}

pressure_status: Text = detectMemoryPressure()
io.print("Memory pressure: %s", pressure_status)
```

### `getUsedMemory() → Long`

Returns the used system memory in bytes.

```o2l
# Memory usage analysis
total_memory: Long = os.getTotalMemory()
used_memory: Long = os.getUsedMemory()
available_memory: Long = os.getAvailableMemory()

# Calculate usage percentage
usage_percentage: Double = (used_memory.toDouble() / total_memory.toDouble()) * 100.0

io.print("Memory Usage: %.1f%%", usage_percentage)

# Memory usage categories
method categorizeMemoryUsage(usage_percent: Double): Text {
    if (usage_percent < 50.0) {
        return "Light usage"
    } else if (usage_percent < 70.0) {
        return "Moderate usage"
    } else if (usage_percent < 85.0) {
        return "Heavy usage"
    } else if (usage_percent < 95.0) {
        return "Critical usage"
    } else {
        return "Memory exhaustion"
    }
}

usage_category: Text = categorizeMemoryUsage(usage_percentage)
io.print("Memory usage category: %s", usage_category)

# Memory trend monitoring
Object MemoryMonitor {
    property samples: List<Long>
    property max_samples: Int

    constructor() {
        this.samples = []
        this.max_samples = 10
    }

    method addSample(): Void {
        current_usage: Long = os.getUsedMemory()
        this.samples.add(current_usage)

        # Keep only recent samples
        if (this.samples.size() > this.max_samples) {
            this.samples.removeAt(0)
        }
    }

    method getTrend(): Text {
        if (this.samples.size() < 2) {
            return "Insufficient data"
        }

        first_sample: Long = this.samples.get(0)
        last_sample: Long = this.samples.get(this.samples.size() - 1)

        if (last_sample > first_sample) {
            return "Memory usage increasing"
        } else if (last_sample < first_sample) {
            return "Memory usage decreasing"
        } else {
            return "Memory usage stable"
        }
    }
}

monitor: MemoryMonitor = new MemoryMonitor()
monitor.addSample()
```

### `getMemoryUsage() → Double`

Returns the memory usage percentage (0.0 to 100.0).

```o2l
# Direct memory usage percentage
memory_usage: Double = os.getMemoryUsage()
io.print("Current memory usage: %.1f%%", memory_usage)

# Memory usage alerts
method checkMemoryUsageAlerts(usage: Double): Void {
    if (usage > 90.0) {
        io.error("CRITICAL: Memory usage at %.1f%%", usage)
    } else if (usage > 80.0) {
        io.warn("WARNING: High memory usage at %.1f%%", usage)
    } else if (usage > 70.0) {
        io.print("INFO: Moderate memory usage at %.1f%%", usage)
    }
}

checkMemoryUsageAlerts(memory_usage)

# Memory usage over time
Object MemoryUsageTracker {
    property readings: List<Double>

    constructor() {
        this.readings = []
    }

    method recordReading(): Void {
        current: Double = os.getMemoryUsage()
        this.readings.add(current)
    }

    method getAverageUsage(): Double {
        if (this.readings.size() == 0) {
            return 0.0
        }

        total: Double = 0.0
        iterator: ListIterator = this.readings.iterator()
        while (iterator.hasNext()) {
            reading: Double = iterator.next()
            total = total + reading
        }

        return total / this.readings.size().toDouble()
    }

    method getPeakUsage(): Double {
        if (this.readings.size() == 0) {
            return 0.0
        }

        peak: Double = 0.0
        iterator: ListIterator = this.readings.iterator()
        while (iterator.hasNext()) {
            reading: Double = iterator.next()
            if (reading > peak) {
                peak = reading
            }
        }

        return peak
    }
}

tracker: MemoryUsageTracker = new MemoryUsageTracker()
tracker.recordReading()
```

---

## CPU Information

### `getCPUCount() → Int`

Returns the number of CPU cores available to the system.

```o2l
# Get CPU information
cpu_count: Int = os.getCPUCount()
io.print("Available CPU cores: %d", cpu_count)

# Parallel processing decisions
method determineThreadCount(): Int {
    cpu_count: Int = os.getCPUCount()

    # Use CPU count - 1 to leave one core for system tasks
    optimal_threads: Int = cpu_count - 1
    if (optimal_threads < 1) {
        optimal_threads = 1
    }

    return optimal_threads
}

thread_count: Int = determineThreadCount()
io.print("Optimal thread count for parallel processing: %d", thread_count)

# CPU-based optimization
method chooseSortingAlgorithm(): Text {
    cpu_count: Int = os.getCPUCount()

    if (cpu_count >= 8) {
        return "Parallel merge sort"
    } else if (cpu_count >= 4) {
        return "Multi-threaded quicksort"
    } else {
        return "Single-threaded quicksort"
    }
}

sort_algorithm: Text = chooseSortingAlgorithm()
io.print("Recommended sorting algorithm: %s", sort_algorithm)
```

### `getCPUModel() → Text`

Returns the CPU model/name string.

```o2l
# Get CPU model information
cpu_model: Text = os.getCPUModel()
io.print("CPU Model: %s", cpu_model)

# CPU-specific optimizations
method detectCPUFeatures(): Text {
    cpu_model: Text = os.getCPUModel()

    features: Text = "Detected features: "

    if (cpu_model.contains("Intel")) {
        features = features + "Intel optimizations, "
    } else if (cpu_model.contains("AMD")) {
        features = features + "AMD optimizations, "
    } else if (cpu_model.contains("ARM")) {
        features = features + "ARM optimizations, "
    }

    # Check for common CPU features (simplified)
    if (cpu_model.contains("64")) {
        features = features + "64-bit, "
    }

    return features
}

cpu_features: Text = detectCPUFeatures()
io.print(cpu_features)

# Performance tier classification
method classifyCPUPerformance(): Text {
    cpu_model: Text = os.getCPUModel()
    cpu_count: Int = os.getCPUCount()

    if (cpu_count >= 12) {
        return "High-performance workstation/server"
    } else if (cpu_count >= 8) {
        return "High-performance desktop"
    } else if (cpu_count >= 4) {
        return "Standard desktop/laptop"
    } else if (cpu_count >= 2) {
        return "Basic desktop/laptop"
    } else {
        return "Low-power device"
    }
}

performance_tier: Text = classifyCPUPerformance()
io.print("CPU Performance Tier: %s", performance_tier)
```

### `getCPUUsage() → Double`

Returns the current CPU usage percentage (0.0 to 100.0).

```o2l
# Monitor CPU usage
cpu_usage: Double = os.getCPUUsage()
io.print("Current CPU usage: %.1f%%", cpu_usage)

# CPU usage alerts
method checkCPUUsageAlerts(usage: Double): Void {
    if (usage > 95.0) {
        io.error("CRITICAL: CPU usage at %.1f%%", usage)
    } else if (usage > 80.0) {
        io.warn("WARNING: High CPU usage at %.1f%%", usage)
    } else if (usage > 60.0) {
        io.print("INFO: Moderate CPU usage at %.1f%%", usage)
    }
}

checkCPUUsageAlerts(cpu_usage)

# CPU throttling detection
method detectCPUThrottling(): Bool {
    # Take multiple samples to detect throttling
    sample1: Double = os.getCPUUsage()
    os.execute("sleep 1")  # Wait 1 second
    sample2: Double = os.getCPUUsage()

    # If usage drops significantly under load, might indicate throttling
    return sample1 > 90.0 && sample2 < 50.0
}

# Adaptive processing based on CPU load
method shouldProcessBatch(): Bool {
    current_usage: Double = os.getCPUUsage()
    return current_usage < 70.0  # Only process if CPU usage is below 70%
}

can_process: Bool = shouldProcessBatch()
io.print("Can process batch job: %o", can_process)
```

### `getLoadAverage() → List<Double>`

Returns the system load averages (1, 5, and 15-minute averages).

```o2l
# Get system load averages
load_averages: List<Double> = os.getLoadAverage()

if (load_averages.size() >= 3) {
    load_1min: Double = load_averages.get(0)
    load_5min: Double = load_averages.get(1)
    load_15min: Double = load_averages.get(2)

    io.print("Load average (1 min):  %.2f", load_1min)
    io.print("Load average (5 min):  %.2f", load_5min)
    io.print("Load average (15 min): %.2f", load_15min)

    # Interpret load averages
    cpu_count: Int = os.getCPUCount()
    optimal_load: Double = cpu_count.toDouble() * 0.7  # 70% of CPU capacity

    method interpretLoad(load: Double, duration: Text): Text {
        if (load > cpu_count.toDouble()) {
            return "Overloaded (" + duration + ")"
        } else if (load > optimal_load) {
            return "High load (" + duration + ")"
        } else if (load > cpu_count.toDouble() * 0.3) {
            return "Normal load (" + duration + ")"
        } else {
            return "Light load (" + duration + ")"
        }
    }

    load_1_status: Text = interpretLoad(load_1min, "1 min")
    load_5_status: Text = interpretLoad(load_5min, "5 min")
    load_15_status: Text = interpretLoad(load_15min, "15 min")

    io.print("Load status: %s, %s, %s", load_1_status, load_5_status, load_15_status)

    # Load trend analysis
    if (load_1min > load_5min) {
        io.print("Load is increasing (recent spike)")
    } else if (load_1min < load_5min * 0.8) {
        io.print("Load is decreasing (cooling down)")
    } else {
        io.print("Load is stable")
    }
}
```

---

## Command Execution

### `execute(command: Text) → Int`

Executes a command and returns the exit code.

```o2l
# Basic command execution
exit_code: Int = os.execute("echo 'Hello from command line!'")
io.print("Command exit code: %d", exit_code)

# File operations
create_file: Int = os.execute("touch /tmp/test_file.txt")
if (create_file == 0) {
    io.print("File created successfully")
} else {
    io.error("Failed to create file, exit code: %d", create_file)
}

# Directory operations
make_dir: Int = os.execute("mkdir -p /tmp/test_directory")
list_dir: Int = os.execute("ls -la /tmp/test_directory")
remove_dir: Int = os.execute("rmdir /tmp/test_directory")

# System maintenance commands
update_system: Int = os.execute("sudo apt update")  # Linux
# update_system: Int = os.execute("brew update")    # macOS

# Error handling with commands
method safeExecute(command: Text): Bool {
    exit_code: Int = os.execute(command)

    if (exit_code == 0) {
        io.print("Command succeeded: %s", command)
        return true
    } else {
        io.error("Command failed with exit code %d: %s", exit_code, command)
        return false
    }
}

success: Bool = safeExecute("whoami")
if (!success) {
    io.print("Unable to get current user")
}
```

### `executeWithOutput(command: Text) → Text`

Executes a command and returns its output as a string.

```o2l
# Capture command output
current_date: Text = os.executeWithOutput("date")
io.print("Current date and time: %s", current_date)

# System information commands
kernel_version: Text = os.executeWithOutput("uname -r")
disk_usage: Text = os.executeWithOutput("df -h /")
memory_info: Text = os.executeWithOutput("free -h")

io.print("Kernel version: %s", kernel_version)
io.print("Disk usage:\n%s", disk_usage)
io.print("Memory info:\n%s", memory_info)

# Process information
running_processes: Text = os.executeWithOutput("ps aux | head -10")
io.print("Top processes:\n%s", running_processes)

# Network information
network_interfaces: Text = os.executeWithOutput("ip addr show")  # Linux
# network_interfaces: Text = os.executeWithOutput("ifconfig")     # macOS/BSD

# File content reading
file_content: Text = os.executeWithOutput("cat /etc/hostname")
io.print("System hostname from file: %s", file_content)

# Command chaining
combined_info: Text = os.executeWithOutput("echo 'System: ' && uname -s && echo 'User: ' && whoami")
io.print("Combined system info:\n%s", combined_info)

# Error handling with output capture
method safeExecuteWithOutput(command: Text): Text {
    try {
        output: Text = os.executeWithOutput(command)
        return output
    } catch (error: Exception) {
        io.error("Failed to execute command: %s", command)
        return ""
    }
}

safe_output: Text = safeExecuteWithOutput("ls /nonexistent")
if (safe_output == "") {
    io.print("Command produced no output or failed")
}
```

### `executeWithTimeout(command: Text, timeout_seconds: Int) → Text`

Executes a command with a timeout and returns its output.

```o2l
# Execute commands with timeout protection
quick_command: Text = os.executeWithTimeout("echo 'Fast command'", 5)
io.print("Quick command output: %s", quick_command)

# Prevent hanging on slow commands
slow_command: Text = os.executeWithTimeout("sleep 2 && echo 'Completed within timeout'", 5)
io.print("Slow command output: %s", slow_command)

# Network operations with timeout
ping_result: Text = os.executeWithTimeout("ping -c 3 google.com", 10)
io.print("Ping result: %s", ping_result)

# File download with timeout
download_result: Text = os.executeWithTimeout("curl -s http://example.com", 15)
io.print("Download size: %d characters", download_result.length())

# Database operations with timeout (example)
db_query: Text = os.executeWithTimeout("mysql -e 'SELECT NOW()' mydb", 30)

# Timeout handling patterns
method executeWithRetry(command: Text, timeout: Int, max_retries: Int): Text {
    retries: Int = 0

    while (retries < max_retries) {
        try {
            result: Text = os.executeWithTimeout(command, timeout)
            if (result != "") {
                return result
            }
        } catch (error: Exception) {
            io.warn("Command attempt %d failed: %s", retries + 1, command)
        }

        retries = retries + 1
        if (retries < max_retries) {
            io.print("Retrying in 2 seconds...")
            os.execute("sleep 2")
        }
    }

    io.error("Command failed after %d attempts: %s", max_retries, command)
    return ""
}

# Use retry mechanism for unreliable commands
reliable_result: Text = executeWithRetry("unstable-network-command", 10, 3)
```

### `executeAsync(command: Text) → Int`

Executes a command asynchronously and returns the process ID.

```o2l
# Asynchronous command execution
async_pid: Int = os.executeAsync("long-running-process > /tmp/output.log 2>&1")
io.print("Started async process with PID: %d", async_pid)

# Background file processing
backup_pid: Int = os.executeAsync("tar -czf /tmp/backup.tar.gz /home/user/documents")
io.print("Backup process started with PID: %d", backup_pid)

# Parallel processing
pid1: Int = os.executeAsync("process_chunk_1.sh")
pid2: Int = os.executeAsync("process_chunk_2.sh")
pid3: Int = os.executeAsync("process_chunk_3.sh")

io.print("Started parallel processes: %d, %d, %d", pid1, pid2, pid3)

# Monitor async process
method waitForProcess(pid: Int, max_wait_seconds: Int): Bool {
    waited: Int = 0

    while (waited < max_wait_seconds) {
        # Check if process is still running
        check_command: Text = "ps -p " + pid.toString() + " > /dev/null 2>&1"
        exit_code: Int = os.execute(check_command)

        if (exit_code != 0) {
            # Process has finished
            io.print("Process %d completed after %d seconds", pid, waited)
            return true
        }

        os.execute("sleep 1")
        waited = waited + 1
    }

    # Timeout reached
    io.warn("Process %d still running after %d seconds", pid, max_wait_seconds)
    return false
}

# Wait for completion
process_completed: Bool = waitForProcess(async_pid, 60)
if (process_completed) {
    # Read the output file
    result: Text = os.executeWithOutput("cat /tmp/output.log")
    io.print("Async process result:\n%s", result)
}

# Cleanup background processes
method killProcess(pid: Int): Bool {
    kill_command: Text = "kill " + pid.toString()
    exit_code: Int = os.execute(kill_command)
    return exit_code == 0
}

# Kill process if needed
if (!process_completed) {
    killed: Bool = killProcess(async_pid)
    io.print("Process %d killed: %o", async_pid, killed)
}
```

---

## Practical Examples

### System Monitoring Dashboard

```o2l
Object SystemMonitor {
    method generateSystemReport(): Text {
        report: Text = "=== SYSTEM MONITORING REPORT ===\n\n"

        # System Information
        report = report + "--- System Information ---\n"
        report = report + "OS: " + os.getOSName() + " " + os.getOSVersion() + "\n"
        report = report + "Architecture: " + os.getArchitecture() + "\n"
        report = report + "Hostname: " + os.getHostname() + "\n"
        report = report + "Current User: " + os.getUsername() + "\n"

        uptime: Long = os.getUptime()
        uptime_hours: Long = uptime / 3600
        report = report + "Uptime: " + uptime_hours.toString() + " hours\n\n"

        # Resource Usage
        report = report + "--- Resource Usage ---\n"
        memory_usage: Double = os.getMemoryUsage()
        cpu_usage: Double = os.getCPUUsage()

        report = report + "Memory Usage: " + memory_usage.toString() + "%\n"
        report = report + "CPU Usage: " + cpu_usage.toString() + "%\n"

        # Load Averages
        load_avg: List<Double> = os.getLoadAverage()
        if (load_avg.size() >= 3) {
            report = report + "Load Average: " + load_avg.get(0).toString() +
                     ", " + load_avg.get(1).toString() +
                     ", " + load_avg.get(2).toString() + "\n"
        }

        # Hardware Info
        report = report + "\n--- Hardware Information ---\n"
        cpu_count: Int = os.getCPUCount()
        cpu_model: Text = os.getCPUModel()
        total_memory: Long = os.getTotalMemory()
        memory_gb: Double = total_memory.toDouble() / 1073741824.0

        report = report + "CPU: " + cpu_model + " (" + cpu_count.toString() + " cores)\n"
        report = report + "Total Memory: " + memory_gb.toString() + " GB\n"

        return report
    }

    method checkSystemHealth(): Bool {
        memory_usage: Double = os.getMemoryUsage()
        cpu_usage: Double = os.getCPUUsage()

        healthy: Bool = true

        if (memory_usage > 90.0) {
            io.error("CRITICAL: Memory usage at %.1f%%", memory_usage)
            healthy = false
        }

        if (cpu_usage > 95.0) {
            io.error("CRITICAL: CPU usage at %.1f%%", cpu_usage)
            healthy = false
        }

        load_avg: List<Double> = os.getLoadAverage()
        if (load_avg.size() > 0) {
            current_load: Double = load_avg.get(0)
            cpu_count: Int = os.getCPUCount()

            if (current_load > cpu_count.toDouble() * 2.0) {
                io.error("CRITICAL: System overloaded (load: %.2f)", current_load)
                healthy = false
            }
        }

        return healthy
    }
}

# Usage
monitor: SystemMonitor = new SystemMonitor()
system_report: Text = monitor.generateSystemReport()
io.print(system_report)

system_healthy: Bool = monitor.checkSystemHealth()
io.print("System health status: %s", system_healthy ? "HEALTHY" : "UNHEALTHY")
```

### Cross-Platform Script Execution

```o2l
Object CrossPlatformExecutor {
    method executeByPlatform(linux_cmd: Text, macos_cmd: Text, windows_cmd: Text): Text {
        os_name: Text = os.getOSName()

        command: Text = ""
        if (os_name == "Linux") {
            command = linux_cmd
        } else if (os_name == "Darwin") {
            command = macos_cmd
        } else if (os_name == "Windows") {
            command = windows_cmd
        } else {
            return "Unsupported platform: " + os_name
        }

        if (command != "") {
            output: Text = os.executeWithOutput(command)
            return output
        } else {
            return "No command defined for platform: " + os_name
        }
    }

    method getSystemProcesses(): Text {
        return this.executeByPlatform(
            "ps aux",                    # Linux
            "ps aux",                    # macOS
            "tasklist"                   # Windows
        )
    }

    method getNetworkInfo(): Text {
        return this.executeByPlatform(
            "ip addr show",              # Linux
            "ifconfig",                  # macOS
            "ipconfig /all"              # Windows
        )
    }

    method getDiskUsage(): Text {
        return this.executeByPlatform(
            "df -h",                     # Linux
            "df -h",                     # macOS
            "wmic logicaldisk get size,freespace,caption"  # Windows
        )
    }
}

# Usage
executor: CrossPlatformExecutor = new CrossPlatformExecutor()
processes: Text = executor.getSystemProcesses()
network: Text = executor.getNetworkInfo()
disk: Text = executor.getDiskUsage()

io.print("System Processes:\n%s", processes)
io.print("Network Information:\n%s", network)
io.print("Disk Usage:\n%s", disk)
```

### Environment Configuration Manager

```o2l
Object EnvironmentManager {
    method detectEnvironment(): Text {
        # Check common environment indicators
        if (os.getEnv("DOCKER_CONTAINER") != "") {
            return "Docker"
        }

        if (os.getEnv("KUBERNETES_SERVICE_HOST") != "") {
            return "Kubernetes"
        }

        if (os.getEnv("AWS_EXECUTION_ENV") != "") {
            return "AWS Lambda"
        }

        if (os.getEnv("CI") == "true") {
            return "CI/CD Pipeline"
        }

        # Check for development environment
        if (os.getEnv("DEVELOPMENT") == "true" || os.getCurrentDir().contains("dev")) {
            return "Development"
        }

        # Check for production indicators
        if (os.getEnv("PRODUCTION") == "true" || os.getEnv("NODE_ENV") == "production") {
            return "Production"
        }

        return "Unknown"
    }

    method configureByEnvironment(): Text {
        environment: Text = this.detectEnvironment()

        if (environment == "Development") {
            return this.setupDevelopmentConfig()
        } else if (environment == "Production") {
            return this.setupProductionConfig()
        } else if (environment == "Docker") {
            return this.setupDockerConfig()
        } else {
            return this.setupDefaultConfig()
        }
    }

    method setupDevelopmentConfig(): Text {
        # Development-specific configuration
        return "Development environment configured"
    }

    method setupProductionConfig(): Text {
        # Production-specific configuration
        return "Production environment configured"
    }

    method setupDockerConfig(): Text {
        # Docker-specific configuration
        return "Docker environment configured"
    }

    method setupDefaultConfig(): Text {
        # Default configuration
        return "Default environment configured"
    }

    method getEnvironmentVariables(): Text {
        important_vars: List<Text> = [
            "HOME", "USER", "PATH", "SHELL", "LANG", "PWD",
            "NODE_ENV", "ENVIRONMENT", "DEBUG", "PORT"
        ]

        result: Text = "Environment Variables:\n"

        var_iterator: ListIterator = important_vars.iterator()
        while (var_iterator.hasNext()) {
            var_name: Text = var_iterator.next()
            var_value: Text = os.getEnv(var_name)

            if (var_value != "") {
                result = result + var_name + "=" + var_value + "\n"
            }
        }

        return result
    }
}

# Usage
env_manager: EnvironmentManager = new EnvironmentManager()
environment_type: Text = env_manager.detectEnvironment()
config_result: Text = env_manager.configureByEnvironment()
env_vars: Text = env_manager.getEnvironmentVariables()

io.print("Environment: %s", environment_type)
io.print("Configuration: %s", config_result)
io.print(env_vars)
```

---

## Error Handling

```o2l
# Safe system operations
method safeSystemOperation(operation: Text): Bool {
    try {
        if (operation == "memory_check") {
            memory_usage: Double = os.getMemoryUsage()
            return memory_usage >= 0.0 && memory_usage <= 100.0
        } else if (operation == "cpu_check") {
            cpu_usage: Double = os.getCPUUsage()
            return cpu_usage >= 0.0 && cpu_usage <= 100.0
        } else if (operation == "process_info") {
            pid: Int = os.getProcessId()
            return pid > 0
        }

        return false
    } catch (error: Exception) {
        io.error("System operation '%s' failed: %s", operation, error.message)
        return false
    }
}

# Safe command execution with comprehensive error handling
method robustCommandExecution(command: Text, timeout: Int): Text {
    try {
        # First check if the command is safe to execute
        if (command.contains("rm -rf /") || command.contains("del /")) {
            io.error("Dangerous command blocked: %s", command)
            return ""
        }

        # Execute with timeout
        output: Text = os.executeWithTimeout(command, timeout)
        return output

    } catch (error: Exception) {
        io.error("Command execution failed: %s", error.message)

        # Try without timeout as fallback
        try {
            exit_code: Int = os.execute(command)
            if (exit_code == 0) {
                return "Command succeeded but produced no output"
            } else {
                return "Command failed with exit code: " + exit_code.toString()
            }
        } catch (fallback_error: Exception) {
            io.error("Fallback execution also failed: %s", fallback_error.message)
            return ""
        }
    }
}
```

---

## Performance Considerations

1. **Command Execution**: External commands have significant overhead
2. **System Queries**: Memory/CPU queries may be expensive on some systems
3. **Output Capture**: Large command outputs consume memory
4. **Async Operations**: Background processes require monitoring
5. **Cross-Platform**: Different platforms have varying performance characteristics

---

## Best Practices

1. **Error Handling**: Always check command exit codes and handle exceptions
2. **Resource Monitoring**: Regularly monitor system resources to prevent issues
3. **Command Validation**: Validate commands before execution to prevent security issues
4. **Timeout Usage**: Use timeouts for potentially long-running commands
5. **Cross-Platform**: Design for multiple operating systems when possible
6. **Environment Detection**: Adapt behavior based on the runtime environment
7. **Process Management**: Clean up background processes properly

---

## See Also

- [system.io API Reference](io.md) - Console input/output operations
- [system.fs API Reference](fs.md) - Filesystem operations
- [Text API Reference](../core/Text.md) - String manipulation for command output
- [List API Reference](../core/List.md) - Working with argument lists
- [Error Handling Guide](../../tutorials/intermediate/03-error-handling.md) - Exception handling patterns
