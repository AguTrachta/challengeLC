/**
 * @file data_monitoring.h
 * @brief Provides a class for monitoring system CPU and memory usage.
 *
 * This file defines the `DataMonitoring` class, which is responsible for
 * monitoring the CPU and memory usage of the system. It provides methods to
 * start, stop, and update the monitoring process, as well as methods to
 * retrieve the current CPU and memory usage.
 */

#ifndef DATA_MONITORING_H
#define DATA_MONITORING_H

#include <iostream>
#include <mutex>
#include <thread>

/**
 * @class DataMonitoring
 * @brief A class that monitors CPU and memory usage.
 *
 * The `DataMonitoring` class is responsible for tracking system CPU and memory
 * usage. It uses separate threads to update the CPU and memory usage statistics
 * every second. It provides methods to start and stop monitoring, as well as
 * retrieve the current usage statistics.
 */
class DataMonitoring {
public:
  /**
   * @brief Constructs a `DataMonitoring` object.
   *
   * Initializes the monitoring flag to `false`, indicating that monitoring is
   * not active at the time of creation.
   */
  DataMonitoring();

  /**
   * @brief Destroys the `DataMonitoring` object.
   *
   * Ensures that the monitoring process is stopped before the object is
   * destroyed.
   */
  ~DataMonitoring();

  /**
   * @brief Starts the CPU and memory monitoring process.
   *
   * This method begins monitoring the CPU and memory usage in separate threads.
   * It will print a message if monitoring is already running.
   */
  void startMonitoring();

  /**
   * @brief Stops the CPU and memory monitoring process.
   *
   * This method stops the monitoring threads and prints a message indicating
   * that the monitoring has stopped.
   */
  void stopMonitoring();

  /**
   * @brief Updates the CPU usage every second.
   *
   * This method runs in a separate thread and updates the global CPU usage
   * value every second by reading system statistics from `/proc/stat`. It
   * calculates the CPU usage as a percentage and updates the global variable.
   */
  void updateCPUUsage();

  /**
   * @brief Updates the memory usage every second.
   *
   * This method runs in a separate thread and updates the global memory usage
   * value every second by reading system statistics from `/proc/meminfo`. It
   * calculates the memory usage as a percentage and updates the global
   * variable.
   */
  void updateMemoryUsage();

  /**
   * @brief Returns the current memory usage as a percentage.
   *
   * This method retrieves the global memory usage value, which is updated by
   * the `updateMemoryUsage` thread.
   *
   * @return The current memory usage percentage.
   */
  double getMemoryUsage();

  /**
   * @brief Returns the current CPU usage as a percentage.
   *
   * This method retrieves the global CPU usage value, which is updated by the
   * `updateCPUUsage` thread.
   *
   * @return The current CPU usage percentage.
   */
  double getCPUUsage();

private:
  bool monitoring_; /**< Flag to indicate whether monitoring is active. */
};

#endif // DATA_MONITORING_H

