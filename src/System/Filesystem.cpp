#include "Filesystem.h"
#include <fstream>
#include <dirent.h>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> 
#include "Output.h"

#if(GUM_OS_WINDOWS)
    #include <stdlib.h>

#elif(GUM_OS_SOLARIS)
    #include <stdlib.h>
    #include <limits.h>

#elif(GUM_OS_LINUX)
    #include <unistd.h>
    #include <limits.h>

#elif(GUM_OS_MACOS)
    #include <mach-o/dyld.h>

#elif (GUM_OS_BSD)
    #include <sys/types.h>
    #include <sys/sysctl.h>
#endif


namespace Gum {
namespace Filesystem {
    void readFileContents(std::string filepath, std::function<void(std::string line)> func)
    {
        std::ifstream filestream;
        filestream.open(filepath, std::ios::in);
        std::string line = "";
        while(std::getline(filestream, line))
            func(line);
        filestream.close();
    }

    std::string readFileContents(std::string filepath)
    {
        std::string contents = "";
        readFileContents(filepath, [&contents](std::string line) {
            contents.append(line + "\n");
        });

        return contents.substr(0, contents.length() - 1); //Get rid of that extra newline 
    }

    void writeToFile(std::string filepath, std::string str)
    {
        std::ofstream filestream;
        filestream.open(filepath, std::ios::out);
        filestream << str;
        filestream.close();
    }

    void appendToFile(std::string filepath, std::string str)
    {
        std::ofstream filestream;
        filestream.open(filepath, std::ios::app);
        filestream << str;
        filestream.close();
    }

    std::string getExecutablePath()
    {
        #if (GUM_OS_WINDOWS)
            char *exePath;
            if (_get_pgmptr(&exePath) != 0)
                exePath = "";
        #elif (GUM_OS_SOLARIS)
            char exePath[PATH_MAX];
            if (realpath(getexecname(), exePath) == NULL)
                exePath[0] = '\0';
        #elif (GUM_OS_LINUX)
            char exePath[PATH_MAX];
            ssize_t len = readlink("/proc/self/exe", exePath, sizeof(exePath));
            if (len == -1 || len == sizeof(exePath))
                len = 0;
            exePath[len] = '\0';
        #elif (GUM_OS_MACOS)
            char exePath[PATH_MAX];
            uint32_t len = sizeof(exePath);
            if (_NSGetExecutablePath(exePath, &len) != 0) {
                exePath[0] = '\0'; // buffer too small (!)
            } else {
                // resolve symlinks, ., .. if possible
                char *canonicalPath = realpath(exePath, NULL);
                if (canonicalPath != NULL) {
                    strncpy(exePath,canonicalPath,len);
                    free(canonicalPath);
                }
            }
        #elif (GUM_OS_BSD)
            char exePath[2048];
            int mib[4];  mib[0] = CTL_KERN;  mib[1] = KERN_PROC;  mib[2] = KERN_PROC_PATHNAME;  mib[3] = -1;
            size_t len = sizeof(exePath);
            if (sysctl(mib, 4, exePath, &len, NULL, 0) != 0)
                exePath[0] = '\0';
        #endif
        std::string exePathStr = std::string(exePath);
        return exePathStr.substr(0, exePathStr.find_last_of('/'));
    }

    void iterateThroughDirectory(std::string directoy, std::function<void(File entry)> func)
    {
        struct dirent *pDirent = nullptr;
        DIR *pDir = opendir(directoy.c_str());

        if (pDir == nullptr)
            return;
            
        while ((pDirent = readdir(pDir)) != nullptr) 
        {
            unsigned char type = pDirent->d_type;
            std::string name = pDirent->d_name;
            if(name == ".." || name == ".") { continue; }
            
            File file;
            if     (type == DT_DIR)  { file.type = FILETYPE::DIRECTORY; }
            else if(type == DT_REG)  { file.type = FILETYPE::FILE; }
            else if(type == DT_FIFO) { file.type = FILETYPE::FIFO; }
            else if(type == DT_BLK)  { file.type = FILETYPE::BLOCK_DEVICE; }
            else if(type == DT_CHR)  { file.type = FILETYPE::CHARACTER_DEVICE; }
            else if(type == DT_LNK)  { file.type = FILETYPE::LINK; }
            else if(type == DT_SOCK) { file.type = FILETYPE::SOCKET; }
            else 
            {
                struct stat retstat;
                lstat((directoy + name).c_str(), &retstat);
                if     (S_ISREG(retstat.st_mode))  { file.type = FILETYPE::FILE; }
                else if(S_ISDIR(retstat.st_mode))  { file.type = FILETYPE::DIRECTORY; }
                else if(S_ISFIFO(retstat.st_mode)) { file.type = FILETYPE::FIFO; }
                else if(S_ISLNK(retstat.st_mode))  { file.type = FILETYPE::LINK; }
                else if(S_ISSOCK(retstat.st_mode)) { file.type = FILETYPE::SOCKET; }
                else if(S_ISBLK(retstat.st_mode))  { file.type = FILETYPE::BLOCK_DEVICE; }
                else if(S_ISCHR(retstat.st_mode))  { file.type = FILETYPE::CHARACTER_DEVICE; }
            }
            file.path = directoy;
            file.name = name;
            func(file);
        }

        closedir(pDir);
    }
}}