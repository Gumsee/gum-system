#include "Filesystem.h"
#include <fstream>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include "File.h"
#include "Output.h"
#include <algorithm>
#if __cplusplus < 201703L // If the version of C++ is less than 17
#include <experimental/filesystem>
    // It was still in the experimental:: namespace
    namespace fs = std::experimental::filesystem;
#else
#include <filesystem>
    namespace fs = std::filesystem;
#endif


#if(GUM_OS_WINDOWS)
    #include <stdlib.h>
    #include <Windows.h>

#elif(GUM_OS_SOLARIS)
    #include <stdlib.h>
    #include <limits.h>

#elif(GUM_OS_LINUX || GUM_OS_ANDROID)
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
    void readFileContentsLineByLine(const File& file, const std::function<void(std::string line)>& func)
    {
        if(file.getType() != Filetype::FILE)
            return;

        std::ifstream filestream;
        filestream.open(file.toString(), std::ios::in);
        std::string line = "";
        while(std::getline(filestream, line))
            func(line);
        filestream.close();
    }

    void readFileContentsInChucksBin(const File& file, const unsigned int& chunksize, const std::function<bool(const char* data, const unsigned int& readbytes)>& func)
    {
        if(file.getType() != Filetype::FILE)
            return;

        std::ifstream filestream(file.toString(), std::ios::binary | std::ios::in);
        char buffer[chunksize];

        while(!filestream.eof()) 
        {
            filestream.read(&buffer[0], chunksize);
            std::streamsize s = filestream.gcount();
            if(!func(&buffer[0], s))
                break;
        }
        filestream.close();
    }

    std::string readFileContents(const File& file)
    {
        if(file.getType() != Filetype::FILE)
            return "";

        std::string contents = "";
        readFileContentsLineByLine(file.toString(), [&contents](std::string line) {
            contents.append(line + "\n");
        });

        return contents.substr(0, contents.length() - 1); //Get rid of that extra newline 
    }

    void writeToFile(const File& file, const std::string& str)
    {
        if(file.getType() != Filetype::FILE)
            return;

        std::ofstream filestream(file.toString(), std::ios::out);
        filestream << str;
        filestream.close();
    }

    void appendToFile(const File& file, const std::string& str)
    {
        if(file.getType() != Filetype::FILE)
            return;

        std::ofstream filestream(file.toString(), std::ios::app);
        filestream << str;
        filestream.close();
    }

    void appendToFileBin(const File& file, const char* data, const unsigned int& length)
    {
        if(file.getType() != Filetype::FILE)
            return;

        std::ofstream filestream(file.toString(), std::ios::app | std::ios::binary);
        filestream.write(&data[0], length);
        filestream.close();
    }

    File getExecutablePath()
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
        #elif (GUM_OS_LINUX || GUM_OS_ANDROID)
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
        std::string exePathStr = "/" + std::string(exePath);
        exePathStr = exePathStr.substr(0, exePathStr.find_last_of('/'));
        exePathStr = exePathStr.substr(0, exePathStr.find_last_of('\\'));
        return File(exePathStr, DIRECTORY);
    }

    void iterateThroughDirectory(const File& directory, const std::function<void(File entry)>& func)
    {
        #if (GUM_OS_WINDOWS)
            WIN32_FIND_DATA w32FindData;
            HANDLE hFind;
            hFind = FindFirstFile((directory.toString() + "/*.*").c_str(), &w32FindData);
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
                file.type = nativeTypeToFiletype(directory.toString() + "\\" + name, w32FindData.dwFileAttributes);
                file.path = directory.toString();
                file.name = name;
                func(file);
            }
            while(FindNextFile(hFind, &w32FindData));

            FindClose(hFind);
        #elif (GUM_OS_LINUX || GUM_OS_ANDROID)
            struct dirent *pDirent = nullptr;
            DIR *pDir = opendir(directory.toString().c_str());

            if (pDir == nullptr)
                return;
                
            while ((pDirent = readdir(pDir)) != nullptr) 
            {
                unsigned char type = pDirent->d_type;
                std::string name = pDirent->d_name;
                if(name == ".." || name == ".") { continue; }
                
                std::string path = directory.toString() + "/" + name;
                File file(path, nativeTypeToFiletype(path, type));
                func(file);
            }

            closedir(pDir);
        #endif
    }

    Filetype nativeTypeToFiletype(const std::string& path, const unsigned long& nativeData)
    {
        #if (GUM_OS_WINDOWS)
            if     (nativeData & FILE_ATTRIBUTE_DIRECTORY)     { return FILETYPE::DIRECTORY; }
            else if(nativeData & FILE_ATTRIBUTE_DEVICE)        { return FILETYPE::CHARACTER_DEVICE; }
            else if(nativeData & FILE_ATTRIBUTE_REPARSE_POINT) { return FILETYPE::LINK; }
            else                                               { return FILETYPE::FILE; }
        #elif (GUM_OS_LINUX || GUM_OS_ANDROID)
            unsigned char type = nativeData;
            if     (type == DT_DIR)  { return Filetype::DIRECTORY; }
            else if(type == DT_REG)  { return Filetype::FILE; }
            else if(type == DT_FIFO) { return Filetype::FIFO; }
            else if(type == DT_BLK)  { return Filetype::BLOCK_DEVICE; }
            else if(type == DT_CHR)  { return Filetype::CHARACTER_DEVICE; }
            else if(type == DT_LNK)  { return Filetype::LINK; }
            else if(type == DT_SOCK) { return Filetype::SOCKET; }
            else 
            {
                struct stat retstat;
                lstat((path).c_str(), &retstat);
                if     (S_ISREG(retstat.st_mode))  { return Filetype::FILE; }
                else if(S_ISDIR(retstat.st_mode))  { return Filetype::DIRECTORY; }
                else if(S_ISFIFO(retstat.st_mode)) { return Filetype::FIFO; }
                else if(S_ISLNK(retstat.st_mode))  { return Filetype::LINK; }
                else if(S_ISSOCK(retstat.st_mode)) { return Filetype::SOCKET; }
                else if(S_ISBLK(retstat.st_mode))  { return Filetype::BLOCK_DEVICE; }
                else if(S_ISCHR(retstat.st_mode))  { return Filetype::CHARACTER_DEVICE; }
            }
        #endif

        return Filetype::UNKNOWN;
    }

    std::string filetypeToString(const Filetype& type)
    {
        switch (type) 
        {
            case Filetype::DIRECTORY:        return "directory";
            case Filetype::FILE:             return "file";
            case Filetype::FIFO:             return "fifo";
            case Filetype::BLOCK_DEVICE:     return "block device";
            case Filetype::CHARACTER_DEVICE: return "character device";
            case Filetype::LINK:             return "link";
            case Filetype::SOCKET:           return "socket";
            default:                         return "unknown";
        };
    }
    
    std::string convertToNativePath(const std::string& path)
    {
        std::string retPath = path;
        #if (GUM_OS_WINDOWS)
            std::replace(retPath.begin(), retPath.end(), '/', '\\');
        #else
            std::replace(retPath.begin(), retPath.end(), '\\', '/');
        #endif

        return retPath;
    }

    bool fileExists(const File& file)
    {
        if(file.getType() != Filetype::FILE)
            return false;

        std::ifstream infile(file.toString()); //Slow
        return infile.good();
    }

    bool dirExists(const File& dir)
    {
        if(dir.getType() != Filetype::DIRECTORY)
            return false;

        struct stat info;

        if(stat(dir.toString().c_str(), &info ) != 0)
            return false;
        else if(info.st_mode & S_IFDIR)
            return true;

        return false;
    }

    bool createDirectory(const File& dir)
    {
        if(dir.getType() != Filetype::DIRECTORY)
        {
            Gum::Output::error("Cannot create directory: " + dir.toString() + " is not of type directory");
            return false;
        }

        return fs::create_directory(dir.toString());
    }

    bool execFile(const File& file, std::vector<std::string> args, const std::function<void(std::string)>& callback)
    {
        if(file.getType() != Filetype::FILE)
        {
            Gum::Output::error("Cannot execute file " + file.toString() + ", is not a file!");
            return false;
        }

        std::string argstr = file.toString();
        for(int i = 0; i < args.size(); i++)
            argstr += " \""+args[i]+"\"";

        std::array<char, 128> buffer;
        std::string stdoutstr;
        std::unique_ptr<::FILE, decltype(&pclose)> pipe(popen(argstr.c_str(), "r"), pclose);

        if(!pipe)
        {
            Gum::Output::error("Failed to execute file: " + file.toString() + "");
            return false;
        }

        while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr)
            stdoutstr += buffer.data();


        callback(stdoutstr);
        return true;
    }
}}