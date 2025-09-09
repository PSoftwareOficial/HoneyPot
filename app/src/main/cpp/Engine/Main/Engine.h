#pragma once
#include <thread>
#include <atomic>
#include <cstdint>
#include "../UI/UIManager.h"
#include "rendering/core_include.h"
#include "../utilities/math/structs/vxd.h"
#include "../utilities/Buffer/Buffer.h"





class Engine {
private:
    //Function which starts the complete engine
    int Init();
    //Function which inits the complete open GL
    int InitGL();
    //Function which updates the complete engine (next frame)
    int Update(uint64_t EuS, uint64_t TuS);

    //Function which Resumes GL
    int ResumeGL();
    //Function which Pauses GL for updates
    int PauseGL();

    //Function which ends everything associated with OpenGL
    int EndGL();

    //Function which destroys the complete Engine.
    int Destroy();



    //Is the engine currently rendering
    std::atomic<bool> bRendering = false;
    //Has the engine started
    std::atomic<bool> bStarted = false;

public:
    static void process_CMD(struct android_app* app, int32_t cmd);
    static int32_t process_INPUT(struct android_app* app, struct AInputEvent* event);

    
    
    //Should the engine be running
    static std::atomic<bool> bRun;

    //Should the engine be rendering
    static std::atomic<bool> bRender;

    //Should the engine be destroyed
    static std::atomic<bool> bDestroy;

    //Thread of the engine
    static std::thread tRun;

    //Engine pointer
    static Engine* engine;
    //Native app pointer
    static struct android_app* app;
    //Open GL Pointer
    static OpenGLEngine openGLEngine;

    //UI Manager
    static UIManager uiManager;

    //Base text renderer
    static TextRenderer textRenderer;

    //Functions which Loops through the engine
    void Loop();

    
    //Input event queue
    static TS_RB<InputEvent,64> touchEvents;


        //General Data
    struct SystemData {
        float glNavBarHeight = 0;
    };
    static void CalcSystemData();
    static SystemData systemData;
    
};