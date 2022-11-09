#include "Mouse.h"
#include "../Output.h"
#include "../Window.h"
#include "../Display.h"

namespace Gum {
namespace IO
{
    bool bIsBusy = false, bIsSnapped = false;
    ivec2 v2PositionDelta, v2ScreenPosition, v2PreviousScreenPosition, v2SnapPoint;

    Mouse::Mouse(Gum::Window* context)
    {
        this->v2Position = ivec2(0,0);
        this->v2PreviousPosition = ivec2(0,0);
        this->pContextWindow = context;
        this->v2LeftClickPosition = ivec2(-1, -1);

        updateonclick = false;
        bIsTrapped = false;

        iMouseWheelState = 0;
        CursorType = 0;
        frameSize = 0;
        rayDir = vec3(0);
        DragAndDropInfo = "";
        mouseOnID = -1;
        
        //glfwSetInputMode(pContextWindow->getRenderWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        
        /*if (glfwRawMouseMotionSupported())
            glfwSetInputMode(pContextWindow->getRenderWindow(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);*/
    }



	float Mouse::getDeltaDistanceNorm() const
	{		
		vec2 normPos = (v2Position - pContextWindow->getPosition()) / pContextWindow->getSize();
		vec2 normOldPos = (v2PreviousPosition - pContextWindow->getPosition()) / pContextWindow->getSize();

		return vec2::distance(normPos, normOldPos); 
	}


	void Mouse::calcRay()
	{
		/*float frameX = (float)renderFrame.x;
        float frameY = (float)renderFrame.y;
        vec4 screen_space;

        //screen space to normalized device space
		screen_space.x = (2 * (getPosition().x / frameX)) - 1;
		screen_space.y = -((2 * (getPosition().y / frameY)) - 1);
        screen_space.z = -1.0f;
		screen_space.z = 1.0f;

		mat4 invertedProjection = mat4::inverse(GumEngine::ActiveCamera->getProjectionMatrix());
		vec4 eye_space = mat4::transpose(invertedProjection) * screen_space;
		eye_space.z = -1.0f;
		eye_space.w = 0.0f;

        //eye space to world space
		vec4 worldspace = GumEngine::ActiveCamera->getViewMatrix() * eye_space;

		rayDir = worldspace;
        rayDir = vec3::normalize(rayDir);*/
	}

    void Mouse::reset()
    {
        //Replace with chrono
        lastClickTimeLeft += 0.01; //FPS::get();
        lastClickTimeRight += 0.01; //FPS::get();
        
        if(LeftReleased)
            v2LeftClickPosition = ivec2(-1, -1);
		LeftReleased = false;
		RightReleased = false;
        LeftDoubleClick = false;
        RightDoubleClick = false;
        LeftDownStart = false;
        RightDownStart = false;

		iMouseWheelState = 0;
        CursorType = 0;
        frameSize = 0;

        setCursor(GUM_CURSOR_DEFAULT);


        if(bIsTrapped) { setPosition(pContextWindow->getSize() / 2); }
    }

    //Setter
    void Mouse::setContextWindow(Gum::Window* context)              { this->pContextWindow = context; }
	void Mouse::updateOnClick(const bool& bln)    	                { this->updateonclick = bln; }
	void Mouse::trap(const bool& doTrap) 			                { this->bIsTrapped = doTrap; }
    void Mouse::setInstanceIDUnderMouse(const unsigned int& id)     { this->mouseOnID = id; }


    //Getter
	vec3 Mouse::getRayDirection() const 			                { return this->rayDir; }
	ivec2 Mouse::getPosition() const 				                { return this->v2Position; }
    ivec2 Mouse::getLeftClickPosition() const                       { return this->v2LeftClickPosition; }
	int Mouse::getCurrentPickedObjectID() const 		            { return this->mouseOnID; }
	int Mouse::getMouseWheelState() const 			                { return this->iMouseWheelState; }
    int Mouse::getCursorType() const                                { return this->CursorType; }
    unsigned int Mouse::getInstanceIDUnderMouse() const             { return this->mouseOnID; }
    
    bool Mouse::hasLeftClick()                                      { return this->LeftDown; }
    bool Mouse::hasRightClick()                                     { return this->RightDown; }
    bool Mouse::hasLeftClickStart()                                 { return this->LeftDownStart; }
    bool Mouse::hasRightClickStart()                                { return this->RightDownStart; }
    bool Mouse::hasLeftDoubleClick()                                { return this->LeftDoubleClick; }
    bool Mouse::hasRightDoubleClick()                               { return this->RightDoubleClick; }
    bool Mouse::hasLeftRelease()                                    { return this->LeftReleased; }
    bool Mouse::hasRightRelease()                                   { return this->RightReleased; }
    //bool Mouse::hasMiddleClick()      { return glfwGetMouseButton(pContextWindow->getRenderWindow(), GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS; }
    //bool Mouse::hasMiddleRelease()    { return glfwGetMouseButton(pContextWindow->getRenderWindow(), GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_RELEASE; }
    

    void Mouse::update()
    {
        /*int x, y, winx, winy;
        unsigned int mask = 0;
        unsigned long childWin, rootWin;
        XQueryPointer(Display::getSystemHandle(), XRootWindow(Display::getSystemHandle(), XDefaultScreen(Display::getSystemHandle())), &childWin, &rootWin, &x, &y, &winx, &winy, &mask);*/
        
        v2ScreenPosition = getScreenPosition();
        v2PositionDelta = v2ScreenPosition - v2PreviousScreenPosition;
        v2PreviousScreenPosition = v2ScreenPosition;

        if(bIsSnapped) 
        {
            setGlobalPosition(v2SnapPoint);
            v2PreviousScreenPosition = v2SnapPoint;
        }
    }

    void Mouse::freeze(const bool& state)
    {
        if(bIsSnapped != state)
        {
            v2SnapPoint = v2ScreenPosition;
            bIsSnapped = state;
        }
    }

    void Mouse::handleEvent(Event event)
    {
        switch(event.type)
        {
            case GUM_EVENT_MOUSE_PRESSED:  mouseButtonPressCallback(event.data.mousebutton, 0); break;
            case GUM_EVENT_MOUSE_RELEASED: mouseButtonReleaseCallback(event.data.mousebutton, 0); break;
            case GUM_EVENT_MOUSE_SCROLL:   mouseScrollCallback(event.data.mousescroll); break;
            case GUM_EVENT_MOUSE_MOVED:    mouseMovedCallback(event.data.mousepos); break;
            case GUM_EVENT_MOUSE_ENTERED:  mouseEnteredCallback(); break;
            case GUM_EVENT_MOUSE_LEFT:     mouseLeftCallback(); break;
        }
    }

    ivec2 Mouse::getScreenPosition() 	              { return v2ScreenPosition; }
    ivec2 Mouse::getDelta() 			              { return v2PositionDelta; }
    bool Mouse::isBusy()                             { return bIsBusy; }

    void Mouse::setBusiness(const bool& val)         { bIsBusy = val; }
    void Mouse::setSnapPoint(const ivec2& snappoint) { v2SnapPoint = snappoint; }
}}