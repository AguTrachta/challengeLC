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

class ResourceMonitoring {
public:
  ResourceMonitoring();
  ~ResourceMonitoring();

  // Starts the monitoring process
  void startMonitoring();

  // Stops the monitoring process
  void stopMonitoring();

private:
  // Parallelized tasks for monitoring CPU and memory
  void monitorCPU();
  void monitorMemory();
  void waitForStopInput();
  void waitForThreads();
  void monitorCPUAndMemory();

  // Thread pool for parallel task execution
  ThreadPool pool_;
  Logger logger_;

  // Monitoring state
  std::atomic<bool> monitoring_;
  std::mutex monitoringMutex_; // Protects the monitoring state
  std::condition_variable stopCondition_;
  DataMonitoring dataMonitor;
};

#endif // RESOURCE_MONITORING_H
