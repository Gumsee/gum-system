#include "Output.h"
#include "File.h"
#include "Filesystem.h"
#define DEBUG 1

namespace Gum {
namespace Output
{
    Filesystem::File logFile(Gum::Filesystem::getExecutablePath().toString() + "/gum.log");
    Filesystem::File debuglogFile("", Filesystem::UNKNOWN);

    void init(const Filesystem::File& logfile, const Filesystem::File& debuglogfile)
    {
        logFile = logfile;
        debuglogFile = debuglogfile;

        if(logFile.getType() != Filesystem::Filetype::FILE)
            logFile = Filesystem::File(Gum::Filesystem::getExecutablePath().toString() + "/gum.log");
        Gum::Filesystem::writeToFile(logFile, "--- Default logfile Start ---\n");

        if(debuglogFile.getType() == Filesystem::Filetype::FILE)
            Gum::Filesystem::writeToFile(debuglogFile, "--- Debug logfile start ---\n");

    }

    void log(const std::string& message)
    {
        Gum::Filesystem::appendToFile(logFile, getCurrentTime() + "[Info] " + message + "\n");
    }

    void error(const std::string& message)
    {
        Gum::Filesystem::appendToFile(logFile, getCurrentTime() + "[Error] " + message + "\n");
        std::cerr << getCurrentTime() << " Error " + message + "\n";
        stoprunning = true;
    }

    void fatal(const std::string& message)
    {
        Gum::Filesystem::appendToFile(logFile, getCurrentTime() + "[Fatal] " + message + "\n");
        std::cout << getCurrentTime() << "[Fatal] " + message + "\n";
        stoprunning = true;
    }

    void warn(const std::string& message)
    {
        Gum::Filesystem::appendToFile(logFile, getCurrentTime() + "[Warn] " + message + "\n");
        std::cerr << getCurrentTime() << " Warn " + message + "\n";
    }
    void info(const std::string& message, bool newline, bool brackets)
    {
        std::string outStr = "";
        if(newline)
        {
            if(brackets)
            {
                outStr = getCurrentTime() + "[Info] " + message + "\n";
            }
            else
            {
                outStr = message + "\n";
            }
        }
        else
        {
            if(brackets)
            {
                outStr = getCurrentTime() + "[Info] " + message;
            }
            else
            {
                outStr = message;
            }
        }
        std::cout << outStr;
        Gum::Filesystem::appendToFile(logFile, outStr);
    }
    void debug(const std::string& message)
    {
        #ifdef DEBUG
            if(debuglogFile.getType() == Filesystem::Filetype::FILE)
            {
                Gum::Filesystem::appendToFile(debuglogFile, "[Debug] " + message + "\n");
                std::cout << getCurrentTime() << "[Debug] " + message + "\n";
            }
        #endif
    }

    std::string getCurrentTime()
    {
        t2=clock();
        float diff ((float)t2-(float)t1);

        return "[" + std::to_string(diff / CLOCKS_PER_SEC) + "]";
    }

    void Failed()
	{
		issuccessful = false;
		std::cout << "\033[1;31m /\\--------------------> [PROGRAM TERMINATED] <--------------------/\\\n \033[0m" << std::endl;
	}

	bool wasSuccessful()
	{
		return issuccessful;
	}
}}