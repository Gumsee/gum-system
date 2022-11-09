#include "Controls.h"
#include "../Output.h"
#include "Keyboard.h"

namespace Gum {
namespace IO {
namespace Controls
{
	int iCurrentKey;
	std::map<std::string, int> mControls;

	void init()
	{
		Gum::Output::log("Setting Default Controls!");
		setControl("Forward", GUM_KEY_W);
		setControl("Backward", GUM_KEY_S);
		setControl("Right", GUM_KEY_D);
		setControl("Left", GUM_KEY_A);
		setControl("Jump", GUM_KEY_SPACE);
		setControl("Up", GUM_KEY_SPACE);
		setControl("Down", GUM_KEY_LEFT_CONTROL);
		setControl("Speed", GUM_KEY_F);
	}

	bool checkControl(const char* ControlName, Keyboard* keyboard)
	{
		return keyboard->checkKeyPressed(mControls[ControlName]);
	}

	//Setter
	void setCurrentKey(const int& character)                        { iCurrentKey = character; }
	void setControl(const std::string& ControlName, const int& key) { mControls[ControlName] = key; }

	//Getter
	char getCurrentKey()		                                    { return iCurrentKey; }
}}}