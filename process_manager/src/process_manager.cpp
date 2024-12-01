// src/process_manager.cpp

#include "../include/process_manager.h"
#include "../include/command_parser.h"
#include "../include/logger.h"
#include "../include/process_control.h"
#include "../include/process_listing.h"
#include "../include/resource_monitoring.h"

#include <iostream>

ProcessManager::ProcessManager() {
  // Initialize logger or other resources if necessary
}

void ProcessManager::run() {
  displayWelcomeScreen();
  startInteractiveLoop();
}

void ProcessManager::displayWelcomeScreen() {
  std::cout << "========================================\n";
  std::cout
      << "  \033[1;34m\033[4mHigh-Performance Process Manager\033[0m\n"; // Bold,
                                                                         // underlined,
                                                                         // and
                                                                         // blue
  std::cout << "========================================\n";
  std::cout
      << "Type '\033[1mhelp\033[0m' to see available commands.\n"; // Bold
                                                                   // "help"
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

  if (parsedCommand.name == "list") {
    ProcessListing processListing;
    processListing.listProcesses();
  } else if (parsedCommand.name == "monitor") {
    ResourceMonitoring resourceMonitor;
    resourceMonitor.startMonitoring();
  } else if (parsedCommand.name == "kill") {
    if (parsedCommand.args.empty()) {
      std::cerr << "Error: 'kill' command requires a PID.\n";
      return;
    }
    int pid = std::stoi(parsedCommand.args[0]);
    ProcessControl processControl;
    processControl.terminateProcess(pid);
  } else if (parsedCommand.name == "log") {
    Logger logger;
    logger.displayRecentLogs();
  } else if (parsedCommand.name == "help") {
    showHelp();
  } else if (parsedCommand.name == "exit") {
    std::cout << "Exiting...\n";
    // Perform any necessary cleanup
    exit(0);
  } else {
    std::cerr << "Unknown command: " << parsedCommand.name << "\n";
    std::cout << "Type 'help' to see available commands.\n";
  }
}

void ProcessManager::showHelp() {
  std::cout << "\nAvailable Commands:\n";
  std::cout << "  list           - List all active processes.\n";
  std::cout
      << "  monitor        - Monitor CPU and memory usage in real-time.\n";
  std::cout << "  kill <pid>     - Terminate a process by PID.\n";
  std::cout << "  log            - Display recent log entries.\n";
  std::cout << "  help           - Show this help message.\n";
  std::cout << "  exit           - Exit the program.\n";
}
