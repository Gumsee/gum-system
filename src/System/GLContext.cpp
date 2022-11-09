#include <limits>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <vector>


#include "GLContext.h"
#include "Display.h"
#include "Window.h"
#include "Output.h"
#include "FramebufferConfig.h"


namespace Gum {

int GLContext::GLVARS::MAX_TEXTURE_SIZE = 0;
std::string GLContext::GLVARS::RENDERER = "";
std::string GLContext::GLVARS::VERSION = "";
std::string GLContext::GLVARS::VENDOR = "";
std::string GLContext::GLVARS::SHADING_LANGUAGE_VERSION = "";
int GLContext::GLVARS::MAJOR_VERSION = 0;
int GLContext::GLVARS::MINOR_VERSION = 0;

std::string GLContext::sGLExtentions = "";
bool GLContext::bOpenGLInitialized = false;

GLContext::GLContext(Gum::Window* window, short requestedGLMajor, short requestedGLMinor)
{
    pWindow = window;
    iOpenGLVersionMajor = requestedGLMajor;
    iOpenGLVersionMinor = requestedGLMinor;

    initNative();
}

GLContext::GLContext(Gum::Window* window, const GLContext& copy)
{
    this->pWindow = window;
    this->pNativeContext = copy.pNativeContext;
    this->oFramebufferConfig = copy.getFramebufferConfig();
}

GLContext::~GLContext()
{
    destroyNative();
}

void GLContext::createContext()
{
    createNativeContext();
}

void GLContext::createContextWindow()
{
    createNativeContextWindow();
}


void GLContext::printInfo()
{
    Gum::Output::info("");
    Gum::Output::info("Context Info:");
    Gum::Output::info("  Renderer: "                + GLVARS::RENDERER);
    Gum::Output::info("  OpenGL version supported " + GLVARS::VERSION);
    Gum::Output::info("  GLSL version supported "   + GLVARS::SHADING_LANGUAGE_VERSION);
    Gum::Output::info("  OpenGL Version "           + std::to_string(GLVARS::MAJOR_VERSION) + "." + std::to_string(GLVARS::MINOR_VERSION));
    Gum::Output::info("  GLX Version "              + std::to_string(iGLXVersionMajor) + "." + std::to_string(iGLXVersionMinor));
    Gum::Output::info("  OpenGL Graphics Vendor: "  + GLVARS::VENDOR);
    Gum::Output::info("");
    Gum::Output::info("Framebuffer Info:");
    Gum::Output::info("  Color Bits: "              + oFramebufferConfig.rgbaBits.toString());
    Gum::Output::info("  Accum Color Bits: "        + oFramebufferConfig.accumrgbaBits.toString());
    Gum::Output::info("  Depth Bits: "              + std::to_string(oFramebufferConfig.depthBits));
    Gum::Output::info("  Stencil Bits: "            + std::to_string(oFramebufferConfig.stencilBits));
    Gum::Output::info("  Number of Samples: "       + std::to_string(oFramebufferConfig.numSamples));
    Gum::Output::info("  Doublebuffer Support: "    + std::string(oFramebufferConfig.doublebuffer_support ? "Yes" : "No"));
    Gum::Output::info("  sRGB Support: "            + std::string(oFramebufferConfig.sRGB_support ? "Yes" : "No"));
    Gum::Output::info("  Transparency Support: "    + std::string(oFramebufferConfig.transparent_support ? "Yes" : "No"));
}
	
void GLContext::initOpenGL()
{
    if(bOpenGLInitialized)
        return;
    
    //Initialize OpenGL Variables and glew
    Gum::Output::info("Initializing OpenGL Variables...");

    GLenum err = glewInit();

    if(err != GLEW_OK)
    {
        Gum::Output::error("GLEW ERROR: " + std::string(reinterpret_cast<const char*>(glewGetErrorString(err))));
        return;
    }

    
 
    //if(!gladLoadGLLoader((GLADloadproc)glXGetProcAddress)) 
    /*if(!gladLoadGL()) 
    {
        Gum::Output::error("GLAD Failed to initialize:");
        return;
    }*/
  
    // glad populates global constants after loading to indicate,
    // if a certain extension/version is available.
    //Gum::Output::print("OpenGL " + std::to_string(GLVersion.major) + "." + std::to_string(GLVersion.minor));

    /*if(GLAD_GL_EXT_framebuffer_multisample) {
        //GL_EXT_framebuffer_multisample is supported
    }
    if(GLAD_GL_VERSION_3_0) {
        //We support at least OpenGL version 3
    }*/

    
    //glEnable(GL_STENCIL_TEST);
    //glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    //glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glDepthFunc(GL_LEQUAL);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLVARS::RENDERER = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    GLVARS::VERSION = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    GLVARS::VENDOR = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    GLVARS::SHADING_LANGUAGE_VERSION = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));
    glGetIntegerv(GL_MAJOR_VERSION, &GLVARS::MAJOR_VERSION);
    glGetIntegerv(GL_MINOR_VERSION, &GLVARS::MINOR_VERSION);


    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &GLVARS::MAX_TEXTURE_SIZE);

    Gum::Output::info("Successfully initialized OpenGL Variables!");
    bOpenGLInitialized = true;
}

bool GLContext::isExtensionSupported(const char *extension)
{
    const char *start;
    const char *where, *terminator;

    /* Extension names should not have spaces. */
    where = strchr(extension, ' ');
    if (where || *extension == '\0')
        return false;

    /* It takes a bit of care to be fool-proof about parsing the
        OpenGL extensions string. Don't be fooled by sub-strings,
        etc. */
    for (start = sGLExtentions.c_str();;) 
    {
        where = strstr(start, extension);

        if (!where)
            break;

        terminator = where + strlen(extension);

        if((where == start || *(where - 1) == ' ') && (*terminator == ' ' || *terminator == '\0'))
            return true;

        start = terminator;
    }

    return false;
}


//
// Getter
//
Gum::ContextFBConfig GLContext::getFramebufferConfig() const { return oFramebufferConfig; }
void* GLContext::getNativeHandle() { return pNativeContext; }

}