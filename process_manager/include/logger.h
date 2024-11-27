// include/logger.h

#ifndef LOGGER_H
#define LOGGER_H

#include "spdlog/spdlog.h"
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>

class Logger {
public:
  Logger();
  void logAction(const std::string &action);
  void logError(const std::string &error);
  void logWarning(const std::string &warning);
  void displayRecentLogs();
  void initializeLogger();

private:
  std::shared_ptr<spdlog::logger> logger_;
};

#endif // LOGGER_H
