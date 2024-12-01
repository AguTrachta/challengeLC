/**
 * @file thread_pool.h
 * @brief Header file for the ThreadPool class that manages a pool of worker
 * threads.
 *
 * The ThreadPool class allows for the parallel execution of tasks using a fixed
 * number of threads. It supports adding tasks to a queue, which are executed by
 * the worker threads. The pool can be stopped gracefully and can wait for all
 * tasks to finish.
 */

#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <atomic>
#include <condition_variable>
#include <functional>
#include <memory>
#include <queue>
#include <thread>
#include <vector>

/**
 * @class ThreadPool
 * @brief A class for managing a pool of worker threads to execute tasks
 * concurrently.
 *
 * This class creates a fixed number of threads that wait for tasks to be added
 * to a queue. Once a task is added, the threads will execute the task
 * concurrently. The pool can be stopped gracefully and can wait until all tasks
 * are completed before destruction.
 */
class ThreadPool {
public:
  /**
   * @brief Constructor that initializes the thread pool with the specified
   * number of threads.
   *
   * Creates the given number of worker threads that will be used to execute the
   * tasks in parallel.
   * @param numThreads The number of threads to be created in the pool.
   */
  explicit ThreadPool(size_t numThreads);

  /**
   * @brief Destructor that ensures all threads are joined before destruction.
   *
   * Stops the pool, waits for all tasks to finish, and joins all worker
   * threads.
   */
  ~ThreadPool();

  /**
   * @brief Templated method to enqueue a task into the thread pool.
   *
   * Adds a task to the task queue. The task will be executed by one of the
   * worker threads.
   * @param f The task to be enqueued.
   */
  template <typename F> void enqueue(F &&f);

  /**
   * @brief Explicit method to stop the pool gracefully.
   *
   * Sets the stop flag and notifies all threads to finish their tasks and exit.
   */
  void stop();

  /**
   * @brief Waits for all tasks to complete.
   *
   * Blocks the caller until all tasks in the queue have been completed.
   */
  void waitForAll();

private:
  /**
   * @brief Worker thread function that processes tasks from the queue.
   *
   * Each worker thread continuously processes tasks from the task queue until
   * the pool is stopped.
   */
  void worker();

  // Vector to store worker threads
  std::vector<std::thread> workers_;

  // Queue to hold tasks
  std::queue<std::function<void()>> tasks_;

  // Mutex for protecting access to task queue
  std::mutex tasksMutex_;

  // Condition variable to notify workers about tasks
  std::condition_variable cv_;

  // Flag to indicate whether the pool is stopped
  std::atomic<bool> stop_;

  // Atomic counter to track active tasks
  std::atomic<int> activeTasks_;
};

// Template function definition inside the header
template <typename F> void ThreadPool::enqueue(F &&f) {
  {
    std::lock_guard<std::mutex> lock(tasksMutex_);
    tasks_.emplace(std::forward<F>(f)); // Add the task to the queue
  }
  cv_.notify_one(); // Notify one thread that a new task is available
}

#endif // THREAD_POOL_H
