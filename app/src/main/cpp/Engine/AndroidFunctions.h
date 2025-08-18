#include "Engine.h"




void Engine::process_CMD(struct android_app* app, int32_t cmd){
	switch (cmd) {
        case APP_CMD_START:
            Engine::Start();
            break;
        case APP_CMD_INIT_WINDOW:
            Engine::Start();  // set up EGL/Vulkan and begin rendering
            break;

        case APP_CMD_TERM_WINDOW:
            Engine::Finish(); // stop rendering and free window-specific resources
            break;

        case APP_CMD_GAINED_FOCUS:
            Engine::Resume(); // resume sensors, audio, input
            break;

        case APP_CMD_LOST_FOCUS:
            Engine::Pause();  // pause sensors, audio
            break;

        case APP_CMD_CONFIG_CHANGED:
            Engine::ReloadConfig(); // handle orientation, dpi, etc.
            break;

        case APP_CMD_LOW_MEMORY:
            Engine::TrimMemory(); // free caches
            break;

        case APP_CMD_RESUME:
            Engine::OnResume();
            break;

        case APP_CMD_PAUSE:
            Engine::OnPause();
            break;

        case APP_CMD_DESTROY:
            Engine::Shutdown();
            break;

        default:
            break;
    }
}

static int32_t process_INPUT(struct android_app* app, struct AInputEvent* event){
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
				Engine::touchEvents.AddElem(event);
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





void Loop();