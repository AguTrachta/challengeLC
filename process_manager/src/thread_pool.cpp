#include "../include/thread_pool.h"
#include <iostream>

// Constructor initializes the thread pool with the given number of threads
ThreadPool::ThreadPool(size_t numThreads) : stop_(false), activeTasks_(0) {
  // Create and start the worker threads
  for (size_t i = 0; i < numThreads; ++i) {
    workers_.emplace_back(&ThreadPool::worker, this);
  }
}

// Destructor: ensures all tasks are completed before destroying the pool
ThreadPool::~ThreadPool() {
  {
    std::lock_guard<std::mutex> lock(tasksMutex_);
    stop_ = true; // Set the stop flag to true
  }
  cv_.notify_all(); // Notify all threads to stop

  // Join all worker threads
  for (auto &worker : workers_) {
    if (worker.joinable()) {
      worker.join();
    }
  }
}

// Worker thread function
void ThreadPool::worker() {
  while (true) {
    std::function<void()> task;
    {
      std::unique_lock<std::mutex> lock(tasksMutex_);

      // Wait until there are tasks or the stop flag is set
      cv_.wait(lock, [this] { return !tasks_.empty() || stop_; });

      if (stop_ && tasks_.empty()) {
        return; // Stop thread if the pool is stopped and no tasks remain
      }

      task = std::move(tasks_.front()); // Get the next task
      tasks_.pop();                     // Remove the task from the queue
    }

    // Execute the task outside the lock
    task();

    // After task completion, decrement the active task count
    --activeTasks_;
  }
}

// Wait for all tasks to finish
void ThreadPool::waitForAll() {
  while (activeTasks_ > 0) {
    // Wait until there are no active tasks
    std::this_thread::sleep_for(
        std::chrono::milliseconds(100)); // Sleep to avoid busy-waiting
  }
}

