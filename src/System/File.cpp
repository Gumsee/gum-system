#include "File.h"
#include "Output.h"

namespace Gum {
namespace Filesystem 
{
File::File()
{
}

File::File(const std::string& str, const Filetype& type)
{
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
        else
        {
            word += c;
        }
    }
    if(word != "")
        push_back(word);
}

std::string File::toString()
{
    std::string pathstr = bIsAbsolute ? "/" : "";
    for(std::string part : *this)
        pathstr += part + "/";
    if(iType != DIRECTORY && pathstr.length() > 0)
        pathstr.erase(pathstr.length() - 1);
    return pathstr;
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

std::string File::getName()
{
    return at(size() - 1);
}

Filetype File::getType()
{
    return iType;
}

std::string& File::operator[](size_t i)
{
    return at(i);
}

bool File::isAbsolute()
{
    return bIsAbsolute;
}

}}