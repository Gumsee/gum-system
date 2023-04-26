#pragma once
#include <Maths/vec.h>
#include <string>
#include <fstream>
#include <time.h>
#include <iostream>

namespace Gum {
namespace Output
{
    extern std::string logfilepath;
    inline std::string Debuglogfilepath = "";
    inline bool stoprunning = false;
    inline clock_t t1 = clock(), t2 = clock();
	inline bool issuccessful = true;

    extern void init(const std::string& logfilestr = "", const std::string& Debuglogfilestr = "");
    extern void log(const std::string& message);
    extern void error(const std::string& message);
    extern void fatal(const std::string& message);
    extern void warn(const std::string& message);
    extern void info(const std::string& message, bool newline = true, bool brackets = true);
    extern void debug(const std::string& message);

    extern std::string getCurrentTime();
	extern void Failed();
	extern bool wasSuccessful();

    template<typename T>
    static inline void print(T printable)
    {
        std::cout << getCurrentTime() << " " << printable << "\n";
    }

    template<typename T, unsigned int S> 
    static inline void print(tvec<T, S> printable) 
    { 
        std::cout << getCurrentTime() << " " << printable.toString() << "\n"; 
    }
}}