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
TS_RB<InputEvent,64> Engine::touchEvents;
Engine::SystemData Engine::systemData;
UIManager Engine::uiManager;
TextRenderer Engine::textRenderer;





static Engine engine;

extern "C" void android_main(struct android_app* state) {
	Engine::engine = &engine;
	Engine::app = state;
	Logger::getInstance().init(state->activity->externalDataPath);
	AssetIO::getInstance().init(state->activity->assetManager);
	LOGI("App started");

	state->onAppCmd = Engine::process_CMD;
	state->onInputEvent = Engine::process_INPUT;
    Engine::tRun = std::thread(&Engine::Loop, Engine::engine);


	struct android_poll_source* engine_poll_source;

	//Detect, recive and process all events
	while(!state->destroyRequested){
		auto result = ALooper_pollOnce(-1, nullptr, nullptr, (void**)&engine_poll_source);
		
		//If error in Polling, terminate
		if(result == ALOOPER_POLL_ERROR){
			LOGE("Looper Poll Return");
			break;
		} 
		//Internal processing
		if(engine_poll_source) engine_poll_source->process(state, engine_poll_source);


	}
}