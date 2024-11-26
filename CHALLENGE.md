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

