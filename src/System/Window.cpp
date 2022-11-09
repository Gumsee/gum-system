#include "Window.h"
#include "Event.h"
#include "GLContext.h"
#include "IO/Keyboard.h"
#include "IO/Mouse.h"
#include "Output.h"
#include "MemoryManagement.h"
#include "Display.h"

#include "DefaultIcon/icon22.h"
#include "DefaultIcon/icon32.h"
#include "DefaultIcon/icon48.h"
#include "DefaultIcon/icon64.h"
#include "DefaultIcon/icon128.h"


bool checkPointInArea(ivec2 point, bbox2i bbox)
{
	return  point.x <= bbox.getPos().x + bbox.getSize().x &&
			point.x >= bbox.getPos().x && 
			point.y <= bbox.getPos().y + bbox.getSize().y && 
			point.y >= bbox.getPos().y;
}

namespace Gum
{
	Window* Window::MainWindow = nullptr;
	Window* Window::CurrentlyBoundWindow = nullptr;
	bool Window::WINDOW_IS_ACTIVE_SCALING = false;
	std::vector<Window*> Window::AllWindows;
	ivec4 Window::iv4DecorationSize;


	Window::Window(std::string title, ivec2 windowsize, int properties, Window* parentWindow, Monitor* monitor)
	{
		this->pParentWindow = parentWindow;
		this->bKeepInsideParent = (properties & GUM_WINDOW_TRAP_IN_PARENT);
		this->v2Size = windowsize;
		this->sTitle = title;
		this->bIsHidden = false;
		this->bHasScalingSnapped = false;
		this->bIsMaximized = false;
		this->bShouldClose = false;
		this->bIsMinimized = false;

		if(monitor == nullptr)
			monitor = Gum::Display::getPrimaryMonitor();
		this->pMonitor = monitor;

		if(properties & GUM_WINDOW_SIZE_IN_PERCENT)
		{
			v2Size.x = Display::getPrimaryMonitor()->getResolution().x * ((float)v2Size.x / 100.0f);
			v2Size.y = Display::getPrimaryMonitor()->getResolution().y * ((float)v2Size.y / 100.0f);
		}
		this->v2PixelSize = vec2(1.0f) / (vec2)v2Size;

		if((properties & GUM_WINDOW_SHARECONTEXT))
		{
			pContext = new GLContext(this, *pParentWindow->getContext());
			initNativeWindow();
		}
		else
		{
			pContext = new GLContext(this, 3, 0);
			initNativeWindow();
			pContext->createContext();
		}

		pContext->createContextWindow();
		bind();


		setTitle(title);
		setVerticalSync(properties & GUM_WINDOW_VERTICAL_SYNC);
		showBorder   (!(properties & GUM_WINDOW_BORDERLESS));
		makeResizable  (properties & GUM_WINDOW_RESIZABLE);
		makeFloating   (properties & GUM_WINDOW_FLOATING);
		makeFullscreen (properties & GUM_WINDOW_FULLSCREEN);
		maximize       (properties & GUM_WINDOW_MAXIMIZED);
		
		setIcon({ icon22, icon32, icon48, icon64, icon128 }, true, vec4(0.61, 0.53, 1, 1));

		if(Window::MainWindow == nullptr)
			Window::MainWindow = this;


		fAspectRatio = (float)v2Size.y / (float)v2Size.x;
		fAspectRatioWidthToHeight = (float)v2Size.x / (float)v2Size.y;
		m4ScreenMatrix = Gum::Maths::ortho((float)v2Size.y, (float)v2Size.x, 0.0f, 0.0f, -100.0f, 100.0f);


		pKeyboard = new Gum::IO::Keyboard(this);
		pMouse = new Gum::IO::Mouse(this);
		AllWindows.push_back(this);
	}

	Window::~Window()
	{
		AllWindows.erase(std::find(AllWindows.begin(), AllWindows.end(), this));
		destroyNativeWindow();
		Gum::_delete(pContext);
		Gum::_delete(pKeyboard);
		Gum::_delete(pMouse);
	}


	void Window::resetViewport()
	{
		glViewport(0.0f, 0.0f, getSize().x, getSize().y);
		glScissor(0.0f, 0.0f, getSize().x, getSize().y);
	}

	void Window::update()
	{
		if(bIsResizable && !bHasBorder && this->pMouse != nullptr)
		{;
			if(checkPointInArea(pMouse->getPosition(), bbox2i(getSize() - ivec2(20, 20), getSize())))
			{
				pMouse->setCursor(GUM_CURSOR_TOPLEFT_TO_BOTTOMRIGHT_RESIZE);
				if(pMouse->hasLeftClick() && !IO::Mouse::isBusy())
				{
					if(!Gum::Window::WINDOW_IS_ACTIVE_SCALING)
					{
						Gum::Window::WINDOW_IS_ACTIVE_SCALING = true;
						bHasScalingSnapped = true;
						IO::Mouse::setBusiness(true);
					}
				}
			}

			if(bHasScalingSnapped)
			{
				setSize(getSize() + IO::Mouse::getDelta());
				if(getSize().x < 100) { setSize(vec2(100, v2Size.y)); }
				if(getSize().y < 100) { setSize(vec2(v2Size.x, 100)); }

				if(!pMouse->hasLeftClick())
				{
					Gum::Window::WINDOW_IS_ACTIVE_SCALING = false;
					bHasScalingSnapped = false;
					IO::Mouse::setBusiness(false);
				}
			}
		}
	}


	void Window::close()            	{ bShouldClose = true; }
	void Window::clear(int clearbits) 	{ glClear(clearbits); }
	void Window::bind()					
	{ 
		getContext()->bind();
		Window::CurrentlyBoundWindow = this;
		glViewport(0.0f, 0.0f, getSize().x, getSize().y);
	}
	void Window::unbind()
	{ 
		Window::MainWindow->getContext()->bind();
		Window::CurrentlyBoundWindow = Window::MainWindow;
		glViewport(0.0f, 0.0f, Window::MainWindow->getSize().x, Window::MainWindow->getSize().y);
	}
    
    void Window::handleEvent(Event event)
    {
        switch(event.type)
        {
			//case ResizeRequest:    break;
            case GUM_EVENT_FOCUS_IN:         windowFocusedCallback(true); break;
            case GUM_EVENT_FOCUS_OUT:        windowFocusedCallback(false); break;
            case GUM_EVENT_WINDOW_REPOSITION: 
                if(event.data.windowpos != v2Pos)
                    windowMovedCallback(event.data.windowpos);
				break;

			case GUM_EVENT_WINDOW_RESIZE:
                if(event.data.windowsize != v2Size)
                    windowResizedCallback(event.data.windowsize);
                break;

            case GUM_EVENT_WINDOW_CLOSE:
				close();
                /*else if (msgType == XdndEnter)
                {
                    // A drag operation has entered the window
                    unsigned long count;
                    Atom* formats = NULL;
                    const GLFWbool list = event.xclient.data.l[1] & 1;

                    _glfw.x11.xdnd.source  = event.xclient.data.l[0];
                    _glfw.x11.xdnd.version = event.xclient.data.l[1] >> 24;
                    _glfw.x11.xdnd.format  = None;

                    if (_glfw.x11.xdnd.version > _GLFW_XDND_VERSION)
                        return;

                    if (list)
                    {
                        count = _glfwGetWindowPropertyX11(_glfw.x11.xdnd.source,
                                                        _glfw.x11.XdndTypeList,
                                                        XA_ATOM,
                                                        (unsigned char**) &formats);
                    }
                    else
                    {
                        count = 3;
                        formats = (Atom*) event.xclient.data.l + 2;
                    }

                    for (unsigned int i = 0;  i < count;  i++)
                    {
                        if (formats[i] == _glfw.x11.text_uri_list)
                        {
                            _glfw.x11.xdnd.format = _glfw.x11.text_uri_list;
                            break;
                        }
                    }

                    if (list && formats)
                        XFree(formats);
                }
                else if(msgType == XdndDrop)
                {
                    // The drag operation has finished by dropping on the window
                    Time time = CurrentTime;

                    if (_glfw.x11.xdnd.version > _GLFW_XDND_VERSION)
                        return;

                    if (_glfw.x11.xdnd.format)
                    {
                        if (_glfw.x11.xdnd.version >= 1)
                            time = event.xclient.data.l[2];

                        // Request the chosen format from the source window
                        XConvertSelection(display,
                                        _glfw.x11.XdndSelection,
                                        _glfw.x11.xdnd.format,
                                        _glfw.x11.XdndSelection,
                                        window->x11.handle,
                                        time);
                    }
                    else if (_glfw.x11.xdnd.version >= 2)
                    {
                        XEvent reply = { ClientMessage };
                        reply.xclient.window = _glfw.x11.xdnd.source;
                        reply.xclient.message_type = _glfw.x11.XdndFinished;
                        reply.xclient.format = 32;
                        reply.xclient.data.l[0] = window->x11.handle;
                        reply.xclient.data.l[1] = 0; // The drag was rejected
                        reply.xclient.data.l[2] = None;

                        XSendEvent(display, _glfw.x11.xdnd.source,
                                False, NoEventMask, &reply);
                        XFlush(display);
                    }
                }
                else if (msgType == XdndPosition)
                {
                    // The drag operation has moved over the window
                    const int xabs = (event.xclient.data.l[2] >> 16) & 0xffff;
                    const int yabs = (event.xclient.data.l[2]) & 0xffff;
                    Window dummy;
                    int xpos, ypos;

                    if (_glfw.x11.xdnd.version > _GLFW_XDND_VERSION)
                        return;

                    XTranslateCoordinates(display,
                                        _glfw.x11.root,
                                        window->x11.handle,
                                        xabs, yabs,
                                        &xpos, &ypos,
                                        &dummy);

                    _glfwInputCursorPos(window, xpos, ypos);

                    XEvent reply = { ClientMessage };
                    reply.xclient.window = _glfw.x11.xdnd.source;
                    reply.xclient.message_type = _glfw.x11.XdndStatus;
                    reply.xclient.format = 32;
                    reply.xclient.data.l[0] = window->x11.handle;
                    reply.xclient.data.l[2] = 0; // Specify an empty rectangle
                    reply.xclient.data.l[3] = 0;

                    if (_glfw.x11.xdnd.format)
                    {
                        // Reply that we are ready to copy the dragged data
                        reply.xclient.data.l[1] = 1; // Accept with no rectangle
                        if (_glfw.x11.xdnd.version >= 2)
                            reply.xclient.data.l[4] = _glfw.x11.XdndActionCopy;
                    }

                    XSendEvent(display, _glfw.x11.xdnd.source, False, NoEventMask, &reply);
                    XFlush(display);
                }*/

                break;
        }
    }


	//Setter
	void Window::setClearColor(vec4 color)			  { glClearColor(color.r, color.g, color.b, color.a); }
	void Window::setResizable(const bool& resizable)  { this->bIsResizable = resizable; }

	//Getter
	ivec2 Window::getSize() const              		  { return this->v2Size; }
	vec2 Window::getPixelSize() const          		  { return this->v2PixelSize; }
	ivec2 Window::getPosition() const          		  { return this->v2Pos; }
	mat4 Window::getScreenMatrix() const			  { return this->m4ScreenMatrix; }
	std::string Window::getTitle() const			  { return this->sTitle; }
	IO::Keyboard* Window::getKeyboard()				  { return this->pKeyboard; }
	IO::Mouse* Window::getMouse()					  { return this->pMouse; }
	float Window::getAspectRatio() const        	  { return this->fAspectRatio; }
	float Window::getAspectRatioWidthToHeight() const { return this->fAspectRatioWidthToHeight; }
	bool Window::isFullscreen() const          		  { return this->bIsFullscreen; }
	bool Window::isOpen() const                		  { return !this->bShouldClose; }
	Gum::Window* Window::getParentWindow()			  { return this->pParentWindow; }
	bool Window::isMaximized() const				  { return this->bIsMaximized; }
	bool Window::isResizable() const				  { return this->bIsResizable; }
	GLContext* Window::getContext()					  { return this->pContext; }
	void* Window::getNativeWindow() const 		      { return this->pNativeWindowHandle; }
	
	Window* MainWindow = nullptr;
}
