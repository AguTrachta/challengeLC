/**
 * @file process_manager.h
 * @brief Provides functionality for managing system processes interactively.
 *
 * This file defines the `ProcessManager` class, which facilitates interactive
 * process management through a terminal interface. It includes commands to list
 * processes, monitor resource usage, terminate processes, view logs, and show a
 * help message.
 */

#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <string>

/**
 * @class ProcessManager
 * @brief A class for managing and interacting with system processes.
 *
 * The `ProcessManager` class provides an interactive command-line interface for
 * managing system processes. It supports commands such as listing processes,
 * monitoring system resources, killing processes, viewing logs, and displaying
 * help information. The program runs in an interactive loop to accept user
 * commands.
 */
class ProcessManager {
public:
  /**
   * @brief Constructor for the ProcessManager class.
   *
   * Initializes the `ProcessManager` object. This may include setting up
   * resources like a logger or initializing other components necessary for
   * process management.
   */
  ProcessManager();

  /**
   * @brief Starts the process manager's interactive session.
   *
   * This method begins the interactive loop, which displays the welcome screen
   * and waits for user input. It processes each command entered by the user and
   * performs the corresponding action.
   */
  void run();

private:
  /**
   * @brief Displays the welcome screen.
   *
   * This method displays an introductory message with the name of the process
   * manager, including some basic formatting for emphasis.
   */
  void displayWelcomeScreen();

  /**
   * @brief Starts the interactive command loop.
   *
   * This method repeatedly prompts the user for input, processes the commands
   * entered, and performs the appropriate actions. It continues until the user
   * decides to exit the program.
   */
  void startInteractiveLoop();

  /**
   * @brief Handles the user command.
   *
   * This method parses and processes the user-entered command. It identifies
   * the command and calls the appropriate functionality, such as listing
   * processes or terminating a process.
   *
   * @param command The command entered by the user.
   */
  void handleCommand(const std::string &command);

  /**
   * @brief Displays the help message with available commands.
   *
   * This method displays a list of commands supported by the process manager
   * along with a brief description of each command's functionality.
   */
  void showHelp();
};

#endif // PROCESS_MANAGER_H
