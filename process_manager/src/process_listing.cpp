// src/process_listing.cpp

#include "../include/process_listing.h"
#include "../include/logger.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <future>
#include <iostream>
#include <thread>
#include <vector>

namespace fs = std::filesystem;

ProcessListing::ProcessListing() {
  // Constructor if needed
}

void ProcessListing::listProcesses() {
  Logger logger;
  logger.logAction("Listing processes");

  fetchProcessList();

  // Display the processes
  std::cout << "PID\tCPU%\tMemory%\tName\n";
  for (const auto &process : processes_) {
    std::cout << process.pid << '\t' << process.cpuUsage << '\t'
              << process.memoryUsage << '\t' << process.name << '\n';
  }
}

void ProcessListing::fetchProcessList() {
  std::vector<int> pids = getAllPIDs();
  std::vector<std::future<void>> futures;

  // Use a thread pool to fetch process info concurrently
  for (int pid : pids) {
    futures.push_back(std::async(std::launch::async,
                                 &ProcessListing::fetchProcessInfo, this, pid));
  }

  // Wait for all threads to finish
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
  // Placeholder implementation
  // Calculating CPU usage requires reading /proc/[pid]/stat and system uptime
  // For simplicity, return 0.0 for now
  return 0.0;
}

double ProcessListing::calculateMemoryUsage(int pid) {
  // Placeholder implementation
  // Calculating memory usage involves reading /proc/[pid]/statm or status
  // For simplicity, return 0.0 for now
  return 0.0;
}
