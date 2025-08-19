#pragma once

//Rendering
#include <EGL/egl.h>
#include <GLES3/gl32.h>
#include "shaders/shaders.h"
#include "text/text.h"

//Drawing routines
struct OpenGLEngine {
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    EGLint width = 1, height = 1;
    GLuint dummy_tex;
};





#include <memory>
struct texture2D {
	int width, height;
	std::unique_ptr<unsigned char[]> tex;
};


#define STRINGIZE2(x) #x
#define STRINGIZE(x) STRINGIZE2(x)

#define GLCheck(msg) { \
    GLenum err = glGetError(); \
    if (err != GL_NO_ERROR) \
        LOG(STRINGIZE(__FILE__) + std::string(" : ") + STRINGIZE(__LINE__) + std::string(": GL ERROR ") + std::to_string(err) + " at " + msg );}

#define GLCheckI() { \
    GLenum err = glGetError(); \
    if (err != GL_NO_ERROR) \
        LOG(STRINGIZE(__FILE__) + std::string(" : ") + STRINGIZE(__LINE__) + std::string(": GL ERROR ") + std::to_string(err));}
