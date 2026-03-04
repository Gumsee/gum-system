#include "File.h"
#include "Filesystem.h"
#include "Output.h"
#include <cassert>

namespace Gum {
namespace Filesystem 
{
File::File()
{
}

File::File(const std::string& str, const Filetype& type)
{
    //assert(!str.empty());
    if(str.empty())
    {
        Gum::Output::error("File: Filepath empty!");
    }
    this->iType = type;
    this->bIsAbsolute = str[0] == '/' || str[0] == '\\';

    for(char c : problematicChars)
    {
        if(str.find(c) != str.npos)
            Gum::Output::warn("Path string contains characters that may cause Problems (" + problematicChars + "): " + str);
    }

    std::string word = "";
    for(char c : str)
    {
        if((c == '/' || c == '\\'))
        {
            if(word != "")
            {
                push_back(word);
                word = "";
            }
        }
        else if(c == '\n')
        {
            
        }
        else
        {
            word += c;
        }
    }
    if(word != "")
        push_back(word);
}

File* File::up()
{
    if(size() > 0)
        erase(end());
    iType = DIRECTORY;
    return this;
}

File* File::add(const File& path)
{
    if(iType != DIRECTORY)
    {
        Gum::Output::warn("Cannot add path to non-directory file: " + toString());
        return this;
    }

    insert(end(), path.begin(), path.end());
    iType = path.iType;
    return this;
}

std::string File::getName() const { return this->at(size() - 1); }
Filetype File::getType() const    { return this->iType; }
bool File::isAbsolute() const     { return this->bIsAbsolute; }
bool File::isEmpty() const        { return this->empty(); }
std::string File::toString() const
{
    std::string pathstr = bIsAbsolute ? "/" : "./";
    for(std::string part : *this)
        pathstr += part + "/";
    if(iType != DIRECTORY && pathstr.length() > 0)
        pathstr.erase(pathstr.length() - 1);
    return pathstr;
}

File File::operator+(const std::string& other) { return this->operator+(File(other)); }
File File::operator+(const File& other)
{
    return *File(*this).add(other);
}

void File::operator+=(const File& other)
{
    this->add(other);
}

}}