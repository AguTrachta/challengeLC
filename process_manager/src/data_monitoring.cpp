
#include "../include/data_monitoring.h"
#include <atomic>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

// Global variable to store the current CPU usage
std::atomic<double> global_cpu_usage(0.0);
std::atomic<double> global_memory_usage(0.0);

// Constants to replace magic numbers
const int MEMORY_UPDATE_INTERVAL_SECONDS = 1;    // Interval for memory updates
const int CPU_UPDATE_INTERVAL_SECONDS = 1;       // Interval for CPU updates
const char *PROC_MEMINFO_PATH = "/proc/meminfo"; // Path for memory info file
const char *PROC_STAT_PATH = "/proc/stat";       // Path for CPU stats file
const char *MEM_TOTAL_KEY = "MemTotal"; // Key for total memory in /proc/meminfo
const char *MEM_AVAILABLE_KEY =
    "MemAvailable"; // Key for available memory in /proc/meminfo

DataMonitoring::DataMonitoring() : monitoring_(false) {}

DataMonitoring::~DataMonitoring() {
  stopMonitoring(); // Ensure monitoring is stopped before destruction
}

void DataMonitoring::startMonitoring() {
  if (monitoring_) {
    std::cout << "Monitoring is already running.\n";
    return;
  }
  monitoring_ = true;
  std::cout << "Starting CPU and Memory monitoring...\n";
}

void DataMonitoring::stopMonitoring() {
  monitoring_ = false;
  std::cout << "Stopping monitoring...\n";
}

void DataMonitoring::updateMemoryUsage() {
  // Initialize memory tracking variables
  static unsigned long long prev_total_memory = 0;
  static unsigned long long prev_available_memory = 0;

  while (monitoring_) {
    std::ifstream meminfo(PROC_MEMINFO_PATH);
    if (!meminfo) {
      std::cerr << "Error: Could not open " << PROC_MEMINFO_PATH << ".\n";
      return;
    }

    std::string line;
    unsigned long long total_memory = 0;
    unsigned long long available_memory = 0;

    // Parse /proc/meminfo for the required values
    while (std::getline(meminfo, line)) {
      if (line.find(MEM_TOTAL_KEY) != std::string::npos) {
        std::sscanf(line.c_str(), "MemTotal: %llu kB", &total_memory);
      } else if (line.find(MEM_AVAILABLE_KEY) != std::string::npos) {
        std::sscanf(line.c_str(), "MemAvailable: %llu kB", &available_memory);
      }
    }

    // Ensure we have valid memory data
    if (total_memory == 0) {
      std::cerr << "Error: Total memory is zero.\n";
      return;
    }

    // Calculate memory usage percentage
    double used_memory = total_memory - available_memory;
    double memory_usage_percent = (used_memory / total_memory) * 100.0;

    // Update the global memory usage variable
    global_memory_usage.store(memory_usage_percent);

    // Update previous values (if needed for further calculations)
    prev_total_memory = total_memory;
    prev_available_memory = available_memory;

    // Sleep for the configured update interval
    std::this_thread::sleep_for(
        std::chrono::seconds(MEMORY_UPDATE_INTERVAL_SECONDS));
  }
}

void DataMonitoring::updateCPUUsage() {
  while (monitoring_) {
    std::ifstream file(PROC_STAT_PATH);
    std::string line;
    std::getline(file, line); // Read the first line, which contains CPU stats
    file.close();

    // Parse the values from /proc/stat
    std::istringstream stream(line);
    std::string cpu;
    long user, nice, system, idle, iowait, irq, softirq, steal;

    // Skip the "cpu" keyword and read the values
    stream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >>
        softirq >> steal;

    // Calculate the total and idle times
    long total = user + nice + system + idle + iowait + irq + softirq + steal;
    long idle_time = idle + iowait;

    // Calculate the CPU usage percentage
    static long prev_total = 0;
    static long prev_idle = 0;

    long total_diff = total - prev_total;
    long idle_diff = idle_time - prev_idle;

    // Prevent division by zero
    if (total_diff != 0) {
      double cpu_usage = 100.0 * (total_diff - idle_diff) / total_diff;
      global_cpu_usage.store(
          cpu_usage); // Save the result in the global variable
    }

    // Store the current values for the next update
    prev_total = total;
    prev_idle = idle_time;

    // Sleep for the configured update interval
    std::this_thread::sleep_for(std::chrono::seconds(
        CPU_UPDATE_INTERVAL_SECONDS)); // Update every second
  }
}

double DataMonitoring::getMemoryUsage() {
  return global_memory_usage
      .load(); // Return the current memory usage from the global variable
}

// CpuMonitoring class
double DataMonitoring::getCPUUsage() {
  return global_cpu_usage
      .load(); // Return the current CPU usage from the global variable
}
