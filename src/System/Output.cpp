#include "Output.h"
#define DEBUG 1

namespace Gum {
namespace Output
{
    std::ofstream logfile;
    std::string logfilepath;
    std::string Debuglogfilepath;
    bool stoprunning = false;
    clock_t t1,t2;
	bool issuccessful = true;

    void init(std::string logfilestr, std::string Debuglogfilestr)
    {
        Debuglogfilepath = Debuglogfilestr;
        logfilepath = logfilestr;
        logfile.open(logfilepath);
        logfile << "--- Default logfile Start ---\n";
        logfile.close();

        if(Debuglogfilepath != "")
        {
            logfile.open(Debuglogfilepath);
            logfile << "--- Debug logfile start ---\n";
            logfile.close();
        }

        t1=clock();
    }

    void log(std::string message)
    {
        logfile.open(logfilepath, std::ios::app);
        logfile << getCurrentTime() << ("[Info] " + message + "\n").c_str();
        logfile.close();
    }

    void error(std::string message)
    {
        logfile.open(logfilepath, std::ios::app);
        logfile << getCurrentTime() << ("[Error] " + message + "\n").c_str();
        logfile.close();
        std::cerr << getCurrentTime() << " Error " + message + "\n";
        stoprunning = true;
    }

    void fatal(std::string message)
    {
        logfile.open(logfilepath, std::ios::app);
        logfile << getCurrentTime() << ("[Fatal] " + message + "\n").c_str();
        logfile.close();
        std::cout << getCurrentTime() << "[Fatal] " + message + "\n";
        stoprunning = true;
    }

    void warn(std::string message)
    {
        logfile.open(logfilepath, std::ios::app);
        logfile << getCurrentTime() << ("[Warn] " + message + "\n").c_str();
        logfile.close();
    }
    void info(std::string message, bool newline, bool brackets)
    {
        logfile.open(logfilepath, std::ios::app);
        std::string outStr = "";
        if(newline)
        {
            if(brackets)
            {
                outStr = getCurrentTime() + ("[Info] " + message + "\n").c_str();
            }
            else
            {
                outStr = (message + "\n").c_str();
            }
        }
        else
        {
            if(brackets)
            {
                outStr = getCurrentTime() + ("[Info] " + message).c_str();
            }
            else
            {
                outStr = (message).c_str();
            }
        }
        std::cout << outStr;
        logfile << outStr;
        logfile.close();
    }
    void debug(std::string message)
    {
        #ifdef DEBUG
            logfile.open(Debuglogfilepath, std::ios::app);
            logfile << getCurrentTime() << ("[Debug] " + message + "\n").c_str();
            logfile.close();
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