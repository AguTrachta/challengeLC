// In resource_monitoring_test.cpp
#include "../include/resource_monitoring.h"
#include "gtest/gtest.h"
#include <thread>

class ResourceMonitoringTest : public ::testing::Test {
protected:
  void SetUp() override {
    // Setup any necessary resources for the tests
  }

  void TearDown() override {
    // Cleanup any resources
  }
};

TEST_F(ResourceMonitoringTest, MonitoringIsNotActiveInitially) {
  ResourceMonitoring resourceMonitoring;

  // Check if the monitoring flag is false initially
  EXPECT_FALSE(resourceMonitoring.getMonitoringBool());
}

TEST_F(ResourceMonitoringTest, StartMonitoringSetsMonitoringTrue) {
  ResourceMonitoring resourceMonitoring;

  // Override the waitForStopInput to simulate Enter press immediately
  resourceMonitoring.getDataMonitor()
      .stopMonitoring(); // Stop monitoring immediately in the test

  // Start monitoring in a separate thread
  std::thread monitorThread(&ResourceMonitoring::startMonitoring,
                            &resourceMonitoring);

  // Give the monitoring thread a moment to start
  std::this_thread::sleep_for(
      std::chrono::seconds(1)); // Sleep for a bit to allow the flag to be set

  // Check if the monitoring flag was set to true
  EXPECT_TRUE(resourceMonitoring.getMonitoringBool());

  // Stop monitoring and join the thread
  resourceMonitoring.stopMonitoring();
  monitorThread.join();
}

TEST_F(ResourceMonitoringTest, StopMonitoringSetsMonitoringFalse) {
  ResourceMonitoring resourceMonitoring;

  // Start monitoring in a separate thread
  std::thread monitorThread(&ResourceMonitoring::startMonitoring,
                            &resourceMonitoring);

  // Wait a bit to allow the thread to start
  std::this_thread::sleep_for(std::chrono::seconds(1));

  // Now stop the monitoring process
  resourceMonitoring.stopMonitoring();

  // Check if the monitoring flag was set to false
  EXPECT_FALSE(resourceMonitoring.getMonitoringBool());

  // Join the thread after stopping the monitoring
  monitorThread.join();
}

TEST_F(ResourceMonitoringTest,
       MultipleStartMonitoringCallsDontCreateNewThreads) {
  ResourceMonitoring resourceMonitoring;

  // Start monitoring in a separate thread
  std::thread monitorThread(&ResourceMonitoring::startMonitoring,
                            &resourceMonitoring);

  // Give it a moment to start
  std::this_thread::sleep_for(std::chrono::seconds(1));

  // Capture the initial thread count (using std::thread::hardware_concurrency()
  // as a proxy)
  unsigned int initialThreadCount = std::thread::hardware_concurrency();

  // Call startMonitoring again without stopping the first thread
  resourceMonitoring.startMonitoring();

  // Give it a moment to ensure no new threads are created
  std::this_thread::sleep_for(std::chrono::seconds(1));

  // Capture the number of threads again
  unsigned int updatedThreadCount = std::thread::hardware_concurrency();

  // Ensure that the number of threads hasn't increased
  EXPECT_EQ(initialThreadCount, updatedThreadCount);

  // Stop monitoring and join the thread
  resourceMonitoring.stopMonitoring();
  monitorThread.join();
}
