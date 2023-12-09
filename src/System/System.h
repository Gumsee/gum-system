#pragma once
#include <csignal>
#include <functional>
#include <string>

namespace Gum {
namespace System {

    extern void init();

    extern void onInterruptSignal(std::function<void(int)> callback);
    extern void* loadDynamicLibrary(std::string filepath);
    extern void freeDynamicLibrary(void* module);

}};