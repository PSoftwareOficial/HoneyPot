#include <android/native_activity.h>
#include <android_native_app_glue.h>
#include <android/input.h>


#include "Engine.h"
#include "EngineFunctions.h"
#include "AndroidFunctions.h"

#include "utilities/AssetIO/API.h"
#include "Engine/UI/UIManager.h"
#include "World/World.h"


#include <memory>

std::atomic<bool> Engine::bRun = false;
std::atomic<bool> Engine::bRender = false;
std::atomic<bool> Engine::bDestroy = false;
std::thread Engine::tRun;
Engine* Engine::engine = nullptr;
struct android_app* Engine::app;
OpenGLEngine Engine::openGLEngine;
std::unique_ptr<TS_RB<InputEvent,64>> Engine::touchEvents = std::make_unique<TS_RB<InputEvent,64>>();
Engine::SystemData Engine::systemData;

//UI Manager
std::unique_ptr<class UIManager> Engine::uiManager = std::make_unique<UIManager>();

//Base text renderer
std::unique_ptr<class TextRenderer> Engine::textRenderer = std::make_unique<TextRenderer>();;

//World pointer
std::unique_ptr<class World> Engine::world = std::make_unique<World>();;



static Engine engine;

extern "C" void android_main(struct android_app* state) {
	
	Logger::getInstance().init(state->activity->externalDataPath);
	LOGI("App started");
	AssetIO::getInstance().init(state->activity->assetManager);
	
	Engine::engine = &engine;
	Engine::app = state;

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