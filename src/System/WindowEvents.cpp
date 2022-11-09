#include "Window.h"
#include "GLContext.h"
#include "IO/Keyboard.h"
#include "Display.h"
#include "Output.h"
#include "MemoryManagement.h"

namespace Gum
{	
	void Window::windowResizedCallback(ivec2 size)
	{
		v2Size = size;
		fAspectRatio = (float)v2Size.y / (float)v2Size.x;
		fAspectRatioWidthToHeight = (float)v2Size.x / (float)v2Size.y;
		m4ScreenMatrix = Gum::Maths::ortho((float)v2Size.y, (float)v2Size.x, 0.0f, 0.0f, -100.0f, 100.0f);
		v2PixelSize = vec2(1.0f) / (vec2)v2Size;

		resetViewport();

		if(resizedCallback != nullptr)
			resizedCallback(size);
	}

	void Window::windowMovedCallback(ivec2 pos)
	{
		v2Pos = pos;
		if(movedCallback != nullptr)
			movedCallback(pos);
	}

	void Window::windowFocusedCallback(bool hasfocus)
	{
		if(focusedCallback != nullptr)
			focusedCallback(hasfocus);
	}

	void Window::onResized(const std::function<void(ivec2 size)>& callback)    { resizedCallback = callback; }
	void Window::onMoved(const std::function<void(ivec2 pos)>& callback)       { movedCallback = callback; }
	void Window::onFocused(const std::function<void(bool hasfocus)>& callback) { focusedCallback = callback; }
}
