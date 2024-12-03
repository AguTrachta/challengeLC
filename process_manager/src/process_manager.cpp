
// src/process_manager.cpp

#include "../include/process_manager.h"
#include "../include/command_parser.h"
#include "../include/logger.h"
#include "../include/process_control.h"
#include "../include/process_listing.h"
#include "../include/resource_monitoring.h"

#include <iostream>

// Constants for magic numbers
constexpr const char *WELCOME_HEADER =
    "========================================";
constexpr const char *PROGRAM_TITLE = "High-Performance Process Manager";
constexpr const char *WELCOME_MESSAGE =
    "Type 'help' to see available commands.";
constexpr size_t MAX_COMMAND_LENGTH = 100; // Maximum command input length
constexpr const char *HELP_COMMAND = "help";
constexpr const char *LIST_COMMAND = "list";
constexpr const char *MONITOR_COMMAND = "monitor";
constexpr const char *KILL_COMMAND = "kill";
constexpr const char *LOG_COMMAND = "log";
constexpr const char *EXIT_COMMAND = "exit";
constexpr const char *UNKNOWN_COMMAND_MSG = "Unknown command: ";
constexpr const char *PID_REQUIRED_MSG =
    "Error: 'kill' command requires a PID.";
constexpr const char *EXIT_MSG = "Exiting...";

ProcessManager::ProcessManager() {
  // Initialize logger or other resources if necessary
}

void ProcessManager::run() {
  displayWelcomeScreen();
  startInteractiveLoop();
}

void ProcessManager::displayWelcomeScreen() {
  std::cout << WELCOME_HEADER << '\n';
  std::cout << "  \033[1;34m\033[4m" << PROGRAM_TITLE
            << "\033[0m\n"; // Bold, underlined, and blue
  std::cout << WELCOME_HEADER << '\n';
  std::cout << WELCOME_MESSAGE << '\n';
}

void ProcessManager::startInteractiveLoop() {
  std::string command;
  while (true) {
    std::cout << "\n> ";
    std::getline(std::cin, command);

    if (command.empty()) {
      continue;
    }

    handleCommand(command);
  }
}

void ProcessManager::handleCommand(const std::string &command) {
  CommandParser parser;
  auto parsedCommand = parser.parse(command);

  if (parsedCommand.name == LIST_COMMAND) {
    ProcessListing processListing;
    processListing.listProcesses();
  } else if (parsedCommand.name == MONITOR_COMMAND) {
    ResourceMonitoring resourceMonitor;
    resourceMonitor.startMonitoring();
  } else if (parsedCommand.name == KILL_COMMAND) {
    if (parsedCommand.args.empty()) {
      std::cerr << PID_REQUIRED_MSG << '\n';
      return;
    }
    int pid = std::stoi(parsedCommand.args[0]);
    ProcessControl processControl;
    processControl.terminateProcess(pid);
  } else if (parsedCommand.name == LOG_COMMAND) {
    Logger logger;
    logger.displayRecentLogs();
  } else if (parsedCommand.name == HELP_COMMAND) {
    showHelp();
  } else if (parsedCommand.name == EXIT_COMMAND) {
    std::cout << EXIT_MSG << '\n';
    // Perform any necessary cleanup
    exit(0);
  } else {
    std::cerr << UNKNOWN_COMMAND_MSG << parsedCommand.name << "\n";
    std::cout << "Type 'help' to see available commands.\n";
  }
}

void ProcessManager::showHelp() {
  std::cout << "\nAvailable Commands:\n";
  std::cout << "  " << LIST_COMMAND
            << "           - List all active processes.\n";
  std::cout << "  " << MONITOR_COMMAND
            << "        - Monitor CPU and memory usage in real-time.\n";
  std::cout << "  " << KILL_COMMAND
            << " <pid>     - Terminate a process by PID.\n";
  std::cout << "  " << LOG_COMMAND
            << "            - Display recent log entries.\n";
  std::cout << "  " << HELP_COMMAND << "           - Show this help message.\n";
  std::cout << "  " << EXIT_COMMAND << "           - Exit the program.\n";
}
