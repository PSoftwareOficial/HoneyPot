#include <chrono>
#include "Engine.h"




void Engine::process_CMD(struct android_app* app, int32_t cmd){
	switch (cmd) {
        case APP_CMD_START:
            bRun = true;
            break;
        case APP_CMD_INIT_WINDOW:
            Engine::bRender = true;  // set up EGL/Vulkan and begin rendering
            LOGI("CMD: Render true");
            break;

        case APP_CMD_TERM_WINDOW:
            Engine::bRender = false; // stop rendering and free window-specific resources
            LOGI("CMD: Render false");
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



static const float dSlashThreshold = 0.04f; //In Rect GL dimensions
static const int dTimeThreshold = 30; //ms


int32_t Engine::process_INPUT(struct android_app* app, struct AInputEvent* event){

    //Save a static implementation of the input event. Does therefore not allow multiple engines.
    //Since the current InputEvent is only relevant for this function.
    static InputEvent inputEvent;

	// Get the type of event
    int eventType = AInputEvent_getType(event);

    switch (eventType) {
        case AINPUT_EVENT_TYPE_MOTION: { // Touch event
            //Get the specific action
            int action = AMotionEvent_getAction(event) & AMOTION_EVENT_ACTION_MASK;

            if(action == AMOTION_EVENT_ACTION_DOWN) {
                //Overwrite any current data, since a non reseted value would mean previous input was missed.
                inputEvent.start.time = std::chrono::steady_clock::now();

                // Extract the touch coordinates
                int xE = AMotionEvent_getX(event, 0); // X coordinate of the touch
                int yE = AMotionEvent_getY(event, 0); // Y coordinate of the touch
                float x = float(xE)/ (float)Engine::openGLEngine.width;
				float y = float(yE)/ (float)Engine::openGLEngine.height;
                //Get the position in field coordinates
                V2D coord {x * 2.0f - 1.0f, -(y * 2.0f - 1.0f) * Engine::openGLEngine.screenSize.y };

                //Overwrite any current data, since a non reseted value would mean previous input was missed.
                inputEvent.start.coord = coord;
                inputEvent.bIsActive = 1;
                //Handled Key event
                return 1;
            }
            else if(action == AMOTION_EVENT_ACTION_UP || action == AMOTION_EVENT_ACTION_CANCEL){
                //Only allow active events to be terminated, since otherwise we have a problem
                if(inputEvent.bIsActive){
                    //Overwrite any current data, since a non reseted value would mean previous input was missed.
                    inputEvent.end.time = std::chrono::steady_clock::now();

                    // Extract the touch coordinates
                    int xE = AMotionEvent_getX(event, 0); // X coordinate of the touch
                    int yE = AMotionEvent_getY(event, 0); // Y coordinate of the touch
                    float x = float(xE)/ (float)Engine::openGLEngine.width;
                    float y = float(yE)/ (float)Engine::openGLEngine.height;
                    //Get the position in field coordinates
                    V2D coord {x * 2.0f - 1.0f, -(y * 2.0f - 1.0f) * Engine::openGLEngine.screenSize.y };

                    //Overwrite any current data, since a non reseted value would mean previous input was missed.
                    inputEvent.end.coord = coord;



                    //Configure the type of event
                    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(inputEvent.end.time - inputEvent.stard.time).count();
                    if((inputEvent.start.coord - inputEvent.end.coord).mag2() < dSlashThreshold && ms < dTimeThreshold){
                        inputEvent.type = InputEvent::CLICK;
                    } else {
                        inputEvent.type = InputEvent::SLASH;
                    }
                    inputEvent.bIsActive = 0;
                    Engine::touchEvents.AddElem(inputEvent);
                    //Handled Key event
                    return 1;
                }
                //Handled Key event
                return 0;
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