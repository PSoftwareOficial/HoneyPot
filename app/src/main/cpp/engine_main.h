#pragma once
#include <thread>
#include <atomic>

#include <android_native_app_glue.h>


#include "render.h"
#include "utilities/log/API.h"


#include "custom/API.h"

std::atomic<bool> bRunEngine;
std::thread tRunEngine;



int EngineStart(struct android_app* app){
	 LOG("Starting Engine");
        [[maybe_unused]] uint32_t frame = 0;

        //Init the GL Engine
        init_gles(app, &gles_display, &gles_surface, &gles_context);


        /*//Get the relevant data
        glGenTextures(1, &dummy_tex);
        glBindTexture(GL_TEXTURE_2D, dummy_tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, whitePixel);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);*/

        gles_width = ANativeWindow_getWidth(app->window);
        gles_height = ANativeWindow_getHeight(app->window);

        return 0;
}

int EngineStop(){
        eglDestroySurface(gles_display, gles_surface);
        eglDestroyContext(gles_display, gles_context);
        eglTerminate(gles_display);

        return 0;
}


#include <chrono>
int EngineLoop(struct android_app* app){
	EngineStart(app);
	 LOG("Starting Engine Loop");
         //Init some data possible
	InitWorld();

        // Save the starting time point
        auto timeStartEngine = std::chrono::system_clock::now();
                auto timePrevFrame = std::chrono::system_clock::now();
                UpdateWorld(0.0,0.0);
		DrawFrame();
	while(bRunEngine.load()){
                auto timeFrame = std::chrono::system_clock::now();
		UpdateWorld(std::chrono::duration<uint64_t, std::micro>(timeFrame - timePrevFrame).count(),std::chrono::duration<uint64_t, std::micro>(timeFrame - timeStartEngine).count());
		DrawFrame();
                // Get the ending time point
                timePrevFrame = timeFrame;
	}



	EngineStop();

        return 0;
}

