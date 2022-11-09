#pragma once
#include <Maths/vec.h>

namespace Gum {
struct ContextFBConfig
{
    ivec4 rgbaBits;
    ivec4 accumrgbaBits;
    int   depthBits = 0, stencilBits = 0;
    int   auxBuffers = 0;
    int   numSamples = 0;
    bool  stereo_support = false;
    bool  sRGB_support = false;
    bool  doublebuffer_support = false;
    bool  transparent_support = false;
    uintptr_t oshandle;
};
}