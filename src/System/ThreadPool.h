#pragma once
#include <thread>
#include <functional>
#include <queue>
#include <condition_variable>
#include <atomic>

class ThreadPool
{
private:
  static inline ThreadPool* pPool = nullptr;
  unsigned int iPoolSize;
  std::vector<std::thread> vThreads;
  std::queue<std::function<void()> > qFunctions;
  std::mutex oQueueMutex;
  std::condition_variable oConditionVariable;
  bool bStop = false;
  std::atomic<int> iPendingFunctions;

public:
  ThreadPool(unsigned int size = std::thread::hardware_concurrency());
  ~ThreadPool();

  static void addThread(std::function<void()> func);
  static void wait();
  static void stop();
  static void initPoolSize(unsigned int size);
};