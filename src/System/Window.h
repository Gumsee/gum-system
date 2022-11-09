#pragma once
#include <gum-maths.h>
#include <functional>
#include <vector>
#include "FramebufferConfig.h"
#include "IO/Mouse.h"
#include "IO/Keyboard.h"

#include "GLContext.h"
#include "Monitor.h"
#include "Event.h"


#define GUM_WINDOW_DEFAULTS        0b000000000
#define GUM_WINDOW_RESIZABLE       0b000000001
#define GUM_WINDOW_FULLSCREEN      0b000000010
#define GUM_WINDOW_FLOATING        0b000000100
#define GUM_WINDOW_BORDERLESS      0b000001000
#define GUM_WINDOW_SIZE_IN_PERCENT 0b000010000
#define GUM_WINDOW_VERTICAL_SYNC   0b000100000
#define GUM_WINDOW_MAXIMIZED       0b001000000
#define GUM_WINDOW_SHARECONTEXT    0b010000000
#define GUM_WINDOW_TRAP_IN_PARENT  0b100000000

struct IconImageData
{
    uivec2 res;
    short numComps = 4;
    unsigned char* data;

	IconImageData(uivec2 res, short numcomps, unsigned char* data) : res(res), numComps(numcomps), data(data) {}
};

namespace Gum
{
	class Window
	{
	private:
    	void* pNativeWindowHandle;

		Gum::Window *pParentWindow;
		ivec2 v2VisibleAreaSize;
		ivec2 v2Size, v2Pos;
		vec2 v2PixelSize;
		mat4 m4ScreenMatrix;
		float fAspectRatio, fAspectRatioWidthToHeight;
		bool bHasBorder, bHasVerticalSync, bHasScalingSnapped, bIsFloating, bIsMaximized, bIsMinimized, bIsResizable, bIsFullscreen, bIsHidden, bShouldClose, bKeepInsideParent;
		std::string sTitle;
		std::string sWindowClass = "Gumball";
		GLContext* pContext;
		Monitor* pMonitor;

		static ivec4 iv4DecorationSize;

		IO::Keyboard* pKeyboard;
		IO::Mouse* pMouse;

		//Native
		void initNativeWindow();
		void destroyNativeWindow();
		

		//Callbacks
    	void windowResizedCallback(ivec2 size);
    	void windowMovedCallback(ivec2 pos);
    	void windowFocusedCallback(bool hasfocus);
		std::function<void(ivec2)> movedCallback = nullptr, resizedCallback = nullptr;
		std::function<void(bool)> focusedCallback = nullptr;

	public:
		Window(std::string title, ivec2 windowsize, int properties, Window* parentWindow = nullptr, Monitor* monitor = nullptr);
		~Window();

		static Window* MainWindow;
		static Window* CurrentlyBoundWindow;
		static bool WINDOW_IS_ACTIVE_SCALING;
		static std::vector<Window*> AllWindows;

		void resetViewport();
		void update();
		void close();
		void clear(int clearbits = GL_COLOR_BUFFER_BIT);
		void bind();
		static void unbind();



		//Native
		void finishRender();
		void showBorder(bool show);
		void makeResizable(bool isresizable);
		void makeFullscreen(bool fullscreen);
		void makeFloating(bool isfloating);
		void minimize(bool dominimize = true);
		void maximize(bool domaximize = true);
		void restore();
		void handleEvent(Event event);
		void hide(bool hiddenstat);
		void focus();
		void setSize(const ivec2& size);
		void setPosition(const ivec2& pos);
		void setVerticalSync(bool vsync);
		void setTitle(const std::string& title);
		void setParent(Window* parent, bool trap = false);
		void setIcon(std::vector<IconImageData> images, bool isgrayscale = false, vec4 color = vec4(1, 1, 1, 1));
		void removeIcon();


		//Events
		void onResized(const std::function<void(ivec2 size)>& callback);
		void onMoved(const std::function<void(ivec2 pos)>& callback);
		void onFocused(const std::function<void(bool hasfocus)>& callback);



		//Setter
		void setClearColor(vec4 color);
		void setResizable(const bool& resizable);

		//Getter
		void* getNativeWindow() const;
		ivec2 getSize() const;
		vec2 getPixelSize() const;
		ivec2 getPosition() const;
		mat4 getScreenMatrix() const;
		std::string getTitle() const;
		IO::Keyboard* getKeyboard();
		IO::Mouse* getMouse();
		float getAspectRatio() const;
		float getAspectRatioWidthToHeight() const;
		Gum::Window* getParentWindow();
		GLContext* getContext();
		bool isFullscreen() const;
		bool isOpen() const;
		bool isMaximized() const;
		bool isResizable() const;
	};
}
