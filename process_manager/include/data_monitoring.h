#ifndef DATA_MONITORING_H
#define DATA_MONITORING_H

#include <iostream>
#include <mutex>
#include <thread>

class DataMonitoring {
public:
  DataMonitoring();
  ~DataMonitoring();

  // Starts the CPU monitoring process
  void startMonitoring();

  // Stops the CPU monitoring process
  void stopMonitoring();

  // Update the CPU usage every second (called by a separate thread)
  void updateCPUUsage();

  void updateMemoryUsage();

  double getMemoryUsage();

  double getCPUUsage();

private:
  bool monitoring_; // Flag to indicate whether monitoring is active
};

#endif // DATA_MONITORING_H
