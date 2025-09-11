#include "World.h"
#include "utilities/AssetIO/API.h"
#include "utilities/log/API.h"
#include "Engine/UI/UIManager.h"
#include "rendering/core_include.h"
#include "Engine/Main/Engine.h"

World::World(){
    
}


int World::Init()
{ 
    LOGI("STARTING WORLD");
    Engine::uiManager->Init();

    for(auto& e: vObjects){
        e->Init();
    }
    
    return 0;
}




int World::InitGL() {
    Engine::textRenderer->InitGL();
    

    for(auto& e: vObjects){
        e->InitGL();
    }
    return 0;
}


int World::Update(uint64_t EuS, uint64_t TuS) {
    Engine::uiManager->Update(EuS, TuS);


    for(auto& e: vObjects){
        e->Update(EuS, TuS);
    }

    //Manage FPS counters
    FPS = 1000000.0f / float(EuS);
    m5FPS.push(FPS);
    return 0;
}

int World::Draw() {

    for(auto& e: vObjects){
        e->Draw();
    }

    Engine::textRenderer->DrawText(V2D{0.0f,0.0f},V2D{0.05f, 0.05f},"Hello GL \n This is nice!");
    Engine::textRenderer->DrawText(V2D{0.0f,Engine::openGLEngine.screenSize.y - 0.25f},V2D{0.1f, 0.1f},"FPS: " + std::to_string(FPS));
    Engine::textRenderer->DrawText(V2D{0.0f,Engine::openGLEngine.screenSize.y - 0.35f},V2D{0.1f, 0.1f},"mFPS: " + std::to_string(m5FPS.avg()));
    Engine::textRenderer->Draw();
    return 0;
}