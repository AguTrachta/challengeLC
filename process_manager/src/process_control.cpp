#include "../include/process_control.h"
#include "../include/logger.h"

#include <cerrno>
#include <cstring>
#include <iostream>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void ProcessControl::terminateProcess(int pid) {
  Logger logger;

  // Step 1: Validate PID
  if (pid <= 0) {
    std::cerr << "Error: Invalid PID " << pid << ".\n";
    logger.logError("Invalid PID " + std::to_string(pid) + " for termination.");
    return;
  }

  // Step 2: Check process existence
  if (kill(pid, 0) == -1) {
    if (errno == ESRCH) {
      std::cerr << "Error: Process with PID " << pid << " does not exist.\n";
      logger.logError("Attempted to terminate non-existing process PID " +
                      std::to_string(pid) + ".");
    } else if (errno == EPERM) {
      std::cerr << "Error: Permission denied to terminate PID " << pid << ".\n";
      logger.logError("Permission denied to terminate PID " +
                      std::to_string(pid) + ".");
    } else {
      std::cerr << "Error: Unable to check process status for PID " << pid
                << ": " << std::strerror(errno) << ".\n";
      logger.logError("Error checking process status for PID " +
                      std::to_string(pid) + ": " + std::strerror(errno));
    }
    return;
  }

  // Step 3: Terminate the process
  if (kill(pid, SIGTERM) == -1) {
    std::cerr << "Error: Failed to terminate process with PID " << pid << ": "
              << std::strerror(errno) << ".\n";
    logger.logError("Failed to terminate PID " + std::to_string(pid) + ": " +
                    std::strerror(errno));
    return;
  }

  // Step 4: Verify termination
  sleep(1); // Give the system a moment to terminate the process
  if (kill(pid, 0) == 0) {
    // Process still exists, attempt SIGKILL
    std::cerr << "Warning: Process with PID " << pid
              << " did not terminate. Attempting forced termination...\n";
    logger.logWarning("PID " + std::to_string(pid) +
                      " did not terminate. Attempting SIGKILL.");

    if (kill(pid, SIGKILL) == -1) {
      std::cerr << "Error: Failed to forcefully terminate process with PID "
                << pid << ": " << std::strerror(errno) << ".\n";
      logger.logError("Failed to forcefully terminate PID " +
                      std::to_string(pid) + ": " + std::strerror(errno));
      return;
    }
  }

  // Step 5: Log success
  std::cout << "Process with PID " << pid << " terminated successfully.\n";
  logger.logAction("Successfully terminated PID " + std::to_string(pid) + ".");
}
