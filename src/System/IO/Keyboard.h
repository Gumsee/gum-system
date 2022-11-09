#pragma once
#include <string>
#include <functional>

#include "../Event.h"

namespace Gum {
class Window;

namespace IO
{
	class Keyboard
	{
	private:
		bool busy;

		Gum::Window* pContextWindow;
		std::string u8TextInput;
		int iLastPressedKey, iLastReleasedKey;
		int iLastPressedModKey, iLastReleasedModKey;

		void initNativeKeyboard();
		void destroyNativeKeyboard();

		
		//Callbacks
		void keyboardPressCallback(unsigned int key, unsigned int modkey);
		void keyboardReleaseCallback(unsigned int key, unsigned int modkey);
		void keyboardTextCallback(unsigned int codepoint);
		std::function<void(std::string, unsigned int)> textEnteredCallback = nullptr;
		std::function<void(unsigned int, unsigned int)> keyPressedCallback = nullptr, keyReleasedCallback = nullptr;

	public:
        Keyboard(Gum::Window* context);
        Keyboard(Keyboard& other) = delete;
        Keyboard& operator=(Keyboard& other) = delete;
		~Keyboard();

		static std::string key2string(const int& key);

		bool checkLastPressedKey(const int& key, const int& modkey = 0) const;
		bool checkLastReleasedKey(const int& key, const int& modkey = 0) const;
		bool checkKeyPressed(const int& key) const;
		bool checkKeyReleased(const int& key) const;

		void handleEvent(Event event);

		void reset();
		
		//Events
		void onKeyPress(const std::function<void(unsigned int key, unsigned int modkey)>& callback);
		void onKeyRelease(const std::function<void(unsigned int key, unsigned int modkey)>& callback);
		void onTextEntered(const std::function<void(std::string utf8, unsigned int codepoint)>& callback);

		//Setter
		void setBusiness(const bool& val);
		
		//Getter
		bool isBusy() const;
		std::string getTextInput() const;
	};


#if defined GUM_OS_LINUX || defined GUM_OS_BSD
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif
extern int GUM_KEY_UNKNOWN;
extern int GUM_KEY_SPACE;
extern int GUM_KEY_APOSTROPHE;
extern int GUM_KEY_COMMA;
extern int GUM_KEY_MINUS;
extern int GUM_KEY_PERIOD;
extern int GUM_KEY_SLASH;
extern int GUM_KEY_0;
extern int GUM_KEY_1;
extern int GUM_KEY_2;
extern int GUM_KEY_3;
extern int GUM_KEY_4;
extern int GUM_KEY_5;
extern int GUM_KEY_6;
extern int GUM_KEY_7;
extern int GUM_KEY_8;
extern int GUM_KEY_9;
extern int GUM_KEY_SEMICOLON;
extern int GUM_KEY_EQUAL;
extern int GUM_KEY_A;
extern int GUM_KEY_B;
extern int GUM_KEY_C;
extern int GUM_KEY_D;
extern int GUM_KEY_E;
extern int GUM_KEY_F;
extern int GUM_KEY_G;
extern int GUM_KEY_H;
extern int GUM_KEY_I;
extern int GUM_KEY_J;
extern int GUM_KEY_K;
extern int GUM_KEY_L;
extern int GUM_KEY_M;
extern int GUM_KEY_N;
extern int GUM_KEY_O;
extern int GUM_KEY_P;
extern int GUM_KEY_Q;
extern int GUM_KEY_R;
extern int GUM_KEY_S;
extern int GUM_KEY_T;
extern int GUM_KEY_U;
extern int GUM_KEY_V;
extern int GUM_KEY_W;
extern int GUM_KEY_X;
extern int GUM_KEY_Y;
extern int GUM_KEY_Z;
extern int GUM_KEY_LEFT_PARENTHESIS;
extern int GUM_KEY_RIGHT_PARENTHESIS;
extern int GUM_KEY_LEFT_BRACKET;
extern int GUM_KEY_RIGHT_BRACKET;
extern int GUM_KEY_BACKSLASH;
extern int GUM_KEY_GRAVE_ACCENT;
//extern int GUM_KEY_WORLD_1 
//extern int GUM_KEY_WORLD_2 
extern int GUM_KEY_ESCAPE;
extern int GUM_KEY_ENTER;
extern int GUM_KEY_TAB;
extern int GUM_KEY_BACKSPACE;
extern int GUM_KEY_INSERT;
extern int GUM_KEY_DELETE;
extern int GUM_KEY_RIGHT;
extern int GUM_KEY_LEFT;
extern int GUM_KEY_DOWN;
extern int GUM_KEY_UP;
extern int GUM_KEY_PAGE_UP;
extern int GUM_KEY_PAGE_DOWN;
extern int GUM_KEY_HOME;
extern int GUM_KEY_END;
extern int GUM_KEY_CAPS_LOCK;
extern int GUM_KEY_SCROLL_LOCK;
extern int GUM_KEY_NUM_LOCK;
extern int GUM_KEY_PRINT_SCREEN;
extern int GUM_KEY_PAUSE;
extern int GUM_KEY_F1;
extern int GUM_KEY_F2;
extern int GUM_KEY_F3;
extern int GUM_KEY_F4;
extern int GUM_KEY_F5;
extern int GUM_KEY_F6;
extern int GUM_KEY_F7;
extern int GUM_KEY_F8;
extern int GUM_KEY_F9;
extern int GUM_KEY_F10;
extern int GUM_KEY_F11;
extern int GUM_KEY_F12;
extern int GUM_KEY_F13;
extern int GUM_KEY_F14;
extern int GUM_KEY_F15;
extern int GUM_KEY_F16;
extern int GUM_KEY_F17;
extern int GUM_KEY_F18;
extern int GUM_KEY_F19;
extern int GUM_KEY_F20;
extern int GUM_KEY_F21;
extern int GUM_KEY_F22;
extern int GUM_KEY_F23;
extern int GUM_KEY_F24;
extern int GUM_KEY_F25;
extern int GUM_KEY_NUMPAD_0;
extern int GUM_KEY_NUMPAD_1;
extern int GUM_KEY_NUMPAD_2;
extern int GUM_KEY_NUMPAD_3;
extern int GUM_KEY_NUMPAD_4;
extern int GUM_KEY_NUMPAD_5;
extern int GUM_KEY_NUMPAD_6;
extern int GUM_KEY_NUMPAD_7;
extern int GUM_KEY_NUMPAD_8;
extern int GUM_KEY_NUMPAD_9;
extern int GUM_KEY_NUMPAD_DECIMAL;
extern int GUM_KEY_NUMPAD_DIVIDE;
extern int GUM_KEY_NUMPAD_MULTIPLY;
extern int GUM_KEY_NUMPAD_SUBTRACT;
extern int GUM_KEY_NUMPAD_ADD;
extern int GUM_KEY_NUMPAD_ENTER;
extern int GUM_KEY_NUMPAD_EQUAL;
extern int GUM_KEY_LEFT_SHIFT;
extern int GUM_KEY_LEFT_CONTROL;
extern int GUM_KEY_LEFT_ALT;
extern int GUM_KEY_LEFT_SUPER;
extern int GUM_KEY_RIGHT_SHIFT;
extern int GUM_KEY_RIGHT_CONTROL;
extern int GUM_KEY_RIGHT_ALT;
extern int GUM_KEY_RIGHT_SUPER;
extern int GUM_KEY_MENU;

extern long GUM_MOD_SHIFT;
extern long GUM_MOD_CONTROL;
extern long GUM_MOD_ALT;
extern long GUM_MOD_SUPER;
extern long GUM_MOD_CAPS_LOCK;
extern long GUM_MOD_NUM_LOCK;
extern long GUM_MOD_ESCAPE;

#if defined GUM_OS_LINUX || defined GUM_OS_BSD
#pragma GCC diagnostic pop
#endif


}}