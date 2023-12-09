#pragma once
#include <functional>
#include <string>
#include "File.h"

namespace Gum {
namespace Filesystem {
    extern void readFileContents(const File& file, const std::function<void(std::string line)>& func);
    extern std::string readFileContents(const File& file);

    extern void writeToFile(const File& file, const std::string& str);
    extern void appendToFile(const File& file, const std::string& str);
    
    extern void iterateThroughDirectory(const File& directory, const std::function<void(File entry)>& func);
    extern Filetype nativeTypeToFiletype(const std::string& path, const unsigned long& nativeData = 0);
    extern std::string filetypeToString(const Filetype& type);

    extern File getExecutablePath();

    extern std::string convertToNativePath(const std::string& path);
}}