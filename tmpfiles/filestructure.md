# High-Performance Process Manager: File Structure

## Overview
This document outlines the proposed file structure for the high-performance process manager tool. The structure is modular, with each module represented by a corresponding header (`.h`) and implementation (`.cpp`) file to ensure scalability, maintainability, and readability.

### File Structure
```
process_manager/
├── include/
│   ├── process_manager.h
│   ├── process_listing.h
│   ├── resource_monitoring.h
│   ├── process_control.h
│   ├── command_parser.h
│   ├── logger.h
│   └── thread_pool.h
├── src/
│   ├── main.cpp
│   ├── process_manager.cpp
│   ├── process_listing.cpp
│   ├── resource_monitoring.cpp
│   ├── process_control.cpp
│   ├── command_parser.cpp
│   ├── logger.cpp
│   └── thread_pool.cpp
└── CMakeLists.txt
```

### Details of Each File

#### **1. Core Files**
##### **`main.cpp`**
- **Purpose**: Entry point of the program.
- **Functionality**:
  - Display the welcome screen.
  - Start the interactive loop.
  - Parse user commands and direct them to appropriate modules.

#### **2. Process Manager Files**
##### **`process_manager.h`**
- **Purpose**: Define the overall interface for the process manager.
- **Contents**:
  - Class declarations for `ProcessManager`.
  - Include key functionalities like starting the interactive loop, initializing modules, and handling general actions.

##### **`process_manager.cpp`**
- **Purpose**: Implement the `ProcessManager` class.
- **Contents**:
  - Methods for initializing modules and managing the command loop.
  - Handle command delegations to different modules.

#### **3. Command Parsing**
##### **`command_parser.h`**
- **Purpose**: Define the interface for parsing user commands.
- **Contents**:
  - Class `CommandParser` for breaking down user input and identifying which module to call.
  - Methods for validating input and handling different command types.

##### **`command_parser.cpp`**
- **Purpose**: Implement command parsing functionality.
- **Contents**:
  - Methods for parsing user input (`list`, `monitor`, `kill <pid>`, etc.).
  - Return parsed command or error messages.

#### **4. Process Listing Module**
##### **`process_listing.h`**
- **Purpose**: Define the interface for listing processes.
- **Contents**:
  - Class `ProcessListing`.
  - Function declarations for retrieving and organizing process data.

##### **`process_listing.cpp`**
- **Purpose**: Implement the process listing functionality.
- **Contents**:
  - Methods for gathering system data using multithreading.
  - Methods to display the list of processes, showing PID, CPU usage, and memory usage.

#### **5. Resource Monitoring Module**
##### **`resource_monitoring.h`**
- **Purpose**: Define the interface for real-time resource monitoring.
- **Contents**:
  - Class `ResourceMonitoring`.
  - Method declarations for starting and managing real-time monitoring of system resources.

##### **`resource_monitoring.cpp`**
- **Purpose**: Implement the real-time resource monitoring.
- **Contents**:
  - Start monitoring with a **thread pool** to ensure low latency.
  - Method to display real-time statistics and handle continuous user input to stop monitoring.

#### **6. Process Control Module**
##### **`process_control.h`**
- **Purpose**: Define the interface for controlling processes.
- **Contents**:
  - Class `ProcessControl`.
  - Method declarations for terminating a process by PID.

##### **`process_control.cpp`**
- **Purpose**: Implement the process control functionalities.
- **Contents**:
  - Methods to validate a PID and terminate the corresponding process.
  - Handle permission checks and log outcomes of termination attempts.

#### **7. Logging**
##### **`logger.h`**
- **Purpose**: Define the interface for logging user actions and system events.
- **Contents**:
  - Class `Logger`.
  - Methods to log actions (e.g., user commands, process terminations) and errors.

##### **`logger.cpp`**
- **Purpose**: Implement the logging functionalities.
- **Contents**:
  - Utilize **spdlog** (or similar) to create and manage logs.
  - Methods to write log entries with timestamps and severity levels.

#### **8. Thread Pool Module**
##### **`thread_pool.h`**
- **Purpose**: Define the interface for managing a thread pool.
- **Contents**:
  - Class `ThreadPool`.
  - Method declarations for adding tasks and managing worker threads.

### **Integration Plan**
- **CMakeLists.txt**:
  - Create a `CMakeLists.txt` file to manage compilation.
  - Define dependencies, including libraries like **spdlog** or **Boost**.
  - Use **Conan** to handle dependency management.

### **Summary of Module Responsibilities**
| **Module**             | **Files**                      | **Responsibility**                                       |
|------------------------|--------------------------------|----------------------------------------------------------|
| **Core Manager**       | `process_manager.h/.cpp`       | Initialize the program, command loop, and manage modules.|
| **Command Parsing**    | `command_parser.h/.cpp`        | Parse and validate user commands.                        |
| **Process Listing**    | `process_listing.h/.cpp`       | List all active processes and show resource usage.       |
| **Resource Monitoring**| `resource_monitoring.h/.cpp`   | Monitor CPU/memory in real-time using thread pooling.    |
| **Process Control**    | `process_control.h/.cpp`       | Terminate processes by PID and handle permission checks. |
| **Logging**            | `logger.h/.cpp`                | Log user actions and system events for tracking/debugging.|


