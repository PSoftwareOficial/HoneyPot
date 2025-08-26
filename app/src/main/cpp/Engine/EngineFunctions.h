#include "Engine.h"
#include <vector>

#include "../rendering/core_include.h"
#include "../custom/API.h"
#include "../utilities/log/API.h"


//World pointer
static std::unique_ptr<World> world;


void Engine::Loop(){
    LOGI("STARTING ENGINE THREAD");
    //Wait for the signal to init Open GL
    //Init the Engine
	Init();

    LOGI("FINISHED INIT ENGINE");
    while(!bRender.load()){
        //Not destroyed nor running, sleep for 100 milliseconds to reduce CPU time
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    InitGL();
    LOGI("FINISHED INIT GL ENGINE");
    bStarted = true;
	

    // Save the starting time point
	auto timeStartEngine = std::chrono::system_clock::now();
    //Ensure no destruction before the time
    while(!bDestroy.load()){
        auto timePrevFrame = std::chrono::system_clock::now();
        //Engine runs
        while(bRun.load()){
            //Engine renders?
            if(bRender.load()){
                if(!bRendering.load()){
                    //There was a change, implement it
                    ResumeGL();
                }

                //For now, only update when rendering, but allow different options in the future. To be implemented with the
                //bRender atomic
                auto timeFrame = std::chrono::system_clock::now();
                Update(std::chrono::duration<uint64_t, std::micro>(timeFrame - timePrevFrame).count(),std::chrono::duration<uint64_t, std::micro>(timeFrame - timeStartEngine).count());
                timePrevFrame = timeFrame;

            } else
                if(bRendering.load()){
                    //There was a change, implement it
                    PauseGL();
                }
        }
        //Not destroyed nor running, sleep for 1 seconds to reduce CPU time
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    //Destory everything
    EndGL();
    Destroy();
}









//Function which starts the complete engine
int Engine::Init(){
    LOG("STARTING ENGINE");
    world = std::make_unique<World>();
    world->Init();
    return 0;
}

static void init_gles(struct android_app* app, EGLDisplay* display, EGLSurface* surface, EGLContext* context){
	    *display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        eglInitialize(*display, NULL, NULL);

       	const EGLint configAttribs[] = {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_DEPTH_SIZE, 0,
        EGL_NONE
        };

        EGLConfig config;
        EGLint numConfigs;
        eglChooseConfig(*display, configAttribs, &config, 1, &numConfigs);

        EGLint format;
        eglGetConfigAttrib(*display, config, EGL_NATIVE_VISUAL_ID, &format);
        ANativeWindow_setBuffersGeometry(app->window, 0, 0, format);

        *surface = eglCreateWindowSurface(*display, config, app->window, NULL);

        EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE };
        *context = eglCreateContext(*display, config, EGL_NO_CONTEXT, contextAttribs);

        eglMakeCurrent(*display, *surface, *surface, *context);
}

//Function which inits the complete open GL
int Engine::InitGL(){
    LOG("Starting Open GL");
    //Init the GL Engine
    
    init_gles(Engine::app, &Engine::openGLEngine.display, &Engine::openGLEngine.surface, &Engine::openGLEngine.context);
    eglQuerySurface(Engine::openGLEngine.display,Engine::openGLEngine.surface,EGL_WIDTH,&Engine::openGLEngine.width);
    eglQuerySurface(Engine::openGLEngine.display,Engine::openGLEngine.surface,EGL_HEIGHT,&Engine::openGLEngine.height);
    Engine::openGLEngine.yAspect = Engine::openGLEngine.width / Engine::openGLEngine.height;
    LOGI("NEW ASPECT RATIO: " + std::to_string(Engine::openGLEngine.yAspect));
    bRendering.store(true);
    world->InitGL();
    return 0;
}


//Function which updates the complete engine (next frame)
int Engine::Update(uint64_t EuS, uint64_t TuS){
    
    world->Update(EuS, TuS);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // Default alpha blending function
    world->Draw();

    eglSwapBuffers(Engine::openGLEngine.display, Engine::openGLEngine.surface);
    return 0;
};

//Function which Resumes GL
int Engine::ResumeGL(){
    LOGI("RESUME GL");

    const EGLint configAttribs[] = {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_DEPTH_SIZE, 0,
        EGL_NONE
        };

        EGLConfig config;
        EGLint numConfigs;
        eglChooseConfig(Engine::openGLEngine.display, configAttribs, &config, 1, &numConfigs);
        EGLint format;
        eglGetConfigAttrib(Engine::openGLEngine.display, config, EGL_NATIVE_VISUAL_ID, &format);
        ANativeWindow_setBuffersGeometry(Engine::app->window, 0, 0, format);

    Engine::openGLEngine.surface = eglCreateWindowSurface(Engine::openGLEngine.display, config, Engine::app->window, NULL);
    eglMakeCurrent(Engine::openGLEngine.display, Engine::openGLEngine.surface, Engine::openGLEngine.surface, Engine::openGLEngine.context);
    eglQuerySurface(Engine::openGLEngine.display,Engine::openGLEngine.surface,EGL_WIDTH,&Engine::openGLEngine.width);
    eglQuerySurface(Engine::openGLEngine.display,Engine::openGLEngine.surface,EGL_HEIGHT,&Engine::openGLEngine.height);
    Engine::openGLEngine.yAspect = Engine::openGLEngine.width / Engine::openGLEngine.height;
    LOGI("NEW ASPECT RATIO: " + std::to_string(Engine::openGLEngine.yAspect));
    bRendering.store(true);
    world->ResumeGL();
    return 0;
}

//Function which Pauses GL for updates
int Engine::PauseGL(){
    LOGI("PAUSE GL");
    eglDestroySurface(Engine::openGLEngine.display, Engine::openGLEngine.surface);
    bRendering.store(false);
    world->PauseGL();
    return 0;
}

//Function which ends everything associated with OpenGL
int Engine::EndGL(){
    LOGI("Endind Open GL");
    eglDestroySurface(Engine::openGLEngine.display, Engine::openGLEngine.surface);
    eglDestroyContext(Engine::openGLEngine.display, Engine::openGLEngine.context);
    eglTerminate(Engine::openGLEngine.display);
    world->EndGL();
    return 0;
}

//Function which destroys the complete Engine.
int Engine::Destroy(){
    LOGI("Endind Engine");
    world->Destroy();
    world.reset();
    return 0;
}