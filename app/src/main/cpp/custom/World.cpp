#include "API.h"

int World::InitGL() {
    
    uiManager.InitGL();
    textRenderer.InitGL();
    return 0;
}


int World::Update(uint64_t EuS, uint64_t TuS) {
    uiManager.Update();
    return 0;
}

int World::Draw() {
        uiManager.Draw();
        textRenderer.DrawText(V2D{-0.5f,0.5f},V2D{0.1f, 0.1f},"Hello GL");
        return 0;
    }