/**
 * @file logger.h
 * @brief Provides a logging class for action, error, and warning messages.
 *
 * This file defines the `Logger` class, which is responsible for logging
 * action, error, and warning messages. The class uses the `spdlog` library to
 * log messages to a file. It also provides functionality to display the most
 * recent logs.
 */

#ifndef LOGGER_H
#define LOGGER_H

#include "spdlog/spdlog.h"
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>

/**
 * @class Logger
 * @brief A class for logging action, error, and warning messages.
 *
 * The `Logger` class provides methods to log messages of different severity
 * levels (action, error, and warning). It uses the `spdlog` library to handle
 * log message formatting and output. The logger writes messages to a log file
 * and also allows displaying the most recent log entries.
 */
class Logger {
public:
  /**
   * @brief Constructs a `Logger` object and initializes the logger.
   *
   * This constructor ensures the logger is initialized immediately upon object
   * creation.
   */
  Logger();

  /**
   * @brief Logs an action message.
   *
   * This method logs an informational message representing an action performed
   * by the application.
   *
   * @param[in] action The action message to log.
   */
  void logAction(const std::string &action);

  /**
   * @brief Logs an error message.
   *
   * This method logs an error message when something goes wrong in the
   * application.
   *
   * @param[in] error The error message to log.
   */
  void logError(const std::string &error);

  /**
   * @brief Logs a warning message.
   *
   * This method logs a warning message, typically used for non-critical issues.
   *
   * @param[in] warning The warning message to log.
   */
  void logWarning(const std::string &warning);

  /**
   * @brief Displays the most recent log entries.
   *
   * This method reads the log file and displays the last few log entries to the
   * console. The number of entries displayed is limited to the most recent 10
   * logs.
   */
  void displayRecentLogs();

  /**
   * @brief Initializes the logger by setting up the log file and pattern.
   *
   * This private method ensures that the necessary directories are created and
   * the logger is properly initialized. It configures the log pattern and log
   * file location.
   */
  void initializeLogger();

private:
  std::shared_ptr<spdlog::logger>
      logger_; /**< Shared pointer to the spdlog logger instance. */
};

#endif // LOGGER_H

