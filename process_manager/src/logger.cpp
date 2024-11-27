#include "../include/logger.h"
#include <iostream>
#include <spdlog/sinks/basic_file_sink.h> // Required for the file sink

Logger::Logger() { initializeLogger(); }

void Logger::initializeLogger() {
  // Ensure the logs directory exists
  std::filesystem::create_directories("logs");

  // Create or retrieve the logger
  logger_ = spdlog::get("basic_logger");
  if (!logger_) {
    logger_ =
        spdlog::basic_logger_mt("basic_logger", "logs/process_manager.log");
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%l] %v");
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

void Logger::displayRecentLogs() {
  const int N = 10;
  std::ifstream logFile("logs/process_manager.log");
  if (logFile.is_open()) {
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(logFile, line)) {
      lines.push_back(line);
    }
    logFile.close();

    // Display the last N lines
    int start = std::max(0, static_cast<int>(lines.size()) - N);
    for (size_t i = start; i < lines.size(); ++i) {
      std::cout << lines[i] << '\n';
    }
  } else {
    std::cerr << "Unable to open log file.\n";
  }
}

