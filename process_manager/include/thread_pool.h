#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <atomic>
#include <condition_variable>
#include <functional>
#include <memory>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
public:
  explicit ThreadPool(size_t numThreads);
  ~ThreadPool();

  // Templated method to enqueue tasks
  template <typename F> void enqueue(F &&f);

  // Explicit method to stop the pool gracefully
  void stop();
  void waitForAll();

private:
  void worker();
  std::vector<std::thread> workers_;
  std::queue<std::function<void()>> tasks_;
  std::mutex tasksMutex_;
  std::condition_variable cv_;
  std::atomic<bool> stop_;
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
