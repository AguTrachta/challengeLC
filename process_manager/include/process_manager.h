// include/process_manager.h

#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <string>

class ProcessManager {
public:
  ProcessManager();
  void run();

private:
  void displayWelcomeScreen();
  void startInteractiveLoop();
  void handleCommand(const std::string &command);
  void showHelp();
};

#endif // PROCESS_MANAGER_H
