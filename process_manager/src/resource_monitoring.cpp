
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
  std::cout << "\033[s"; // Save the current cursor position

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

    std::cout << "\033[u"; // Restore the saved cursor position

    // Add colors and formatting
    std::cout << "\033[1;32mResource Monitoring\033[0m\n"; // Green and bold
    std::cout << "--------------------\n";
    std::cout << "CPU Usage:    \033[1m" << std::fixed << std::setprecision(2)
              << cpuUsage << "%\033[0m\n"; // Bold for CPU percentage
    std::cout << "Memory Usage: \033[1m" << std::fixed << std::setprecision(2)
              << memoryUsage << "%\033[0m\n"; // Bold for Memory percentage
    std::cout << std::flush;

    // Sleep for 1 second before updating the display
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  std::cout << "\n";
}
