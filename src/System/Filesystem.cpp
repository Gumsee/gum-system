#include "Filesystem.h"
#include <fstream>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include "Output.h"
#include <algorithm>

#if(GUM_OS_WINDOWS)
    #include <stdlib.h>
    #include <Windows.h>

#elif(GUM_OS_SOLARIS)
    #include <stdlib.h>
    #include <limits.h>

#elif(GUM_OS_LINUX)
    #include <unistd.h>
    #include <limits.h>
    #include <dirent.h>

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
            char exePath[MAX_PATH];
            GetModuleFileNameA(NULL, exePath, MAX_PATH);

            //PathRemoveFileSpec(exePath);
            //if (_get_pgmptr(&exePath) != 0)
            //    exePath = "";
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
        exePathStr = exePathStr.substr(0, exePathStr.find_last_of('/'));
        exePathStr = exePathStr.substr(0, exePathStr.find_last_of('\\'));
        return exePathStr;
    }

    void iterateThroughDirectory(std::string directory, std::function<void(File entry)> func)
    {
        #if (GUM_OS_WINDOWS)
            WIN32_FIND_DATA w32FindData;
            HANDLE hFind;
            hFind = FindFirstFile((directory + "/*.*").c_str(), &w32FindData);
            if(hFind == INVALID_HANDLE_VALUE)
            {
                Gum::Output::error("FindFirstFile failed: " + GetLastError());
                return;
            }
            
            do
            {
                std::string name = w32FindData.cFileName;
                if(name == ".." || name == ".") { continue; }

                File file;
                file.type = nativeTypeToFiletype(directory + "\\" + name, w32FindData.dwFileAttributes);
                file.path = directory;
                file.name = name;
                func(file);
            }
            while(FindNextFile(hFind, &w32FindData));

            FindClose(hFind);
        #elif (GUM_OS_LINUX)
            struct dirent *pDirent = nullptr;
            DIR *pDir = opendir(directory.c_str());

            if (pDir == nullptr)
                return;
                
            while ((pDirent = readdir(pDir)) != nullptr) 
            {
                unsigned char type = pDirent->d_type;
                std::string name = pDirent->d_name;
                if(name == ".." || name == ".") { continue; }
                
                File file;
                file.type = nativeTypeToFiletype(directory + "/" + name, type);
                file.path = directory;
                file.name = name;
                func(file);
            }

            closedir(pDir);
        #endif
    }

    FILETYPE nativeTypeToFiletype(std::string path, unsigned long nativeData)
    {
        #if (GUM_OS_WINDOWS)
            if     (nativeData & FILE_ATTRIBUTE_DIRECTORY)     { return FILETYPE::DIRECTORY; }
            else if(nativeData & FILE_ATTRIBUTE_DEVICE)        { return FILETYPE::CHARACTER_DEVICE; }
            else if(nativeData & FILE_ATTRIBUTE_REPARSE_POINT) { return FILETYPE::LINK; }
            else                                               { return FILETYPE::FILE; }
        #elif (GUM_OS_LINUX)
            unsigned char type = nativeData;
            if     (type == DT_DIR)  { return FILETYPE::DIRECTORY; }
            else if(type == DT_REG)  { return FILETYPE::FILE; }
            else if(type == DT_FIFO) { return FILETYPE::FIFO; }
            else if(type == DT_BLK)  { return FILETYPE::BLOCK_DEVICE; }
            else if(type == DT_CHR)  { return FILETYPE::CHARACTER_DEVICE; }
            else if(type == DT_LNK)  { return FILETYPE::LINK; }
            else if(type == DT_SOCK) { return FILETYPE::SOCKET; }
            else 
            {
                struct stat retstat;
                lstat((path).c_str(), &retstat);
                if     (S_ISREG(retstat.st_mode))  { return FILETYPE::FILE; }
                else if(S_ISDIR(retstat.st_mode))  { return FILETYPE::DIRECTORY; }
                else if(S_ISFIFO(retstat.st_mode)) { return FILETYPE::FIFO; }
                else if(S_ISLNK(retstat.st_mode))  { return FILETYPE::LINK; }
                else if(S_ISSOCK(retstat.st_mode)) { return FILETYPE::SOCKET; }
                else if(S_ISBLK(retstat.st_mode))  { return FILETYPE::BLOCK_DEVICE; }
                else if(S_ISCHR(retstat.st_mode))  { return FILETYPE::CHARACTER_DEVICE; }
            }
        #endif

        return FILETYPE::UNKNOWN;
    }

    std::string filetypeToString(FILETYPE type)
    {
        switch (type) 
        {
            case FILETYPE::DIRECTORY:        return "directory";
            case FILETYPE::FILE:             return "file";
            case FILETYPE::FIFO:             return "fifo";
            case FILETYPE::BLOCK_DEVICE:     return "block device";
            case FILETYPE::CHARACTER_DEVICE: return "character device";
            case FILETYPE::LINK:             return "link";
            case FILETYPE::SOCKET:           return "socket";
            default:                         return "unknown";
        };
    }
    
    std::string convertToNativePath(std::string path)
    {
        std::string retPath = path;
        #if (GUM_OS_WINDOWS)
            std::replace(retPath.begin(), retPath.end(), '/', '\\');
        #else
            std::replace(retPath.begin(), retPath.end(), '\\', '/');
        #endif

        return retPath;
    }
}}