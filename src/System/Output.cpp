#include "Output.h"
#include "Filesystem.h"
#define DEBUG 1

namespace Gum {
namespace Output
{
    std::string logfilepath = Gum::Filesystem::getExecutablePath() + "/gum.log";

    void init(const std::string& logfilestr, const std::string& Debuglogfilestr)
    {
        Debuglogfilepath = Debuglogfilestr;
        logfilepath = logfilestr;

        if(logfilepath == "")
            logfilepath = Gum::Filesystem::getExecutablePath() + "/gum.log";
        Gum::Filesystem::writeToFile(logfilepath, "--- Default logfile Start ---\n");

        if(Debuglogfilepath != "")
            Gum::Filesystem::writeToFile(Debuglogfilepath, "--- Debug logfile start ---\n");

    }

    void log(const std::string& message)
    {
        Gum::Filesystem::appendToFile(logfilepath, getCurrentTime() + "[Info] " + message + "\n");
    }

    void error(const std::string& message)
    {
        Gum::Filesystem::appendToFile(logfilepath, getCurrentTime() + "[Error] " + message + "\n");
        std::cerr << getCurrentTime() << " Error " + message + "\n";
        stoprunning = true;
    }

    void fatal(const std::string& message)
    {
        Gum::Filesystem::appendToFile(logfilepath, getCurrentTime() + "[Fatal] " + message + "\n");
        std::cout << getCurrentTime() << "[Fatal] " + message + "\n";
        stoprunning = true;
    }

    void warn(const std::string& message)
    {
        Gum::Filesystem::appendToFile(logfilepath, getCurrentTime() + "[Warn] " + message + "\n");
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
        Gum::Filesystem::appendToFile(logfilepath, outStr);
    }
    void debug(const std::string& message)
    {
        #ifdef DEBUG
            if(Debuglogfilepath != "")
            {
                Gum::Filesystem::appendToFile(Debuglogfilepath, "[Debug] " + message + "\n");
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