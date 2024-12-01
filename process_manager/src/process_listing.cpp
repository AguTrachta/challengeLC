// src/process_listing.cpp

#include "../include/process_listing.h"
#include "../include/logger.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <future>
#include <iostream>
#include <sstream>

#include <unistd.h>
#include <vector>

namespace fs = std::filesystem;

ProcessListing::ProcessListing() {
  // Constructor if needed
}

void ProcessListing::listProcesses() {
  Logger logger;
  logger.logAction("Listing processes");

  fetchProcessList();

  // Print header with proper spacing
  std::cout << std::left << std::setw(8) << "PID" << std::setw(10) << "CPU%"
            << std::setw(10) << "Memory%" << "Name\n";
  std::cout << std::string(40, '-') << '\n'; // Separator line

  // Print each process with formatted columns
  for (const auto &process : processes_) {
    // Print PID (uncolored)
    std::cout << std::left << std::setw(8) << process.pid;

    // Print CPU% with colors
    if (process.cpuUsage > 50.0) {
      std::cout << "\033[31m"; // Red for high usage
    } else if (process.cpuUsage > 20.0) {
      std::cout << "\033[33m"; // Yellow for moderate usage
    } else {
      std::cout << "\033[32m"; // Green for low usage
    }
    std::cout << std::setw(10) << std::fixed << std::setprecision(2)
              << process.cpuUsage << "\033[0m"; // Reset color

    // Print Memory% with colors
    if (process.memoryUsage > 50.0) {
      std::cout << "\033[31m"; // Red for high usage
    } else if (process.memoryUsage > 20.0) {
      std::cout << "\033[33m"; // Yellow for moderate usage
    } else {
      std::cout << "\033[32m"; // Green for low usage
    }
    std::cout << std::setw(10) << std::fixed << std::setprecision(2)
              << process.memoryUsage << "\033[0m"; // Reset color

    // Print Name (uncolored)
    std::cout << process.name.substr(0, 30) << '\n'; // Limit name to 30 chars
  }
}

void ProcessListing::fetchProcessList() {
  std::vector<int> pids = getAllPIDs();
  size_t batchSize = 15; // Number of PIDs to process per thread
  size_t numBatches =
      (pids.size() + batchSize - 1) / batchSize; // Calculate batches
  std::vector<std::future<void>> futures;

  for (size_t i = 0; i < numBatches; ++i) {
    futures.push_back(std::async(std::launch::async, [&, i]() {
      size_t start = i * batchSize;
      size_t end = std::min(start + batchSize, pids.size());
      for (size_t j = start; j < end; ++j) {
        fetchProcessInfo(pids[j]);
      }
    }));
  }

  for (auto &fut : futures) {
    fut.get();
  }
}

std::vector<int> ProcessListing::getAllPIDs() {
  std::vector<int> pids;
  for (const auto &entry : fs::directory_iterator("/proc")) {
    if (entry.is_directory()) {
      std::string filename = entry.path().filename().string();
      if (std::all_of(filename.begin(), filename.end(), ::isdigit)) {
        pids.push_back(std::stoi(filename));
      }
    }
  }
  return pids;
}

void ProcessListing::fetchProcessInfo(int pid) {
  ProcessInfo info;
  info.pid = pid;
  info.name = getProcessName(pid);
  info.cpuUsage = calculateCPUUsage(pid);
  info.memoryUsage = calculateMemoryUsage(pid);

  // Lock mutex before modifying shared data
  std::lock_guard<std::mutex> lock(mutex_);
  processes_.push_back(info);
}

std::string ProcessListing::getProcessName(int pid) {
  std::string name = "Unknown";
  std::string path = "/proc/" + std::to_string(pid) + "/comm";
  std::ifstream commFile(path);
  if (commFile.is_open()) {
    std::getline(commFile, name);
    commFile.close();
  }
  return name;
}

double ProcessListing::calculateCPUUsage(int pid) {
  static std::unordered_map<int, std::vector<unsigned long long>> prev_times;
  unsigned long long utime, stime, cutime, cstime, total_time;
  unsigned long long system_total_time = 0;
  unsigned long long prev_total_time = 0;
  unsigned long long prev_proc_time = 0;

  // Read process-specific times
  std::ifstream statFile("/proc/" + std::to_string(pid) + "/stat");
  if (!statFile.is_open()) {
    return 0.0; // Return 0.0 if the process stat file cannot be read
  }

  std::string ignore;
  for (int i = 0; i < 13; ++i) {
    statFile >> ignore; // Skip unnecessary fields
  }
  statFile >> utime >> stime >> cutime >> cstime;
  statFile.close();

  total_time = utime + stime + cutime + cstime;

  // Calculate system-wide CPU time
  std::ifstream cpuStatFile("/proc/stat");
  if (!cpuStatFile.is_open()) {
    return 0.0;
  }

  std::string line;
  std::getline(cpuStatFile, line);
  cpuStatFile.close();

  std::istringstream iss(line);
  iss >> ignore; // Skip 'cpu'
  unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
  iss >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
  system_total_time =
      user + nice + system + idle + iowait + irq + softirq + steal;

  // Fetch previous values for the process
  if (prev_times.find(pid) != prev_times.end()) {
    prev_proc_time = prev_times[pid][0];
    prev_total_time = prev_times[pid][1];
  }

  // Calculate CPU usage
  unsigned long long total_delta = system_total_time - prev_total_time;
  unsigned long long proc_delta = total_time - prev_proc_time;

  prev_times[pid] = {total_time, system_total_time};

  if (total_delta == 0) {
    return 0.0;
  }

  return (static_cast<double>(proc_delta) / total_delta) * 100.0;
}

double ProcessListing::calculateMemoryUsage(int pid) {
  unsigned long long resident_pages = 0;
  long page_size_kb = sysconf(_SC_PAGESIZE) / 1024; // Page size in KB

  // Read process memory info
  std::ifstream statmFile("/proc/" + std::to_string(pid) + "/statm");
  if (!statmFile.is_open()) {
    return 0.0; // Return 0.0 if the statm file cannot be read
  }

  std::string dummy;                    // Variable to discard unused values
  statmFile >> dummy >> resident_pages; // Read the second value (RSS)
  statmFile.close();

  unsigned long long process_memory_kb = resident_pages * page_size_kb;

  // Read total memory
  std::ifstream meminfoFile("/proc/meminfo");
  if (!meminfoFile.is_open()) {
    return 0.0;
  }

  std::string line;
  unsigned long long total_memory_kb = 0;
  while (std::getline(meminfoFile, line)) {
    std::istringstream iss(line);
    std::string key;
    unsigned long long value;
    iss >> key >> value;
    if (key == "MemTotal:") {
      total_memory_kb = value;
      break;
    }
  }
  meminfoFile.close();

  if (total_memory_kb == 0) {
    return 0.0;
  }

  return (static_cast<double>(process_memory_kb) / total_memory_kb) * 100.0;
}
