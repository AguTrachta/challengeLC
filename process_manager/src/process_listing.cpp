// src/process_listing.cpp

#include "../include/process_listing.h"
#include "../include/logger.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <future>
#include <iostream>
#include <sstream>
#include <thread>
#include <unistd.h>
#include <vector>

namespace fs = std::filesystem;

namespace {
// Constants for better readability
const size_t BATCH_SIZE = 15;      // Number of PIDs to process per thread
const size_t MAX_NAME_LENGTH = 30; // Max length for process name display
const double HIGH_USAGE_THRESHOLD =
    50.0; // Threshold for high usage (CPU/Memory)
const double MODERATE_USAGE_THRESHOLD =
    20.0; // Threshold for moderate usage (CPU/Memory)
const size_t PROCESS_STAT_FIELDS_TO_SKIP =
    13; // Number of fields to skip in /proc/<pid>/stat
const std::string PROC_STAT_PATH = "/proc/stat"; // Path to CPU stats
const std::string PROC_COMM_PATH_PREFIX =
    "/proc/"; // Prefix for process comm files
} // Anonymous namespace

ProcessListing::ProcessListing() {
  // Constructor if needed
}

#include <iomanip>
#include <iostream>

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
    if (process.cpuUsage > HIGH_USAGE_THRESHOLD) {
      std::cout << "\033[31m"; // Red for high usage
    } else if (process.cpuUsage > MODERATE_USAGE_THRESHOLD) {
      std::cout << "\033[33m"; // Yellow for moderate usage
    } else {
      std::cout << "\033[32m"; // Green for low usage
    }
    std::cout << std::setw(10) << std::fixed << std::setprecision(2)
              << process.cpuUsage << "\033[0m"; // Reset color

    // Print Memory% with colors
    if (process.memoryUsage > HIGH_USAGE_THRESHOLD) {
      std::cout << "\033[31m"; // Red for high usage
    } else if (process.memoryUsage > MODERATE_USAGE_THRESHOLD) {
      std::cout << "\033[33m"; // Yellow for moderate usage
    } else {
      std::cout << "\033[32m"; // Green for low usage
    }
    std::cout << std::setw(10) << std::fixed << std::setprecision(2)
              << process.memoryUsage << "\033[0m"; // Reset color

    // Print Name (uncolored)
    std::cout << process.name.substr(0, MAX_NAME_LENGTH)
              << '\n'; // Limit name to 30 chars
  }
}

void ProcessListing::fetchProcessList() {
  std::vector<int> pids = getAllPIDs();
  size_t numBatches =
      (pids.size() + BATCH_SIZE - 1) / BATCH_SIZE; // Calculate batches
  std::vector<std::future<void>> futures;

  for (size_t i = 0; i < numBatches; ++i) {
    futures.push_back(std::async(std::launch::async, [&, i]() {
      size_t start = i * BATCH_SIZE;
      size_t end = std::min(start + BATCH_SIZE, pids.size());
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
  std::string path = PROC_COMM_PATH_PREFIX + std::to_string(pid) + "/comm";
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
  for (int i = 0; i < PROCESS_STAT_FIELDS_TO_SKIP; ++i) {
    statFile >> ignore; // Skip unnecessary fields
  }
  statFile >> utime >> stime >> cutime >> cstime;
  statFile.close();

  total_time = utime + stime + cutime + cstime;

  // Calculate system-wide CPU time
  std::ifstream cpuStatFile(PROC_STAT_PATH);
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
  total_time -= prev_proc_time;
  system_total_time -= prev_total_time;

  prev_proc_time = total_time;
  prev_total_time = system_total_time;

  // Return CPU usage as percentage
  if (system_total_time == 0)
    return 0.0;
  return (static_cast<double>(total_time) / system_total_time) * 100.0;
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
