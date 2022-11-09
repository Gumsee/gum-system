#pragma once
#include <string>
#include <functional>
#include <gum-maths.h>
#include <vector>

#define GUM_MOUSE_BUTTON_LEFT     					0x00
#define GUM_MOUSE_BUTTON_RIGHT    					0x01
#define GUM_MOUSE_BUTTON_MIDDLE   					0x03
#define GUM_MOUSE_BUTTON_PREVIOUS 					0x04
#define GUM_MOUSE_BUTTON_NEXT     					0x05

#define GUM_CURSOR_DEFAULT 							0x00
#define GUM_CURSOR_HORIZONTAL_RESIZE 				0x01
#define GUM_CURSOR_VERTICAL_RESIZE 					0x02
#define GUM_CURSOR_HAND 							0x03
#define GUM_CURSOR_CROSSHAIR 						0x04
#define GUM_CURSOR_IBEAM 							0x05
#define GUM_CURSOR_TOPLEFT_TO_BOTTOMRIGHT_RESIZE 	0x06
#define GUM_CURSOR_TOPRIGHT_TO_BOTTOMLEFT_RESIZE 	0x07
#define GUM_CURSOR_ALL_SIDES_RESIZE 				0x08
#define GUM_CURSOR_NOT_ALLOWED 						0x09


#if defined GUM_OS_LINUX || defined GUM_OS_BSD
	#include <X11/Xutil.h>
	#include <X11/Xlib.h>
#endif

#include "../Event.h"

namespace Gum {
class Window;

namespace IO
{
	class Mouse
	{
	private:
		ivec2 v2Position, v2PreviousPosition, v2LeftClickPosition;
		int iMouseWheelState;
		int frameSize;
		int CursorType;
		vec3 rayDir;
		int mouseOnID;
		Gum::Window* pContextWindow;
		
		#if defined GUM_OS_LINUX || defined GUM_OS_BSD
		static Cursor pDefaultCursor, pIBeamCursor, pCrosshairCursor, pHandCursor, pVResizeCursor, pHResizeCursor, pNWSEResizeCursor, pNESWResizeCursor, pAllResizeCursor, pNotAllowedCursor;
		Cursor pActiveCursor;
		Cursor pEmptyCursor;
		#endif

		float lastClickTimeLeft = 0.0f;
		float lastClickTimeRight = 0.0f;

		bool LeftDown = false;
		bool RightDown = false;
		bool LeftDownStart = false;
		bool RightDownStart = false;
		bool LeftClickOnce = false;
		bool RightClickOnce = false;
		bool LeftReleased = false;
		bool RightReleased = false;
		bool LeftDoubleClick = false;
		bool RightDoubleClick = false;

		bool defaulHideState;
		bool defaulTrapState;
		bool updateonclick;
		bool bIsTrapped;

		//Callbacks
    	void mouseButtonPressCallback(int button, int mods);
    	void mouseButtonReleaseCallback(int button, int mods);
    	void mouseScrollCallback(ivec2 dir);
    	void mouseMovedCallback(ivec2 pos);
    	void mouseEnteredCallback();
    	void mouseLeftCallback();
		std::function<void(int, int)> pressCallback = nullptr, releaseCallback = nullptr;
		std::function<void(ivec2)> scrollCallback = nullptr, movedCallback = nullptr;
		std::function<void()> enteredCallback = nullptr, leftCallback = nullptr;

		void initNativeMouse();
		void destroyNativeMouse();

	public:
        Mouse(Gum::Window* context);
        Mouse(Mouse& other) = delete;
        Mouse& operator=(Mouse& other) = delete;
		~Mouse();

		std::string DragAndDropInfo;

		void calcRay();		
		void reset();
		void handleEvent(Event event);

		//Events
		void onPress(std::function<void(int button, int mods)> callback);
		void onRelease(std::function<void(int button, int mods)> callback);
		void onScroll(std::function<void(ivec2 dir)> callback);
		void onMoved(std::function<void(ivec2 pos)> callback);
		void onEntered(std::function<void()> callback);
		void onLeft(std::function<void()> callback);

		//Setter
		void setContextWindow(Gum::Window* context);
		void setPosition(const ivec2& pos);
		void trap(const bool& doTrap);
		void hide(const bool& doHide);
		void updateOnClick(const bool& bln);
		void setInstanceIDUnderMouse(const unsigned int& id);
		void setCursor(uint8_t shape);

		//Getter
		vec3 getRayDirection() const;
		ivec2 getPosition() const;
		ivec2 getLeftClickPosition() const;
		float getDeltaDistanceNorm() const;
		int getMouseWheelState() const;
		int getCurrentPickedObjectID() const;
		int getCursorType() const;
		unsigned int getInstanceIDUnderMouse() const;
		bool hasLeftClick();
		bool hasRightClick();
		bool hasLeftClickStart();
		bool hasRightClickStart();
		bool hasLeftDoubleClick();
		bool hasRightDoubleClick();
		bool hasLeftRelease();
		bool hasRightRelease();
		bool hasMiddleClick();
		bool hasMiddleRelease();

		
		static void update();
        static void freeze(const bool& state);


		static ivec2 getScreenPosition();
		static ivec2 getDelta();
		static bool isBusy();

		static void setBusiness(const bool& val);
		static void setSnapPoint(const ivec2& snappoint);
		static void setGlobalPosition(const ivec2& pos);
	};
}}