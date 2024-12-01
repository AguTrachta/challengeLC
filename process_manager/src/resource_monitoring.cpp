
#include "../include/resource_monitoring.h"
#include "../include/data_monitoring.h"
#include "../include/logger.h"
#include <chrono>
#include <condition_variable> // for condition_variable
#include <iomanip>
#include <iostream>
#include <mutex>
#include <thread>

// Constructor
ResourceMonitoring::ResourceMonitoring() : pool_(4), monitoring_(false) {}

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
  std::cout << "Starting parallelized resource monitoring...\n";
  std::cout << "Press Enter to stop the monitor.\n";

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
  std::cout << "Stopping resource monitoring...\n";

  stopCondition_.notify_all(); // Notify all waiting threads to stop
}

/*void ResourceMonitoring::monitorCPU() {
  while (monitoring_) {
    double cpuUsage = dataMonitor.getCPUUsage(); // Get the current CPU usage

    // Create a string with enough space to overwrite the previous line
    std::cout << "\rCurrent CPU Usage: " << std::fixed << std::setprecision(2)
              << cpuUsage << "%\n"
              << std::flush;
    std::this_thread::sleep_for(
        std::chrono::seconds(1)); // Simulate CPU monitoring interval
  }
}

// Monitor Memory
void ResourceMonitoring::monitorMemory() {
  while (monitoring_) {
    double memoryUsage =
        dataMonitor.getMemoryUsage(); // Get the current memory usage

    // Create a string with enough space to overwrite the previous line
    std::cout << "\rCurrent Memory Usage: " << std::fixed
              << std::setprecision(2) << memoryUsage << "%\n"
              << std::flush;

    std::this_thread::sleep_for(
        std::chrono::seconds(1)); // Simulate memory monitoring interval
  }
}
*/
// Wait for user input to stop monitoring
void ResourceMonitoring::waitForStopInput() {
  std::string input;
  std::getline(std::cin, input); // Wait for the user to press Enter

  stopMonitoring(); // Stop the monitoring when Enter is pressed
}

void ResourceMonitoring::monitorCPUAndMemory() {
  // Save the cursor position where the output starts
  std::cout << "\033[s"; // Save the current cursor position

  while (monitoring_) {
    // Get the current CPU and memory usage
    double cpuUsage = dataMonitor.getCPUUsage();
    double memoryUsage = dataMonitor.getMemoryUsage();

    // Restore the saved cursor position
    std::cout << "\033[u"; // Restore the saved cursor position

    // Display the monitoring data (overwrite previous data in place)
    std::cout << "Resource Monitoring\n";
    std::cout << "--------------------\n";
    std::cout << "CPU Usage:    " << std::fixed << std::setprecision(2)
              << cpuUsage << "%\n";
    std::cout << "Memory Usage: " << std::fixed << std::setprecision(2)
              << memoryUsage << "%\n";

    // Flush the output to ensure the data is displayed immediately
    std::cout << std::flush;

    // Sleep for 1 second before updating the display
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  // Optionally, add a newline after monitoring ends for better appearance
  std::cout << "\n";
}
