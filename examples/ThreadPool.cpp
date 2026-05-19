#include "System/File.h"
#include <System/Filesystem.h>
#include <System/Output.h>
#include <System/ThreadPool.h>

int main(int argc, char** argv)
{
  ThreadPool::initPoolSize(5);

  for(unsigned int i = 0; i < 25; i++)
  {
    ThreadPool::addThread([i](){
      guminfo << "Thread " << i << " " << std::this_thread::get_id();
      std::this_thread::sleep_for(std::chrono::milliseconds(150));
    });
  }

  ThreadPool::wait();

	return 0;
};
