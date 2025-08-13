#include <android/native_activity.h>
#include <android_native_app_glue.h>
#include <android/input.h>

#include "Engine.h"
#include "custom/API.h"

//Utilities
#include "render.h"
#include "utilities/log/API.h"
#include "utilities/AssetIO/API.h"



Engine engine;
int Engine::Update(uint64_t EuS, uint64_t TuS){
	UpdateWorld(EuS,TuS);
	DrawFrame(&Engine::openGLEngine.display, &Engine::openGLEngine.surface);
	return 0;
}


void Engine::Loop() {
	//Init the Engine
	Engine::engine->Init();
	InitWorld();

	// Save the starting time point
	auto timeStartEngine = std::chrono::system_clock::now();
    auto timePrevFrame = std::chrono::system_clock::now();
	while(!bDestroy.load()){
		while(bRun.load()) {
			auto timeFrame = std::chrono::system_clock::now();
			Engine::engine->Update(std::chrono::duration<uint64_t, std::micro>(timeFrame - timePrevFrame).count(),std::chrono::duration<uint64_t, std::micro>(timeFrame - timeStartEngine).count());
			timePrevFrame = timeFrame;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	Engine::engine->Destroy();
	
}











int Engine::Init(){
	LOG("Starting Engine");
	[[maybe_unused]] uint32_t frame = 0;

	//Init the GL Engine
	init_gles(Engine::app, &Engine::openGLEngine.display, &Engine::openGLEngine.surface, &Engine::openGLEngine.context);

	Engine::openGLEngine.width = ANativeWindow_getWidth(Engine::app->window);
	Engine::openGLEngine.height = ANativeWindow_getHeight(Engine::app->window);


	LOG("Engine Started");
    return 0;
}

int Engine::Destroy(){
	LOG("Ending Engine");
	eglDestroySurface(Engine::openGLEngine.display, Engine::openGLEngine.surface);
	eglDestroyContext(Engine::openGLEngine.display, Engine::openGLEngine.context);
	eglTerminate(Engine::openGLEngine.display);
	LOG("Engine Destoryed");
	return 0;

}

void Engine::Start() {bRun = true; Engine::tRun = std::thread(Engine::engine->Loop); }
void Engine::Resume() {bRun = true;}
void Engine::Pause() {bRun = false;}
void Engine::Finish() {bDestroy = true; bRun = false; if (Engine::tRun.joinable()) Engine::tRun.join();}

void Engine::process_CMD(struct android_app* app, int32_t cmd){
	switch (cmd) {
        case APP_CMD_INIT_WINDOW:
			Engine::Start();
			break;

        case APP_CMD_TERM_WINDOW:
            Engine::Finish();
            break;
    }

}

int32_t Engine::process_INPUT(struct android_app* app, struct AInputEvent* event){
	// This function will be set in app->onInputEvent
    int eventType = AInputEvent_getType(event);

    switch (eventType) {
        case AINPUT_EVENT_TYPE_MOTION: { // Touch event
            int action = AMotionEvent_getAction(event);
            if (action == AMOTION_EVENT_ACTION_DOWN || action == AMOTION_EVENT_ACTION_MOVE) {
                // Extract the touch coordinates
                int xE = AMotionEvent_getX(event, 0); // X coordinate of the touch
                int yE = AMotionEvent_getY(event, 0); // Y coordinate of the touch


				float x = float(xE)/ (float)Engine::openGLEngine.width;
				float y = float(yE)/ (float)Engine::openGLEngine.height;
                // Store touch event data atomically or with mutex
				InputEvent event{V2D{ x * 2.0f - 1.0f, -(y * 2.0f - 1.0f) }};
				Engine::touchEvent.AddElem(event);
                // Optionally, you can use atomic flags as well
                // touchEventOccurred.store(true, std::memory_order_release);

                return 1; // Indicate that the event has been handled
            }
            break;
        }

        case AINPUT_EVENT_TYPE_KEY: { // Key event
            int keyCode = AKeyEvent_getKeyCode(event);
            int action = AKeyEvent_getAction(event);
            if (action == AKEY_EVENT_ACTION_DOWN) {
                // Handle key press logic
                if (keyCode == AKEYCODE_BACK) {
                    // Handle back key
                }
            }
            break;
        }

        default:
            break;
    }

    return 0; // Default return, event not handled
}

Engine* Engine::engine = nullptr;
std::atomic<bool> Engine::bRun;
std::atomic<bool> Engine::bDestroy;
std::thread Engine::tRun;
android_app* Engine::app = nullptr;
OpenGLEngine Engine::openGLEngine;





extern "C" void android_main(struct android_app* state) {
	Engine::bDestroy = false;
	Engine::bRun = false;
	Engine::engine = &engine;
	Engine::app = state;
	Logger::getInstance().init(state->activity->externalDataPath);
	AssetIO::getInstance().init(state->activity->assetManager);
	LOGI("App started");

	state->onAppCmd = Engine::process_CMD;
	state->onInputEvent = Engine::process_INPUT;


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
	//Destroy all data;
	Engine::Finish();
}





