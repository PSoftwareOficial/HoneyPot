#include "API.h"

int World::InitGL() {
    
    //uiManager.InitGL();
    textRenderer.InitGL();
    return 0;
}


int World::Update(uint64_t EuS, uint64_t TuS) {
    //uiManager.Update();
    return 0;
}

int World::Draw() {
        //uiManager.Draw();
        textRenderer.DrawText(V2D{0.0f,0.0f},V2D{0.05f, 0.05f},"Hello GL \n This is nice!");
        return 0;
    }

UIManager World::uiManager;
TextRenderer World::textRenderer;