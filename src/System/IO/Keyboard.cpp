#include "Keyboard.h"
#include "../Window.h"
#include "../Output.h"
#include <locale>
#include <codecvt>

namespace Gum {
namespace IO
{
	Keyboard::Keyboard(Gum::Window* context)
	{
		pContextWindow = context;
		iLastPressedKey = 0;
		iLastReleasedKey = 0;
		u8TextInput = "";

		initNativeKeyboard();
	}

	Keyboard::~Keyboard()
	{
		destroyNativeKeyboard();
	}

	void Keyboard::reset()
	{
		this->u8TextInput = "";
		this->iLastPressedKey = 0;
		this->iLastReleasedKey = 0;
	}


	std::string Keyboard::key2string(const int& key)
	{
		std::string ret = "";
		if     (key == GUM_KEY_A) 					return "A";
		else if(key == GUM_KEY_B) 					return "B";
		else if(key == GUM_KEY_C) 					return "C";
		else if(key == GUM_KEY_D) 					return "D";
		else if(key == GUM_KEY_E) 					return "E";
		else if(key == GUM_KEY_F) 					return "F";
		else if(key == GUM_KEY_G) 					return "G";
		else if(key == GUM_KEY_H) 					return "H";
		else if(key == GUM_KEY_I) 					return "I";
		else if(key == GUM_KEY_J) 					return "J";
		else if(key == GUM_KEY_K) 					return "K";
		else if(key == GUM_KEY_L) 					return "L";
		else if(key == GUM_KEY_M) 					return "M";
		else if(key == GUM_KEY_N) 					return "N";
		else if(key == GUM_KEY_O) 					return "O";
		else if(key == GUM_KEY_P) 					return "P";
		else if(key == GUM_KEY_Q) 					return "Q";
		else if(key == GUM_KEY_R) 					return "R";
		else if(key == GUM_KEY_S) 					return "S";
		else if(key == GUM_KEY_T) 					return "T";
		else if(key == GUM_KEY_U) 					return "U";
		else if(key == GUM_KEY_V) 					return "V";
		else if(key == GUM_KEY_W) 					return "W";
		else if(key == GUM_KEY_X) 					return "X";
		else if(key == GUM_KEY_Y) 					return "Y";
		else if(key == GUM_KEY_Z) 					return "Z";
		else if(key == GUM_KEY_0) 					return "0";
		else if(key == GUM_KEY_1) 					return "1";
		else if(key == GUM_KEY_2) 					return "2";
		else if(key == GUM_KEY_3) 					return "3";
		else if(key == GUM_KEY_4) 					return "4";
		else if(key == GUM_KEY_5) 					return "5";
		else if(key == GUM_KEY_6) 					return "6";
		else if(key == GUM_KEY_7) 					return "7";
		else if(key == GUM_KEY_8) 					return "8";
		else if(key == GUM_KEY_9) 					return "9";
		else if(key == GUM_KEY_NUMPAD_0) 	    	return "Numpad0";
		else if(key == GUM_KEY_NUMPAD_1) 	    	return "Numpad1";
		else if(key == GUM_KEY_NUMPAD_2) 	    	return "Numpad2";
		else if(key == GUM_KEY_NUMPAD_3) 	    	return "Numpad3";
		else if(key == GUM_KEY_NUMPAD_4) 	    	return "Numpad4";
		else if(key == GUM_KEY_NUMPAD_5) 	    	return "Numpad5";
		else if(key == GUM_KEY_NUMPAD_6) 	    	return "Numpad6";
		else if(key == GUM_KEY_NUMPAD_7) 	    	return "Numpad7";
		else if(key == GUM_KEY_NUMPAD_8) 	    	return "Numpad8";
		else if(key == GUM_KEY_NUMPAD_9) 	    	return "Numpad9";
		else if(key == GUM_KEY_F1) 					return "F1";
		else if(key == GUM_KEY_F2) 					return "F2";
		else if(key == GUM_KEY_F3) 					return "F3";
		else if(key == GUM_KEY_F4) 					return "F4";
		else if(key == GUM_KEY_F5) 					return "F5";
		else if(key == GUM_KEY_F6) 					return "F6";
		else if(key == GUM_KEY_F7) 					return "F7";
		else if(key == GUM_KEY_F8) 					return "F8";
		else if(key == GUM_KEY_F9) 					return "F9";
		else if(key == GUM_KEY_F10) 				return "F10";
		else if(key == GUM_KEY_F11) 				return "F11";
		else if(key == GUM_KEY_F12) 				return "F12";
		else if(key == GUM_KEY_F13) 				return "F13";
		else if(key == GUM_KEY_F14) 				return "F14";
		else if(key == GUM_KEY_F15) 				return "F15";
		else if(key == GUM_KEY_F16) 				return "F16";
		else if(key == GUM_KEY_F17) 				return "F17";
		else if(key == GUM_KEY_F18) 				return "F18";
		else if(key == GUM_KEY_F19) 				return "F19";
		else if(key == GUM_KEY_F20) 				return "F20";
		else if(key == GUM_KEY_F21) 				return "F21";
		else if(key == GUM_KEY_F22) 				return "F22";
		else if(key == GUM_KEY_F23) 				return "F23";
		else if(key == GUM_KEY_F24) 				return "F24";
		else if(key == GUM_KEY_F25) 				return "F25";
		else if(key == GUM_KEY_ESCAPE) 				return "Escape";
		else if(key == GUM_KEY_LEFT_CONTROL) 		return "LControl";
		else if(key == GUM_KEY_LEFT_SHIFT) 			return "LShift";
		else if(key == GUM_KEY_LEFT_ALT) 			return "LAlt";
		else if(key == GUM_KEY_LEFT_SUPER) 			return "LSystem";
		else if(key == GUM_KEY_RIGHT_CONTROL) 		return "RControl";
		else if(key == GUM_KEY_RIGHT_SHIFT) 		return "RShift";
		else if(key == GUM_KEY_RIGHT_ALT) 			return "RAlt";
		else if(key == GUM_KEY_RIGHT_SUPER) 		return "RSystem";
		else if(key == GUM_KEY_MENU) 				return "Menu";
		else if(key == GUM_KEY_LEFT_PARENTHESIS) 	return "(";
		else if(key == GUM_KEY_RIGHT_PARENTHESIS) 	return ")";
		else if(key == GUM_KEY_LEFT_BRACKET) 		return "[";
		else if(key == GUM_KEY_RIGHT_BRACKET) 		return "]";
		else if(key == GUM_KEY_SEMICOLON) 			return ";";
		else if(key == GUM_KEY_COMMA) 				return ",";
		else if(key == GUM_KEY_PERIOD) 				return ".";
		else if(key == GUM_KEY_APOSTROPHE) 			return "\'";
		else if(key == GUM_KEY_SLASH) 				return "/";
		else if(key == GUM_KEY_BACKSLASH) 			return "\\";
		else if(key == GUM_KEY_EQUAL) 				return "=";
		else if(key == GUM_KEY_MINUS) 				return "-";
		else if(key == GUM_KEY_SPACE) 				return "Space";
		else if(key == GUM_KEY_ENTER) 				return "Return";
		else if(key == GUM_KEY_BACKSPACE) 			return "Back";
		else if(key == GUM_KEY_TAB) 				return "Tab";
		else if(key == GUM_KEY_PAGE_UP) 			return "Page Up";
		else if(key == GUM_KEY_PAGE_DOWN) 			return "Page Down";
		else if(key == GUM_KEY_END) 				return "End";
		else if(key == GUM_KEY_HOME) 				return "Home";
		else if(key == GUM_KEY_INSERT) 				return "Insert";
		else if(key == GUM_KEY_DELETE) 				return "Delete";
		else if(key == GUM_KEY_PAUSE) 				return "Paues";
		else if(key == GUM_KEY_NUMPAD_ADD) 			return "+";
		else if(key == GUM_KEY_NUMPAD_SUBTRACT) 	return "-";
		else if(key == GUM_KEY_NUMPAD_MULTIPLY) 	return "*";
		else if(key == GUM_KEY_NUMPAD_DIVIDE) 		return "/";
		else if(key == GUM_KEY_LEFT) 				return "Left";
		else if(key == GUM_KEY_RIGHT) 				return "Right";
		else if(key == GUM_KEY_UP) 					return "UP";
		else if(key == GUM_KEY_DOWN) 				return "Down";

		return "";
	}

	bool Keyboard::checkLastPressedKey(const int& key, const int& modkey) const
	{ 
		return iLastPressedKey == key && iLastPressedModKey == modkey; 
	}

	bool Keyboard::checkLastReleasedKey(const int& key, const int& modkey) const
	{ 
		return iLastReleasedKey == key && iLastReleasedModKey == modkey; 
	}

	bool Keyboard::checkKeyPressed(const int& key) const
	{ 
		//return glfwGetKey(pContextWindow->getRenderWindow(), key) == GLFW_PRESS; 
		return false;
	}

	bool Keyboard::checkKeyReleased(const int& key) const
	{ 
		//return glfwGetKey(pContextWindow->getRenderWindow(), key) == GLFW_RELEASE; 
		return false;
	}

	void Keyboard::setBusiness(const bool& val)  { busy = val; }

    
    void Keyboard::handleEvent(Event event)
    {
        switch(event.type)
        {
            case GUM_EVENT_KEYBOARD_PRESSED:
                /*c = Xutf8LookupString(ic, &event.xkey, buff, buff_size - 1, &ksym, &status);
                std::cout << ksym << std::endl;
                if (status == XBufferOverflow)
                {
                    printf("reallocate to the size of: %lu\n", c + 1);
                    buff = (char*)realloc(buff, c + 1);
                    c = Xutf8LookupString(ic, &event.xkey, buff, c, &ksym, &status);
                }*/
                
                //keyboardTextCallback(XLookupKeysym(&event.xkey, 0));
                keyboardPressCallback(event.data.keyboardkey, 0);
                break;
                
            case GUM_EVENT_KEYBOARD_RELEASED:
                keyboardReleaseCallback(event.data.keyboardkey, 0);
                break;
        }
    }

	bool Keyboard::isBusy() const           { return busy; }
	std::string Keyboard::getTextInput() const { return this->u8TextInput; }
}}