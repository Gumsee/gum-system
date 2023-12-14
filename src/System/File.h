#pragma once
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

    File* up();
    File* add(const File& path);

    using std::vector<std::string>::operator[];

    //Getter
    std::string getName() const;
    Filetype getType() const;
    bool isAbsolute() const;
    std::string toString() const;

    File operator+(const File& other);
    void operator+=(const File& other);
};

}}