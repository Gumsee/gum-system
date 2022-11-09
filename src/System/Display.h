#pragma once
#include "Monitor.h"
#include <gum-maths.h>

namespace Gum {
namespace Display
{
    extern void init();
    extern void initNativeDisplay();
    extern void pollNativeEvents();
    extern void destroyNativeDisplay();
    extern void cleanup();
    extern void pollEvents();
    extern void addMonitors(Monitor* monitor);
    extern void printInfo();

    extern void setPrimaryMonitor(Monitor* monitor);

    extern void* getSystemHandle();
    extern std::vector<Monitor*> getMonitors();
    extern Monitor* getPrimaryMonitor();
}}