# Additional Logic and HPC Optimization Challenge

### Scenario: Excessive Memory and CPU Usage

During testing, the High-Performance Process Manager tool occasionally consumes excessive memory and CPU resources when monitoring processes with high update frequencies. This issue can hinder the tool's performance and scalability, especially in environments with numerous active processes or high system load.

# Monitoring analysis

![image](https://github.com/user-attachments/assets/246d728a-8b13-4651-8c89-051f3d53d5d7)

The `monitor` command in the **High-Performance Process Manager** is designed to provide real-time insights into the system's CPU and memory usage. Here's a breakdown of its key components and functionalities based on the provided code:

- **Thread Pool Management:**
  - **Thread Pool Size:** The command initializes a thread pool with a size of 4 (`THREAD_POOL_SIZE = 4`). This pool manages concurrent tasks related to monitoring CPU and memory usage.
  - **Task Enqueuing:** Multiple tasks are enqueued into the thread pool, including updating CPU and memory usage (`updateCPUUsage` and `updateMemoryUsage`), displaying the monitored data (`monitorCPUAndMemory`), and handling user input to stop monitoring (`waitForStopInput`).

- **Monitoring Control:**
  - **Start Monitoring:** The `startMonitoring` method checks if monitoring is already active to prevent duplicate monitoring sessions. If not, it initializes the monitoring process, logs the action, and begins the monitoring tasks.
  - **Stop Monitoring:** The `stopMonitoring` method gracefully terminates the monitoring process by updating the monitoring flag, logging the action, notifying all threads to stop, and ensuring all threads complete their execution.

- **Data Collection and Display:**
  - **CPU and Memory Usage Updates:** The `updateCPUUsage` and `updateMemoryUsage` methods continuously read system statistics from `/proc/stat` and `/proc/meminfo` respectively, calculating the usage percentages and storing them in atomic variables (`global_cpu_usage` and `global_memory_usage`).
  - **Real-Time Display:** The `monitorCPUAndMemory` method retrieves the latest CPU and memory usage data and displays it in a formatted manner on the console, utilizing ANSI escape codes for color and formatting enhancements.

## **Memory Usage Analysis**

### **Valgrind Findings**

The Valgrind output indicates the following:

- **Heap Summary:**
  - **In Use at Exit:** 2,019 bytes in 24 blocks.
  - **Total Heap Usage:** 145 allocations and 121 deallocations, totaling 147,743 bytes allocated.
  - **Leak Summary:**
    - **Definitely Lost:** 0 bytes in 0 blocks.
    - **Indirectly Lost:** 0 bytes in 0 blocks.
    - **Possibly Lost:** 0 bytes in 0 blocks.
    - **Still Reachable:** 2,019 bytes in 24 blocks.
    - **Suppressed:** 0 bytes in 0 blocks.
  - **Error Summary:** 0 errors from 0 contexts.

- **Loss Records:**
  - All loss records fall under the **"still reachable"** category, primarily associated with the **spdlog** library. This includes memory allocations related to pattern formatting and file handling within the logging system.

### **Interpretation of Valgrind Output**

- **No Active Memory Leaks:**
  - The absence of **"definitely lost"** and **"possibly lost"** blocks indicates that there are no outright memory leaks within the application. All allocated memory is either freed appropriately or remains accessible through existing pointers.

- **"Still Reachable" Memory:**
  - The **"still reachable"** memory blocks, totaling 2,019 bytes, are primarily attributed to the **spdlog** library. This is typical behavior for logging libraries that maintain global instances or static objects throughout the application's lifecycle.
  - Since these allocations are still accessible and not lost, they do not pose a memory leak threat. However, they do contribute to the overall memory footprint of the application.

### **Heaptrack Observation**

- **Memory Usage:**
  - The Heaptrack analysis indicates that the `monitor` command utilizes approximately **90 KB** of memory. This figure encompasses both the application's own allocations and those made by external libraries like **spdlog**.

- **Assessment:**
  - **90 KB** is relatively modest for a modern application, especially one that performs continuous monitoring and logging. However, it's essential to ensure that this memory usage remains stable over time and does not grow unexpectedly, which could indicate inefficiencies or memory leaks in other parts of the application.

#### **3. Potential Causes for Excessive Memory and CPU Usage**

Based on the code and Valgrind findings, here are two primary reasons that might contribute to higher memory and CPU consumption:

1. **Inefficient Thread Management:**
   - **Thread Pool Size:** With a fixed thread pool size of 4, the application might create multiple threads for CPU and memory updates, especially if multiple monitoring sessions are initiated. This can lead to increased CPU usage due to context switching and resource contention.
   - **Thread Lifecycle:** Threads might not be terminating promptly or could be lingering longer than necessary, leading to accumulated resource usage over time.

2. **Frequent and Redundant Data Collection:**
   - **Update Intervals:** The monitoring functions (`updateCPUUsage` and `updateMemoryUsage`) are set to update every second (`MONITOR_UPDATE_INTERVAL_SECONDS = 1`). Such high-frequency updates can lead to continuous file I/O operations, increasing both CPU and memory usage.
   - **Parsing Overhead:** Continuously reading and parsing system files like `/proc/stat` and `/proc/meminfo` can be resource-intensive, especially if the parsing logic is not optimized.

# **List command**

![image](https://github.com/user-attachments/assets/69e512bd-d33b-41f8-b639-65bee2dc4a55)

#### **1. Overview of the `list` Command**

The `list` command in the **High-Performance Process Manager** is designed to display a real-time list of active processes along with their CPU and memory usage percentages. The provided `process_listing.cpp` implements this functionality by fetching process information from the `/proc` filesystem and displaying it in a formatted manner. Here's a breakdown of its key components and functionalities based on the provided code:

- **Threading and Concurrency:**
  - **Batch Processing:** Processes are fetched and processed in batches (`BATCH_SIZE = 15`) to optimize performance and reduce the overhead of handling a large number of processes simultaneously.
  - **Asynchronous Execution:** Utilizes `std::async` with `std::launch::async` to execute batch processing tasks concurrently, leveraging multiple CPU cores for efficiency.

- **Process Information Retrieval:**
  - **PID Extraction:** Scans the `/proc` directory to identify all active process IDs (PIDs) by filtering directory names that are purely numeric.
  - **Process Details:** For each PID, retrieves the process name from `/proc/<pid>/comm`, CPU usage from `/proc/<pid>/stat`, and memory usage from `/proc/<pid>/statm`.

- **Data Aggregation and Display:**
  - **Formatting:** Utilizes `std::setw` and other formatting tools to present the data in neatly aligned columns.
  - **Color-Coding:** Applies ANSI escape codes to color-code CPU and memory usage percentages based on predefined thresholds, enhancing readability and immediate visual cues.
  - **Synchronization:** Employs `std::lock_guard<std::mutex>` to ensure thread-safe access when modifying shared data structures (`processes_` vector).

#### **2. Memory Usage Analysis**

## **Valgrind Findings**

The Valgrind output provided indicates the following:

- **Heap Summary:**
  - **In Use at Exit:** 2,019 bytes in 24 blocks.
  - **Total Heap Usage:** 5,711 allocations and 5,687 deallocations, totaling 12,490,207 bytes allocated.
  - **Leak Summary:**
    - **Definitely Lost:** 0 bytes in 0 blocks.
    - **Indirectly Lost:** 0 bytes in 0 blocks.
    - **Possibly Lost:** 0 bytes in 0 blocks.
    - **Still Reachable:** 2,019 bytes in 24 blocks.
    - **Suppressed:** 0 bytes in 0 blocks.
  - **Error Summary:** 0 errors from 0 contexts.

## **Heaptrack Observation**

- **Memory Usage:**
  - The Heaptrack analysis indicates that the `list` command utilizes approximately **200 KB** of memory. This is significantly higher compared to the `monitor` command's memory usage of approximately **90 KB**.

- **Assessment:**
  - **200 KB** is still within a manageable range for modern applications. However, considering that the `monitor` command only uses about **90 KB**, this increase suggests that the `list` command may involve more intensive memory operations, possibly due to handling a larger volume of process data or more complex data structures.

## **Interpretation of Valgrind Output**

- **No Active Memory Leaks:**
  - The absence of **"definitely lost"** and **"possibly lost"** blocks indicates that there are no outright memory leaks within the `list` command. All allocated memory is either freed appropriately or remains accessible through existing pointers.

- **Heaptrack Insights:**
  - The higher memory usage observed during the `list` command could be attributed to:
    - **Data Structures:** The `processes_` vector stores `ProcessInfo` objects, which may accumulate substantial data when handling numerous processes.
    - **String Operations:** Extensive use of `std::string` for process names and file paths can lead to increased memory allocations.
    - **Concurrency Overheads:** Managing multiple asynchronous tasks and futures can introduce additional memory overhead due to thread management and task queuing.

#### **3. Potential Causes for Increased Memory Usage**

Based on the provided code and Valgrind findings, the following factors may contribute to the higher memory usage observed during the execution of the `list` command:

1. **Large Number of Processes:**
   - Systems with a high number of active processes can lead to increased memory consumption as each process's information is stored and processed.

2. **Batch Processing Overhead:**
   - While batching improves performance, managing multiple batches concurrently may introduce additional memory overhead, especially if each batch involves significant data processing.

3. **String Handling:**
   - Frequent construction and manipulation of `std::string` objects (e.g., process names, file paths) can lead to higher memory allocations, especially if not managed efficiently.

4. **Asynchronous Task Management:**
   - Using `std::async` for concurrent execution can result in the creation of numerous temporary objects and future instances, contributing to increased memory usage.

5. **ProcessInfo Data Structure:**
   - The `ProcessInfo` struct contains multiple fields (PID, CPU usage, memory usage, name). If not optimized, storing numerous instances can cumulatively consume significant memory.

