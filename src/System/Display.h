#pragma once
#include "Monitor.h"
#include <gum-maths.h>

#if defined GUM_OS_LINUX || defined GUM_OS_BSD
	#include <X11/Xutil.h>
	#include <X11/Xlib.h>
#endif

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

    #if defined GUM_OS_LINUX || defined GUM_OS_BSD
        extern ::Display* getSystemHandle();
    #endif
    extern std::vector<Monitor*> getMonitors();
    extern Monitor* getPrimaryMonitor();
}}