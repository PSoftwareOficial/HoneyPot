#pragma once
#include <thread>
#include <atomic>
#include 
#include "rendering/core_include.h"
#include "utilities/math/structs/vxd.h"
#include "utilities/Buffer/Buffer.h"

class Engine {
public:
    
private:
    int Init();
    int Update(uint64_t EuS, uint64_t TuS);
    int EndGL();
    int Destroy();


public:
    static void process_CMD(struct android_app* app, int32_t cmd);
    static int32_t process_INPUT(struct android_app* app, struct AInputEvent* event);

    static Engine* engine;
    static std::atomic<bool> bStarted;
    static std::atomic<bool> bRun;
    static std::atomic<bool> bDestroy;
    static std::thread tRun;
    static android_app* app;

    static OpenGLEngine openGLEngine;

    static void Loop();
    static void Start();
    static void Resume();
    static void Pause();
    static void Finish();

    struct InputEvent{
        V2D coord;
    };
    static TS_RB<InputEvent,64> touchEvents;
};