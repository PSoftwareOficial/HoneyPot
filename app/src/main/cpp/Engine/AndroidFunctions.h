#include "Engine.h"




void Engine::process_CMD(struct android_app* app, int32_t cmd){
	switch (cmd) {
        case APP_CMD_START:
            Engine::tRun = std::thread(&Engine::Loop, &Engine::engine);
            break;
        case APP_CMD_INIT_WINDOW:
            Engine::bRender = true;  // set up EGL/Vulkan and begin rendering
            bRun = true;
            break;

        case APP_CMD_TERM_WINDOW:
            Engine::bRender = false; // stop rendering and free window-specific resources
            bRun = false;
            break;

        case APP_CMD_GAINED_FOCUS:
            break;

        case APP_CMD_LOST_FOCUS:
            break;

        case APP_CMD_CONFIG_CHANGED:
            break;

        case APP_CMD_LOW_MEMORY:
            break;

        case APP_CMD_RESUME:
            break;

        case APP_CMD_PAUSE:
            break;

        case APP_CMD_DESTROY:
            Engine::bRun = false;
            Engine::bDestroy = true;
            break;

        default:
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