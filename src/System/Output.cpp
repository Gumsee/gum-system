#include "Output.h"
#include "File.h"
#include "Filesystem.h"
#include "Terminal.h"
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
        Gum::Filesystem::appendToFile(logFile, getCurrentTime() + "[I] " + message + "\n");
    }

    void error(const std::string& message)
    {
        Gum::Filesystem::appendToFile(logFile, getCurrentTime() + "[E] " + message + "\n");
        std::cerr << Terminal::COLORS::GRAY << getCurrentTime() << Terminal::COLORS::LIGHT_RED << "[E] " << message << Terminal::COLORS::RESET << std::endl;
        stoprunning = true;
    }

    void fatal(const std::string& message)
    {
        Gum::Filesystem::appendToFile(logFile, getCurrentTime() + "[F] " + message + "\n");
        std::cout << Terminal::COLORS::GRAY << getCurrentTime() << Terminal::COLORS::RED << "[F] " << message << Terminal::COLORS::RESET << std::endl;
        stoprunning = true;
    }

    void warn(const std::string& message)
    {
        Gum::Filesystem::appendToFile(logFile, getCurrentTime() + "[W] " + message + "\n");
        std::cerr << Terminal::COLORS::GRAY << getCurrentTime() << Terminal::COLORS::YELLOW << "[W] " << message << Terminal::COLORS::RESET << std::endl;
    }
    void info(const std::string& message, bool newline, bool brackets)
    {
        std::string outStr = "";
        if(newline)
        {
            if(brackets)
            {
                outStr = Terminal::COLORS::GRAY + getCurrentTime() + Terminal::COLORS::LIGHT_GRAY + "[I] " + message + Terminal::COLORS::RESET + "\n";
            }
            else
            {
                outStr = Terminal::COLORS::LIGHT_GRAY + message + Terminal::COLORS::RESET + "\n";
            }
        }
        else
        {
            if(brackets)
            {
                outStr = Terminal::COLORS::GRAY + getCurrentTime() + Terminal::COLORS::LIGHT_GRAY + "[I] " + message + Terminal::COLORS::RESET;
            }
            else
            {
                outStr = Terminal::COLORS::LIGHT_GRAY + message + Terminal::COLORS::RESET;
            }
        }
        std::cout << outStr;
        Gum::Filesystem::appendToFile(logFile, outStr);
    }
    void debug(const std::string& message)
    {
        #ifdef DEBUG
            if(debuglogFile.getType() == Filesystem::Filetype::FILE)
              Gum::Filesystem::appendToFile(debuglogFile, "[D] " + message + "\n");
            std::cout << Terminal::COLORS::GRAY << getCurrentTime() << Terminal::COLORS::GREEN << "[D] " << message << Terminal::COLORS::RESET << std::endl;
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
		std::cout << Terminal::COLORS::RED << "/\\--------------------> [PROGRAM TERMINATED] <--------------------/\\\n" << Terminal::COLORS::RESET << std::endl;
	}

	bool wasSuccessful()
	{
		return issuccessful;
	}

  std::string getOutputSpacing()
  {
    return "              ";
  }
}}