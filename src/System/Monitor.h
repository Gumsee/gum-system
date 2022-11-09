#pragma once
#include <Maths/vec.h>
#include <string>

#define GUM_MONITOR_CONNECTOR_UNKNOWN   0x00
#define GUM_MONITOR_CONNECTOR_HDMI      0x01
#define GUM_MONITOR_CONNECTOR_DP        0x02
#define GUM_MONITOR_CONNECTOR_VGA       0x03
#define GUM_MONITOR_CONNECTOR_DVI       0x04

class Monitor
{
private:
    ivec2 iv2Resolution, iv2SizeMillimeters, iv2Position;
    uint8_t iPixelDepth;
    unsigned int iRefreshrate;
    unsigned int iID;
    short iRotation;
    uint8_t iConnectorType;
    std::string sOutputName;
    bool bIsPrimary;

public:
    Monitor(unsigned int monitorID, ivec2 resolution, ivec2 sizeMillimeters, ivec2 position, uint8_t pixelDepth, unsigned int refreshrate, short rotation, uint8_t connectorType, std::string name, bool isprimary);
    ~Monitor();

    //Getter
    uint8_t getPixelDepth() const;
    unsigned int getRefreshrate() const;
    unsigned int getID() const;
    short getRotation() const;
    ivec2 getResolution() const;
    ivec2 getPosition() const;
    std::string getOutputName() const;
    bool isPrimary() const;
};