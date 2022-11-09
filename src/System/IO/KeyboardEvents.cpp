#include "Keyboard.h"
#include "../Window.h"
#include "../Output.h"
#include <locale>
#include <codecvt>

namespace Gum {
namespace IO
{	

	void Keyboard::keyboardPressCallback(unsigned int key, unsigned int modkey)
	{
		iLastPressedKey  = key; 
		iLastPressedModKey = modkey;

		if(keyPressedCallback != nullptr)
			keyPressedCallback(key, modkey);
	}

	void Keyboard::keyboardReleaseCallback(unsigned int key, unsigned int modkey)
	{
		iLastReleasedKey = key; 
		iLastReleasedModKey = modkey;

		if(keyReleasedCallback != nullptr)
			keyReleasedCallback(key, modkey);
	}

	void Keyboard::keyboardTextCallback(unsigned int codepoint)
	{
		std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
		u8TextInput = convert.to_bytes(codepoint);

		if(textEnteredCallback != nullptr)
			textEnteredCallback(u8TextInput, codepoint);
	}

	
	void Keyboard::onKeyPress(const std::function<void(unsigned int key, unsigned int modkey)>& callback)       { keyPressedCallback = callback; }
	void Keyboard::onKeyRelease(const std::function<void(unsigned int key, unsigned int modkey)>& callback)     { keyReleasedCallback = callback; }
	void Keyboard::onTextEntered(const std::function<void(std::string utf8, unsigned int codepoint)>& callback) { textEnteredCallback = callback; }
}}