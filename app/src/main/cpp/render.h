#pragma once
#include <vector>

#include "rendering/core_include.h"
#include "utilities/log/API.h"


void DrawFrame(EGLDisplay* display, EGLSurface* surface) {
        eglSwapBuffers(*display, *surface);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
}



void init_gles(struct android_app* app, EGLDisplay* display, EGLSurface* surface, EGLContext* context){
        LOGI("Init Gles");

	*display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        eglInitialize(*display, NULL, NULL);

       	const EGLint configAttribs[] = {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_DEPTH_SIZE, 0,
        EGL_NONE
    };

        EGLConfig config;
        EGLint numConfigs;
        eglChooseConfig(*display, configAttribs, &config, 1, &numConfigs);

        EGLint format;
        eglGetConfigAttrib(*display, config, EGL_NATIVE_VISUAL_ID, &format);
        ANativeWindow_setBuffersGeometry(app->window, 0, 0, format);

        *surface = eglCreateWindowSurface(*display, config, app->window, NULL);

        EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE };
        *context = eglCreateContext(*display, config, EGL_NO_CONTEXT, contextAttribs);

        eglMakeCurrent(*display, *surface, *surface, *context);
}
