#include "Terminal.h"

namespace Gum { namespace Terminal
{
  namespace COLORS
  {
    #if defined GUM_OS_UNIX
    std::string RESET        = "\033[0m";
    std::string BLACK        = "\033[0;30m";
    std::string GRAY         = "\033[1;30m";
    std::string RED          = "\033[0;31m";
    std::string LIGHT_RED    = "\033[1;31m";
    std::string GREEN        = "\033[0;32m";
    std::string LIGHT_GREEN  = "\033[1;32m";
    std::string BROWN        = "\033[0;33m";
    std::string YELLOW       = "\033[1;33m";
    std::string BLUE         = "\033[0;34m";
    std::string LIGHT_BLUE   = "\033[1;34m";
    std::string PURPLE       = "\033[0;35m";
    std::string LIGHT_PURPLE = "\033[1;35m";
    std::string CYAN         = "\033[0;36m";
    std::string LIGHT_CYAN   = "\033[1;36m";
    std::string LIGHT_GRAY   = "\033[0;37m";
    std::string WHITE        = "\033[1;37m";
    #else
    std::string RESET        = "";
    std::string BLACK        = "";
    std::string GRAY         = "";
    std::string RED          = "";
    std::string LIGHT_RED    = "";
    std::string GREEN        = "";
    std::string LIGHT_GREEN  = "";
    std::string BROWN        = "";
    std::string YELLOW       = "";
    std::string BLUE         = "";
    std::string LIGHT_BLUE   = "";
    std::string PURPLE       = "";
    std::string LIGHT_PURPLE = "";
    std::string CYAN         = "";
    std::string LIGHT_CYAN   = "";
    std::string LIGHT_GRAY   = "";
    std::string WHITE        = "";
    #endif
  }

}}