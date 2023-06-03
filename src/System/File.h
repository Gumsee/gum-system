#pragma once
#include "Output.h"
#include <vector>
#include <string>

namespace Gum {
namespace Filesystem 
{

enum Filetype
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

class File : private std::vector<std::string>
{
private:
    inline static const std::string problematicChars = "\"<>:\\/|?*";
    Filetype iType;
    bool bIsAbsolute;

public:
    File();
    File(const std::string& str, const Filetype& type = FILE);

    std::string toString();
    File* up();
    File* add(const File& path);

    //Getter
    std::string getName();
    Filetype getType();
    std::string& operator[](size_t i);
    bool isAbsolute();
};

}}