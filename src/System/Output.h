#pragma once
#include <Maths/vec.h>
#include <string>
#include <fstream>
#include <time.h>
#include <iostream>

namespace Gum {
namespace Output
{
    extern std::ofstream logfile;
    extern std::string logfilepath;
    extern std::string Debuglogfilepath;
    extern bool stoprunning;
    extern clock_t t1,t2;
	extern bool issuccessful;

    extern void init(std::string logfilestr, std::string Debuglogfilestr = "");
    extern void log(std::string message);
    extern void error(std::string message);
    extern void fatal(std::string message);
    extern void warn(std::string message);
    extern void info(std::string message, bool newline = true, bool brackets = true);
    extern void debug(std::string message);

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