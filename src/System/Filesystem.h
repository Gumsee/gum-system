#pragma once
#include <functional>
#include <string>
#include "File.h"

namespace Gum {
namespace Filesystem {
    extern void readFileContentsLineByLine(const File& file, const std::function<void(std::string line)>& func);
    extern void readFileContentsInChucksBin(const File& file, const size_t& chunksize, const std::function<bool(const char* chunk, const size_t& readbytes)>& func);
    extern std::string readFileContents(const File& file);

    extern void writeToFile(const File& file, const std::string& str);
    extern void appendToFile(const File& file, const std::string& str);
    extern void appendToFileBin(const File& file, const char* data, const size_t& length);
    
    extern void iterateThroughDirectory(const File& directory, const std::function<void(File entry)>& func);
    extern Filetype nativeTypeToFiletype(const std::string& path, const unsigned long& nativeData = 0);
    extern std::string filetypeToString(const Filetype& type);

    extern File getExecutablePath();

    extern std::string convertToNativePath(const std::string& path);

    extern bool fileExists(const File& file);
    extern bool dirExists(const File& file);

    extern bool createDirectory(const File& dir);

    extern bool execFile(const File& file, std::vector<std::string> args, const std::function<void(std::string)>& callback);
}}