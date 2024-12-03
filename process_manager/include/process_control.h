/**
 * @file process_control.h
 * @brief Provides functionality to terminate processes by their PID.
 *
 * This file defines the `ProcessControl` class, which is responsible for
 * terminating processes by sending termination signals (SIGTERM and SIGKILL) to
 * a process identified by its PID.
 */

#ifndef PROCESS_CONTROL_H
#define PROCESS_CONTROL_H

/**
 * @class ProcessControl
 * @brief A class for controlling processes, specifically terminating them.
 *
 * The `ProcessControl` class provides a method to terminate a process using its
 * PID. It checks the validity of the PID, verifies if the process exists, and
 * attempts to terminate the process gracefully (SIGTERM), followed by a forced
 * termination (SIGKILL) if needed. It also logs the termination attempts and
 * outcomes using the `Logger` class.
 */
class ProcessControl {
public:
  /**
   * @brief Terminates a process using its PID.
   *
   * This method validates the given PID, checks whether the process exists, and
   * attempts to terminate it using the SIGTERM signal. If the process does not
   * terminate after SIGTERM, a SIGKILL signal is sent for forced termination.
   *
   * @param[in] pid The PID (Process ID) of the process to terminate.
   *
   * @note Logs will be generated for any errors, warnings, or successful
   * actions.
   */
  void terminateProcess(int pid);
};

#endif // PROCESS_CONTROL_H
