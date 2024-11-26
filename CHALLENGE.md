# High-Performance Process Manager

## Objective
Develop a high-performance command-line tool in C++20 that functions as a process manager. This tool is designed to handle and display information about processes efficiently, leveraging HPC (High-Performance Computing) concepts like multithreading to optimize resource monitoring and reduce latency.

## Features Overview

### 1. Process Listing
The tool includes a feature to list all active processes on the system, providing detailed information such as:
- **Process ID (PID)**: The unique identifier for each running process.
- **Process Name**: The name of the running process.
- **CPU Usage**: The current percentage of CPU resources consumed by each process.
- **Memory Usage**: The amount of memory allocated to each process.

#### Optimization
- The process data retrieval is optimized to handle high system loads effectively.
- **Multithreading** is utilized to fetch process information concurrently, reducing latency and improving overall performance.

### 2. Parallelized Resource Monitoring
The tool implements a **parallelized approach** to monitor CPU and memory usage in real-time.

- **Thread Pooling** is employed to distribute monitoring tasks across multiple threads for better performance.
- The use of HPC techniques ensures that monitoring is efficient, without causing high CPU usage by the tool itself.

### 3. Process Control
- Users can terminate processes by specifying the PID.
- The tool includes **robust error handling** and **permissions management** to ensure that only valid and accessible processes are terminated, maintaining system stability.

### 4. Logging and Error Handling
- Each user action, such as terminating a process or encountering an error, is logged with a timestamp.
- Critical system information and error details are documented in a log file, aiding in tracking the tool's activities and troubleshooting issues.

## Requirements

### Functional Requirements
1. **Process Listing**:
   - List all active processes on the system.
   - Display details including:
     - **Process ID (PID)**.
     - **Process name**.
     - **CPU usage**.
     - **Memory usage**.
2. **Real-Time Resource Monitoring**:
   - Monitor CPU and memory usage in real-time.
   - Use multithreading to efficiently collect resource usage data concurrently.
   - Provide a continuously updated output of system resource consumption.
3. **Process Control**:
   - Allow users to **terminate a process** by specifying its PID.
   - Perform a **permissions check** to ensure only valid and accessible processes can be terminated.
   - Handle errors gracefully if the process termination fails.
4. **Command Logging**:
   - Log all user actions (e.g., listing processes, terminating a process).
   - Log any errors or unusual conditions (e.g., permission errors, non-existent PIDs).
   - Include **timestamps** for all log entries.
5. **User Interaction**:
   - Provide an interactive mode for users to issue commands without restarting the program each time.
   - Offer commands such as:
     - `list` to list all processes.
     - `monitor` to enter real-time monitoring mode.
     - `kill <pid>` to terminate a specific process.
     - `exit` to exit the program.

### Non-Functional Requirements
1. **Performance**:
   - Must operate with **low latency** when listing processes or monitoring system resources.
   - The **resource overhead** of running the tool should be minimal, avoiding heavy CPU or memory consumption.
2. **Scalability**:
   - Should handle systems with **large numbers of processes** (e.g., 1000+ processes) without significant degradation in response times.
   - Efficient memory and CPU management to avoid performance issues in high-load environments.
3. **Reliability**:
   - The tool should be **robust** and able to handle unexpected situations such as invalid input, lack of permissions, or high load without crashing.
4. **Portability**:
   - Primarily designed for **Linux** systems, but should have a flexible architecture that can be adapted to other Unix-like systems.
   - Code should be written in a **platform-agnostic** way where possible, allowing easier future expansion to Windows or macOS.
5. **Usability**:
   - The user interface should be simple and intuitive, allowing users with basic command-line experience to use the tool comfortably.
   - Provide **meaningful output messages** and **usage instructions** if incorrect commands are entered.
6. **Security**:
   - Prevent unauthorized actions by implementing **permission checks** before performing process control actions like termination.
   - Ensure sensitive information (e.g., logs) is not accessible by unauthorized users.
7. **Extensibility**:
   - The architecture should support **adding new features** in the future (e.g., network usage monitoring, detailed I/O stats).
   - The codebase should be modular, with well-defined boundaries between components, making future changes easier.


