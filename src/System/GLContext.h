#pragma once
#include "FramebufferConfig.h"
#include <string>
#include <GL/glew.h>

namespace Gum {
class Window;

class GLContext
{
private:
    Gum::Window* pWindow;
    
    void* pNativeContext;
    void* pNativeContextWindow;
    
    Gum::ContextFBConfig oFramebufferConfig;
    int iGLXVersionMajor, iGLXVersionMinor;
    int iOpenGLVersionMajor, iOpenGLVersionMinor;
    static std::string sGLExtentions;
    static bool bOpenGLInitialized;

    bool bForwardCompatible = true;
    bool bCoreProfile = true;
    bool bCompatibilityProfile = false;
    bool bDebug = true;
    bool bRobustness = false;
    bool bRelease = true;
    bool bNoError = false;

    void initNative();
    void createNativeContext();
    void createNativeContextWindow();
    void destroyNative();

public:
    GLContext(Gum::Window* window, short requestedGLMajor, short requestedGLMinor);
    GLContext(Gum::Window* window, const GLContext& copy);
    ~GLContext();

    struct GLVARS
    {
        static int MAX_TEXTURE_SIZE;
        static std::string RENDERER;
        static std::string VERSION;
        static std::string VENDOR;
        static std::string SHADING_LANGUAGE_VERSION;
        static int MAJOR_VERSION;
        static int MINOR_VERSION;
    };
    void createContext();
    void createContextWindow();
    void swapBuffers();
    void bind();
    void unbind();

    void printInfo();

    Gum::ContextFBConfig getFramebufferConfig() const;

    void* getNativeHandle();

    static void initOpenGL();

    // Helper to check for extension string presence.  Adapted from:
    //   http://www.opengl.org/resources/features/OGLextensions/
    static bool isExtensionSupported(const char *extension);
};

}