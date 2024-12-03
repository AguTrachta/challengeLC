
/**
 * @file resource_monitoring.h
 * @brief Provides functionality for monitoring system resources like CPU and
 * memory usage.
 *
 * This file defines the `ResourceMonitoring` class, which provides parallelized
 * monitoring of system resources. The class monitors CPU and memory usage in
 * real-time and allows the user to start and stop the monitoring process.
 */

#ifndef RESOURCE_MONITORING_H
#define RESOURCE_MONITORING_H

#include "data_monitoring.h"
#include "logger.h"
#include "thread_pool.h"
#include <atomic>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

/**
 * @class ResourceMonitoring
 * @brief A class for monitoring system resources such as CPU and memory.
 *
 * The `ResourceMonitoring` class enables the monitoring of system resources
 * like CPU and memory usage. The class supports real-time, parallelized
 * monitoring, with the ability to start and stop the monitoring process via
 * user input.
 */
class ResourceMonitoring {
public:
  /**
   * @brief Constructor for the ResourceMonitoring class.
   *
   * Initializes the resource monitoring system. The thread pool is created, and
   * the monitoring state is set to false initially.
   */
  ResourceMonitoring();

  /**
   * @brief Destructor for the ResourceMonitoring class.
   *
   * Stops the monitoring process if it is still running and ensures proper
   * cleanup.
   */
  ~ResourceMonitoring();

  /**
   * @brief Starts the monitoring process.
   *
   * Initiates parallelized monitoring of CPU and memory usage. The monitoring
   * runs until the user inputs a command to stop the process.
   */
  void startMonitoring();

  /**
   * @brief Stops the monitoring process.
   *
   * Halts the resource monitoring, stops all running tasks, and performs
   * necessary cleanup.
   */
  void stopMonitoring();

  /**
   * @brief Gets the `DataMonitoring` object to access resource data.
   *
   * Returns a reference to the `DataMonitoring` object, allowing access to
   * the system resource monitoring data (e.g., CPU and memory usage).
   *
   * @return A reference to the `DataMonitoring` object.
   */
  DataMonitoring &getDataMonitor() { return dataMonitor; }

  /**
   * @brief Gets the monitoring status.
   *
   * Returns the current status of the monitoring process (true if active,
   * false otherwise).
   *
   * @return `true` if monitoring is active, `false` otherwise.
   */
  bool getMonitoringBool() const { return monitoring_; }

private:
  /**
   * @brief Monitors CPU usage.
   *
   * Runs a task that continuously checks and updates CPU usage at regular
   * intervals. This function operates in parallel with other monitoring tasks.
   */
  void monitorCPU();

  /**
   * @brief Monitors memory usage.
   *
   * Runs a task that continuously checks and updates memory usage at regular
   * intervals. This function operates in parallel with other monitoring tasks.
   */
  void monitorMemory();

  /**
   * @brief Waits for the user to input a command to stop monitoring.
   *
   * The method waits for the user to press Enter to stop the monitoring
   * process. It listens for the stop signal and coordinates the stopping of
   * all monitoring tasks.
   */
  void waitForStopInput();

  /**
   * @brief Waits for all threads to finish execution.
   *
   * This function ensures that all tasks related to resource monitoring are
   * completed before exiting. It waits for the monitoring threads to finish
   * their execution, ensuring proper synchronization.
   */
  void waitForThreads();

  /**
   * @brief Monitors both CPU and memory usage in parallel.
   *
   * Updates the display with CPU and memory usage statistics in real-time,
   * running in parallel until the monitoring is stopped. This method starts
   * the tasks for monitoring both resources and is intended to run as the main
   * monitoring process.
   */
  void monitorCPUAndMemory();

  // Thread pool for executing parallel tasks
  ThreadPool pool_;

  // Logger to log monitoring actions and warnings
  Logger logger_;

  // Atomic flag to indicate whether monitoring is active
  std::atomic<bool> monitoring_;

  // Mutex to protect access to the monitoring state
  std::mutex monitoringMutex_;

  // Condition variable to wait for the stop signal
  std::condition_variable stopCondition_;

  // Data monitoring object for collecting resource usage data
  DataMonitoring dataMonitor;
};

#endif // RESOURCE_MONITORING_H
