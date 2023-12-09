#include "System.h"

#if(GUM_OS_UNIX)
#include <signal.h>
#include <dlfcn.h>

namespace Gum {
namespace System {

    std::function<void(int)> interruptCallback = nullptr;

    void interruptHandler(int signal)
    {
        if(interruptCallback != nullptr)
            interruptCallback(signal);
    }

    void init()
    {
        struct sigaction sigIntHandler;
        sigIntHandler.sa_handler = interruptHandler;
        sigemptyset(&sigIntHandler.sa_mask);
        sigIntHandler.sa_flags = 0;
        sigaction(SIGINT, &sigIntHandler, NULL);
    }

    void onInterruptSignal(std::function<void(int)> callback)
    {
        interruptCallback = callback;
    }   


    void* loadDynamicLibrary(std::string filepath)
    {
        return dlopen(filepath.c_str(), RTLD_LAZY | RTLD_LOCAL);
    }

    void freeDynamicLibrary(void* module)
    {
        dlclose(module);
    }

}};

#endif