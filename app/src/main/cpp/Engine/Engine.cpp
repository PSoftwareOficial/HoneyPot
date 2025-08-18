#include <android/native_activity.h>
#include <android_native_app_glue.h>
#include <android/input.h>

#include "Engine.h"
#include "EngineFunctions.h"
#include "AndroidFunctions.h"


static Enginestd::atomic<bool> Engine::bRun = false;
static std::atomic<bool> Engine::bRender = false;
static std::atomic<bool> Engine::bDestroy = false;
static std::thread Engine::tRun;
static Engine* Engine::engine = nullptr;
static struct android_app* Engine::app;
static OpenGLEngine Engine::openGLEngine;
static TS_RB<InputEvent,64> Engine::touchEvents;