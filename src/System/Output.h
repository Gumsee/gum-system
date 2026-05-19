#pragma once
#include "File.h"
#include "Filesystem.h"
#include <string>
#include <fstream>
#include <time.h>
#include <iostream>
#include "Terminal.h"
#include <mutex>
#include <sstream>

#define guminfo  Gum::Output("[I]", Gum::Terminal::COLORS::LIGHT_GRAY)
#define gumwarn  Gum::Output("[W]", Gum::Terminal::COLORS::YELLOW)
#define gumerror Gum::Output("[E]", Gum::Terminal::COLORS::LIGHT_RED)
#define gumfatal Gum::Output("[F]", Gum::Terminal::COLORS::RED)
#define gumdebug Gum::Output("[D]", Gum::Terminal::COLORS::GREEN)

namespace Gum {
class Output
{
private:
  static inline Filesystem::File logFile;
  static inline Filesystem::File debuglogFile;
  static inline bool stoprunning = false;
  static inline clock_t t1 = clock(), t2 = clock();
  std::mutex oPrintMutex;
  std::ostringstream oss;

public:
  Output(const std::string& prefix = "", const std::string& col = "")
  {
    oss << Terminal::COLORS::GRAY << getCurrentTime() << col << " " << prefix << " ";
  }

  template <class T>
  Output &operator<<(const T &v)
  {
    oss << v;
    return *this;
  }

  ~Output()
  {
    oPrintMutex.lock();
    oss << Terminal::COLORS::RESET << std::endl;
    std::cout << oss.str();
    oPrintMutex.unlock();
  }
  
  static void log(const std::string& message);
  static void error(const std::string& message);
  static void fatal(const std::string& message);
  static void warn(const std::string& message);
  static void info(const std::string& message, bool newline = true, bool brackets = true);
  static void debug(const std::string& message);

  static std::string getCurrentTime();
  static void Failed();
  static bool wasSuccessful();

  template<typename T>
  static inline void print(const T& printable)
  {
      std::cout << Terminal::COLORS::GRAY << getCurrentTime() << Terminal::COLORS::RESET << " " << printable << std::endl;
  }

  static std::string getOutputSpacing();
};

}