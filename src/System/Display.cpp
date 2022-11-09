#include "Display.h"
#include "Output.h"
#include "MemoryManagement.h"

#include "Window.h"

namespace Gum {
namespace Display
{
	std::vector<Monitor*> vMonitors;
	Monitor* pPrimaryMonitor;

    void init()
	{
    	/*glfwSetErrorCallback([](int error, const char* description) {
    		Gum::Output::error("GLFW Error("+std::to_string(error)+"): " + description);
		});*/
		initNativeDisplay();
	}

    void cleanup()
	{
		destroyNativeDisplay();
		
		for(Monitor* mon : vMonitors)
			Gum::_delete(mon);
	}

	void pollEvents() 
	{ 
		pollNativeEvents();
	}

	void addMonitors(Monitor* monitor)
	{	
		vMonitors.push_back(monitor);
	}

	void printInfo()
	{
		for(size_t i = 0; i < vMonitors.size(); i++)
		{
			Monitor* mon = vMonitors[i];
            std::string primaryStr = mon->isPrimary() ? " (Primary Monitor)" : "";
			Gum::Output::info("Display: " + std::to_string(i) + primaryStr);
			Gum::Output::info("\tOutput: " + mon->getOutputName());
			Gum::Output::info("\tResolution: " + mon->getResolution().toString("", "", "x"));
			Gum::Output::info("\tRefreshrate: " + std::to_string(mon->getRefreshrate()));
			Gum::Output::info("\tPixelDepth: " + std::to_string(mon->getPixelDepth()));
			Gum::Output::info("\tOffset: " + mon->getPosition().toString());
		}
	}


	void setPrimaryMonitor(Monitor* monitor) 	{ pPrimaryMonitor = monitor; }

	std::vector<Monitor*> getMonitors() 		{ return vMonitors; }
	Monitor* getPrimaryMonitor()        		{ return pPrimaryMonitor; }
}}