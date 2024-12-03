
#include "../include/logger.h"
#include <iostream>
#include <spdlog/sinks/basic_file_sink.h> // Required for the file sink

namespace {
// Constants for meaningful values
const std::string LOG_DIRECTORY = "logs"; // Directory for logs
const std::string LOG_FILE_PATH = "logs/process_manager.log"; // Log file path
const std::string LOG_PATTERN =
    "[%Y-%m-%d %H:%M:%S] [%l] %v"; // Log format pattern
const int DISPLAY_LOG_LINES = 10;  // Number of recent logs to display
} // namespace

Logger::Logger() { initializeLogger(); }

void Logger::initializeLogger() {
  // Ensure the logs directory exists
  std::filesystem::create_directories(LOG_DIRECTORY);

  // Create or retrieve the logger
  logger_ = spdlog::get("basic_logger");
  if (!logger_) {
    logger_ = spdlog::basic_logger_mt("basic_logger", LOG_FILE_PATH);
    spdlog::set_pattern(LOG_PATTERN);
  }
}

void Logger::logAction(const std::string &action) {
  if (logger_) {
    logger_->info(action);
    logger_->flush(); // Ensure logs are immediately written to the file
  } else {
    std::cerr << "Logger is not initialized.\n";
  }
}

void Logger::logError(const std::string &error) {
  if (logger_) {
    logger_->error(error);
    logger_->flush(); // Ensure logs are immediately written to the file
  } else {
    std::cerr << "Logger is not initialized.\n";
  }
}

void Logger::logWarning(const std::string &warning) {
  if (logger_) {
    logger_->warn(warning);
    logger_->flush(); // Ensure logs are immediately written to the file
  } else {
    std::cerr << "Logger is not initialized.\n";
  }
}

void Logger::displayRecentLogs() {
  std::ifstream logFile(LOG_FILE_PATH);
  if (logFile.is_open()) {
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(logFile, line)) {
      lines.push_back(line);
    }
    logFile.close();

    // Display the last DISPLAY_LOG_LINES lines
    int start = std::max(0, static_cast<int>(lines.size()) - DISPLAY_LOG_LINES);
    for (size_t i = start; i < lines.size(); ++i) {
      std::cout << lines[i] << '\n';
    }
  } else {
    std::cerr << "Unable to open log file.\n";
  }
}
