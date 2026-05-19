#include "ThreadPool.h"
#include "MemoryManagement.h"
#include "Output.h"

ThreadPool::ThreadPool(unsigned int size)
  : iPoolSize(size)
{
  guminfo << "Creating " << iPoolSize << " threads";
  
  for(unsigned int i = 0; i < iPoolSize; ++i)
  {
    vThreads.emplace_back([this] {
      while(true) 
      {
        std::function<void()> func;
        {
          std::unique_lock<std::mutex> lock(oQueueMutex);

          oConditionVariable.wait(lock, [this] {
            return !qFunctions.empty() || bStop;
          });
          if (qFunctions.empty() && bStop)
            return;

          func = std::move(qFunctions.front());
          qFunctions.pop();
        }

        func();
        iPendingFunctions--;
      }
    });
  }
}

ThreadPool::~ThreadPool()
{
  {
    // Lock the queue to update the stop flag safely
    std::unique_lock<std::mutex> lock(oQueueMutex);
    bStop = true;
  }

  // Notify all threads
  oConditionVariable.notify_all();

  // Joining all worker threads to ensure they have
  // completed their tasks
  for(std::thread& thread : vThreads)
    thread.join();
}


void ThreadPool::addThread(std::function<void()> func)
{
  if(pPool == nullptr)
    pPool = new ThreadPool();

  {
    std::unique_lock<std::mutex> lock(pPool->oQueueMutex);
    pPool->qFunctions.emplace(std::move(func));
    pPool->iPendingFunctions++;
  }
  pPool->oConditionVariable.notify_one();
}

void ThreadPool::wait()
{
  if(pPool == nullptr)
    return;

  while(pPool->iPendingFunctions > 0) { }
}

void ThreadPool::stop()
{
  if(pPool == nullptr)
    return;
  
  Gum::_delete(pPool);
}

void ThreadPool::initPoolSize(unsigned int size)
{
  if(pPool == nullptr)
    pPool = new ThreadPool(size);
}
