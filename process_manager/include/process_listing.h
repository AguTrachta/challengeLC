// include/process_listing.h

#ifndef PROCESS_LISTING_H
#define PROCESS_LISTING_H

#include <mutex>
#include <string>
#include <vector>

struct ProcessInfo {
  int pid;
  std::string name;
  double cpuUsage;
  double memoryUsage;
};

class ProcessListing {
public:
  ProcessListing();
  void listProcesses();

private:
  std::vector<ProcessInfo> processes_;
  std::mutex mutex_;

  void fetchProcessList();
  void fetchProcessInfo(int pid);
  static std::vector<int> getAllPIDs();

  // Helper functions
  double calculateCPUUsage(int pid);
  double calculateMemoryUsage(int pid);
  std::string getProcessName(int pid);
};

#endif // PROCESS_LISTING_H
