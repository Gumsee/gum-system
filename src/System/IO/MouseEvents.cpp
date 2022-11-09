#include "Mouse.h"
#include "../Output.h"
#include "../Window.h"
#include "../Display.h"

namespace Gum {
namespace IO
{    
    void Mouse::mouseButtonPressCallback(int button, int mods)
    {
        if(button == GUM_MOUSE_BUTTON_LEFT)
        {
            LeftClickOnce = true;
            LeftDownStart = true;
            LeftDown = true;
            v2LeftClickPosition = getPosition();
        }

        if(button == GUM_MOUSE_BUTTON_RIGHT)
        {
            RightClickOnce = true;
            RightDownStart = true;
            RightDown = true;
        }

        if(pressCallback != nullptr)
            pressCallback(button, mods);
    }

    void Mouse::mouseButtonReleaseCallback(int button, int mods)
    {
        if(button == GUM_MOUSE_BUTTON_LEFT)
        {
            if(LeftClickOnce)
            { 
                if(lastClickTimeLeft < 0.2f)
                {
                    LeftDoubleClick = true;
                    lastClickTimeLeft = 0.2f;
                }
                else
                    lastClickTimeLeft = 0;

                LeftClickOnce = false;
            }
            LeftReleased = true; 
            LeftDown = false;
        }

        if(button == GUM_MOUSE_BUTTON_RIGHT)
        {
            if(RightClickOnce)
            {
                if(lastClickTimeRight < 0.2f)
                {
                    RightDoubleClick = true;
                    lastClickTimeRight = 0.2f;
                }
                else
                    lastClickTimeRight = 0;
            }
            RightReleased = true; 
            RightDown = false;
        }

        if(releaseCallback != nullptr)
            releaseCallback(button, mods);
    }
    
    void Mouse::mouseScrollCallback(ivec2 dir)
    {
        iMouseWheelState = dir.y;

        if(scrollCallback != nullptr)
            scrollCallback(dir);
    }
    
    void Mouse::mouseMovedCallback(ivec2 pos)
    {
        v2PreviousPosition = v2Position;
        v2Position = pos;

        //v2PositionDelta = v2Position - v2PreviousPosition;
        calcRay();

        if(movedCallback != nullptr)
            movedCallback(pos);
    }
    
    void Mouse::mouseEnteredCallback()
    {
        if(enteredCallback != nullptr)
            enteredCallback();
    }

    void Mouse::mouseLeftCallback()
    {
        if(leftCallback != nullptr)
            leftCallback();
    }


    void Mouse::onPress(std::function<void(int button, int mods)> callback)    { pressCallback = callback; }
    void Mouse::onRelease(std::function<void(int button, int mods)> callback)  { releaseCallback = callback; }
    void Mouse::onScroll(std::function<void(ivec2 dir)> callback)              { scrollCallback = callback; }
    void Mouse::onMoved(std::function<void(ivec2 pos)> callback)               { movedCallback = callback; }
    void Mouse::onEntered(std::function<void()> callback)                      { enteredCallback = callback; }
    void Mouse::onLeft(std::function<void()> callback)                         { leftCallback = callback; }
}}