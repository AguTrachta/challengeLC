
#include "../include/resource_monitoring.h"
#include "../include/data_monitoring.h"
#include "../include/logger.h"
#include <chrono>
#include <condition_variable> // for condition_variable
#include <iomanip>
#include <iostream>
#include <mutex>
#include <thread>

// Constants
constexpr int THREAD_POOL_SIZE = 4; // Size of the thread pool
constexpr int MONITOR_UPDATE_INTERVAL_SECONDS =
    1; // Interval for updating CPU and Memory usage
constexpr const char *USER_STOP_PROMPT =
    "Press Enter to stop the monitor.\n"; // User prompt
constexpr const char *RESOURCE_MONITORING_HEADER =
    "\033[1;32mResource Monitoring\033[0m\n"; // Header with color
constexpr const char *RESOURCE_MONITORING_SEPARATOR =
    "--------------------\n";                             // Separator line
constexpr const char *CPU_USAGE_LABEL = "CPU Usage:    "; // Label for CPU usage
constexpr const char *MEMORY_USAGE_LABEL =
    "Memory Usage: ";                                  // Label for Memory usage
constexpr const char *CURSOR_POSITION_SAVE = "\033[s"; // Save cursor position
constexpr const char *CURSOR_POSITION_RESTORE =
    "\033[u";                                   // Restore cursor position
constexpr const char *BOLD_FORMAT = "\033[1m";  // Bold text formatting
constexpr const char *RESET_FORMAT = "\033[0m"; // Reset text formatting

// Constructor
ResourceMonitoring::ResourceMonitoring()
    : pool_(THREAD_POOL_SIZE), monitoring_(false) {}

// Destructor
ResourceMonitoring::~ResourceMonitoring() {
  stopMonitoring(); // Ensure monitoring is stopped before destruction
}

// Start the monitoring process
void ResourceMonitoring::startMonitoring() {
  std::unique_lock<std::mutex> lock(monitoringMutex_);

  if (monitoring_) {
    logger_.logWarning(
        "Attempted to start monitoring, but it's already running.");
    std::cout << "Resource monitoring is already running.\n";
    return;
  }

  monitoring_ = true;
  logger_.logAction("Starting resource monitoring.");

  std::cout << USER_STOP_PROMPT;

  dataMonitor.startMonitoring();

  pool_.enqueue([this]() { dataMonitor.updateCPUUsage(); });
  pool_.enqueue([this]() { dataMonitor.updateMemoryUsage(); });

  pool_.enqueue([this]() { monitorCPUAndMemory(); });
  // Enqueue the task to wait for user input to stop monitoring
  pool_.enqueue([this]() { waitForStopInput(); });

  // Wait until the stop signal is received (prevent main thread from finishing
  // prematurely)
  stopCondition_.wait(lock, [this]() {
    return !monitoring_;
  }); // Wait for the monitoring to stop
}

// Stop the monitoring process
void ResourceMonitoring::stopMonitoring() {
  std::unique_lock<std::mutex> lock(monitoringMutex_);
  monitoring_ = false;
  logger_.logAction("Stopping resource monitoring.");

  stopCondition_.notify_all(); // Notify all waiting threads to stop
  pool_.waitForAll();
}

// Wait for user input to stop monitoring
void ResourceMonitoring::waitForStopInput() {
  std::string input;
  std::getline(std::cin, input); // Wait for the user to press Enter

  stopMonitoring(); // Stop the monitoring when Enter is pressed
}

void ResourceMonitoring::monitorCPUAndMemory() {
  std::cout << CURSOR_POSITION_SAVE; // Save the current cursor position

  while (monitoring_) {
    {
      // Add a scoped lock to ensure thread-safe access to monitoring_ flag
      std::unique_lock<std::mutex> lock(monitoringMutex_);
      if (!monitoring_) {
        break;
      }
    }

    double cpuUsage = dataMonitor.getCPUUsage();
    double memoryUsage = dataMonitor.getMemoryUsage();

    std::cout << CURSOR_POSITION_RESTORE; // Restore the saved cursor position

    // Add colors and formatting
    std::cout << RESOURCE_MONITORING_HEADER;    // Green and bold header
    std::cout << RESOURCE_MONITORING_SEPARATOR; // Separator line
    std::cout << CPU_USAGE_LABEL << BOLD_FORMAT << std::fixed
              << std::setprecision(2) << cpuUsage << "%" << RESET_FORMAT
              << "\n"; // Bold for CPU percentage
    std::cout << MEMORY_USAGE_LABEL << BOLD_FORMAT << std::fixed
              << std::setprecision(2) << memoryUsage << "%" << RESET_FORMAT
              << "\n"; // Bold for Memory percentage
    std::cout << std::flush;

    // Sleep for the defined interval before updating the display
    std::this_thread::sleep_for(
        std::chrono::seconds(MONITOR_UPDATE_INTERVAL_SECONDS));
  }
  std::cout << "\n";
}
