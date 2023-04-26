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

    extern void readFileContents(const std::string& filepath, const std::function<void(std::string line)>& func);
    extern std::string readFileContents(const std::string& filepath);

    extern void writeToFile(const std::string& filepath, const std::string& str);
    extern void appendToFile(const std::string& filepath, const std::string& str);
    
    extern void iterateThroughDirectory(const std::string& directory, const std::function<void(File entry)>& func);
    extern FILETYPE nativeTypeToFiletype(const std::string& path, const unsigned long& nativeData = 0);
    extern std::string filetypeToString(const FILETYPE& type);

    extern std::string getExecutablePath();

    extern std::string convertToNativePath(const std::string& path);
}}