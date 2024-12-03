#include "../include/logger.h"
#include "../include/process_manager.h"

int main() {
  // Ensure logger is initialized
  Logger logger;
  logger.logAction("Application started");

  // Run the process manager
  ProcessManager manager;
  try {
    manager.run();
    logger.logAction("Application finished successfully");
  } catch (const std::exception &ex) {
    logger.logError(std::string("Application encountered an error: ") +
                    ex.what());
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
