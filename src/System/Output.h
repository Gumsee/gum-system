#pragma once
#include "File.h"
#include "Filesystem.h"
#include <Maths/vec.h>
#include <Maths/mat.h>
#include <string>
#include <fstream>
#include <time.h>
#include <iostream>

namespace Gum {
namespace Output
{
    extern Filesystem::File logFile;
    extern Filesystem::File debuglogFile;
    inline bool stoprunning = false;
    inline clock_t t1 = clock(), t2 = clock();
	inline bool issuccessful = true;

    extern void init(const Filesystem::File& logfilestr = Filesystem::File(Gum::Filesystem::getExecutablePath().toString() + "/gum.log"), const Filesystem::File& Debuglogfilestr = Filesystem::File("", Filesystem::Filetype::UNKNOWN));
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
    static inline void print(const T& printable)
    {
        std::cout << getCurrentTime() << " " << printable << "\n";
    }

    template<typename T, unsigned int S> 
    static inline void print(tvec<T, S> printable) 
    { 
        std::cout << getCurrentTime() << " " << printable.toString() << "\n"; 
    }

    template<typename T, unsigned int N, unsigned int M> 
    static inline void print(mat<T, N, M> printable) 
    { 
        std::cout << getCurrentTime() << " " << printable.toString() << "\n"; 
    }
}}