#pragma once
#include <functional>
#include <string>
#include <vector>

namespace Gum {
namespace Filesystem {

    enum FILETYPE
    {
        FILE,
        LINK,
        DIRECTORY,
        CHARACTER_DEVICE,
        BLOCK_DEVICE,
        FIFO,
        SOCKET,
        UNKNOWN,
    };

    struct File
    {
        std::string path;
        std::string name;
        FILETYPE type;
    };

    extern void readFileContents(std::string filepath, std::function<void(std::string line)> func);
    extern std::string readFileContents(std::string filepath);

    extern void writeToFile(std::string filepath, std::string str);
    extern void appendToFile(std::string filepath, std::string str);
    
    extern void iterateThroughDirectory(std::string directoy, std::function<void(File entry)> func);

    extern std::string getExecutablePath();
}}