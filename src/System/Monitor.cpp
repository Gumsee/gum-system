#include "Monitor.h"
#include "Display.h"

Monitor::Monitor(unsigned int monitorID, ivec2 resolution, ivec2 sizeMillimeters, ivec2 position, uint8_t pixelDepth, unsigned int refreshrate, short rotation, uint8_t connectorType, std::string name, bool isprimary)
{
    this->iID = monitorID;
    this->iv2Resolution = resolution;
    this->iv2SizeMillimeters = sizeMillimeters;
    this->iv2Position = position;
    this->iPixelDepth = pixelDepth;
    this->iRefreshrate = refreshrate;
    this->iRotation = rotation;
    this->iConnectorType = connectorType;
    this->sOutputName = name;
    this->bIsPrimary = isprimary;
}

Monitor::~Monitor()
{

}

uint8_t Monitor::getPixelDepth() const       { return this->iPixelDepth; }
unsigned int Monitor::getRefreshrate() const { return this->iRefreshrate; }
unsigned int Monitor::getID() const          { return this->iID; }
ivec2 Monitor::getResolution() const         { return this->iv2Resolution; }
ivec2 Monitor::getPosition() const           { return this->iv2Position; }
short Monitor::getRotation() const           { return this->iRotation; }
std::string Monitor::getOutputName() const   { return this->sOutputName; }
bool Monitor::isPrimary() const              { return this->bIsPrimary; }