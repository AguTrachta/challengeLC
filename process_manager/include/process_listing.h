/**
 * @file process_listing.h
 * @brief Provides functionality for listing and retrieving system processes.
 *
 * This file defines the `ProcessListing` class, which allows for fetching and
 * displaying a list of system processes along with their resource usage (CPU
 * and memory). The processes are identified by their PID, and the resource
 * usage statistics are calculated for each process.
 */

#ifndef PROCESS_LISTING_H
#define PROCESS_LISTING_H

#include <mutex>
#include <string>
#include <vector>

/**
 * @struct ProcessInfo
 * @brief Holds information about a process.
 *
 * This structure stores details of a process such as its PID, name, CPU usage
 * percentage, and memory usage percentage.
 */
struct ProcessInfo {
  int pid;            ///< Process ID
  std::string name;   ///< Name of the process
  double cpuUsage;    ///< CPU usage percentage
  double memoryUsage; ///< Memory usage percentage
};

/**
 * @class ProcessListing
 * @brief A class for listing and retrieving process information.
 *
 * The `ProcessListing` class provides functionality to list all system
 * processes along with their CPU and memory usage. The class retrieves process
 * information asynchronously to improve performance and utilizes a mutex to
 * ensure thread safety when modifying shared data.
 */
class ProcessListing {
public:
  /**
   * @brief Constructor for the ProcessListing class.
   *
   * Initializes the object and prepares it for listing processes.
   */
  ProcessListing();

  /**
   * @brief Lists all processes with their resource usage.
   *
   * This method retrieves a list of processes, calculates their CPU and memory
   * usage, and displays the information in a formatted table. Processes with
   * high CPU or memory usage are displayed in red or yellow for better
   * visibility.
   */
  void listProcesses();

private:
  std::vector<ProcessInfo> processes_; ///< List of processes with their details
  std::mutex mutex_; ///< Mutex to synchronize access to shared data

  /**
   * @brief Fetches the list of all process PIDs.
   *
   * This method reads the `/proc` directory to collect all process IDs (PIDs)
   * of running processes. It returns a vector of PIDs.
   *
   * @return A vector containing all process PIDs.
   */
  static std::vector<int> getAllPIDs();

  /**
   * @brief Fetches information for a specific process by its PID.
   *
   * This method retrieves the process name, CPU usage, and memory usage for the
   * given PID. It stores the information in the `processes_` list.
   *
   * @param pid The PID of the process whose information is to be fetched.
   */
  void fetchProcessInfo(int pid);

  /**
   * @brief Fetches the list of processes asynchronously.
   *
   * This method divides the list of PIDs into smaller batches and uses multiple
   * threads to fetch the process information concurrently.
   */
  void fetchProcessList();

  /**
   * @brief Retrieves the name of a process given its PID.
   *
   * This method reads the process name from `/proc/<pid>/comm`.
   *
   * @param pid The PID of the process.
   * @return The name of the process.
   */
  std::string getProcessName(int pid);

  /**
   * @brief Calculates the CPU usage of a process.
   *
   * This method reads the `/proc/<pid>/stat` file to compute the CPU usage of a
   * process. It uses the difference in process times to calculate the CPU usage
   * percentage.
   *
   * @param pid The PID of the process.
   * @return The CPU usage percentage.
   */
  double calculateCPUUsage(int pid);

  /**
   * @brief Calculates the memory usage of a process.
   *
   * This method reads the `/proc/<pid>/statm` file to determine the memory
   * usage of a process. It compares the process's resident memory with the
   * total system memory to compute the memory usage percentage.
   *
   * @param pid The PID of the process.
   * @return The memory usage percentage.
   */
  double calculateMemoryUsage(int pid);
};

#endif // PROCESS_LISTING_H

