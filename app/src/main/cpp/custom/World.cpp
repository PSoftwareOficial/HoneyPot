#include "API.h"

bool World::debugKeyboard = true;




int World::Init()
{ 
    LOGI("STARTING WORLD");
    uiManager.Init();

    return 0;
}




int World::InitGL() {
    textRenderer.InitGL();
    uiManager.InitGL();
    
    return 0;
}


int World::Update(uint64_t EuS, uint64_t TuS) {
    uiManager.Update();


    //Manage FPS counters
    FPS = 1000000.0f / float(EuS);
    minFPS = std::min(FPS, minFPS);
    m5FPS.push(FPS);
    return 0;
}

int World::Draw() {
        uiManager.Draw();
        textRenderer.DrawText(V2D{0.0f,0.0f},V2D{0.05f, 0.05f},"Hello GL \n This is nice!");
        
        
        textRenderer.DrawText(V2D{0.0f,Engine::openGLEngine.screenSize.y - 0.25f},V2D{0.05f, 0.05f},"FPS: " + std::to_string(FPS));
        textRenderer.DrawText(V2D{0.0f,Engine::openGLEngine.screenSize.y - 0.3f},V2D{0.05f, 0.05f},"mFPS: " + std::to_string(m5FPS.avg()));
        textRenderer.DrawText(V2D{0.0f,Engine::openGLEngine.screenSize.y - 0.35f},V2D{0.05f, 0.05f},"minFPS: " + std::to_string(minFPS));
        textRenderer.Draw();
        return 0;
    }

UIManager World::uiManager;
TextRenderer World::textRenderer;