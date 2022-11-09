#pragma once
#if defined GUM_OS_LINUX || defined GUM_OS_BSD
#include <X11/Xlib.h>
#endif

#include <Maths/vec.h>
#include <list>

#define GUM_EVENT_NONE              0x000
#define GUM_EVENT_FOCUS_IN          0x001
#define GUM_EVENT_FOCUS_OUT         0x002
#define GUM_EVENT_FOCUS_OUT         0x002
#define GUM_EVENT_WINDOW_REPOSITION 0x003
#define GUM_EVENT_WINDOW_RESIZE     0x004
#define GUM_EVENT_WINDOW_CLOSE      0x005
#define GUM_EVENT_MOUSE_PRESSED     0x006
#define GUM_EVENT_MOUSE_RELEASED    0x007
#define GUM_EVENT_MOUSE_MOVED       0x008
#define GUM_EVENT_MOUSE_SCROLL      0x009
#define GUM_EVENT_MOUSE_ENTERED     0x00A
#define GUM_EVENT_MOUSE_LEFT        0x00B
#define GUM_EVENT_KEYBOARD_PRESSED  0x00C
#define GUM_EVENT_KEYBOARD_RELEASED 0x00D

namespace Gum {
struct Event
{
    union EventData
    {
        ivec2 windowpos, windowsize;
        ivec2 mousepos, mousescroll;
        int mousebutton;
        unsigned int keyboardkey;

        EventData()
        {

        }
    };

    int nativeEvent = 0;
    int type = GUM_EVENT_NONE;
    EventData data;

    void initNative();

    #if defined GUM_OS_LINUX || defined GUM_OS_BSD
        static std::list<Event> createFromNative(XEvent xevent);
    #endif

    Event()
    {
        initNative();
    }
};
}