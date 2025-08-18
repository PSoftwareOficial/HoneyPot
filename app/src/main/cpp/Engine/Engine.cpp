#include <android/native_activity.h>
#include <android_native_app_glue.h>
#include <android/input.h>

#include "Engine.h"
#include "EngineFunctions.h"
#include "AndroidFunctions.h"


std::atomic<bool> Engine::bRun = false;
std::atomic<bool> Engine::bRender = false;
std::atomic<bool> Engine::bDestroy = false;
std::thread Engine::tRun;
Engine* Engine::engine = nullptr;
struct android_app* Engine::app;
OpenGLEngine Engine::openGLEngine;
TS_RB<Engine::InputEvent,64> Engine::touchEvents;