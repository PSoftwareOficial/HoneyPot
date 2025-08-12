#pragma once
#include "../utilities/AssetIO/API.h"
#include "../rendering/2DRenderer/2DRenderer.h"
#include <vector>
#include "../Engine.h"
#include "UI/UIManager.h"

static UIManager uiManager;
void InitWorld(){
    LOG("Init World");
    uiManager.Init();
}



void UpdateWorld(uint64_t uElapsedMicros, uint64_t uTotalMicros) {
	uiManager.Draw();
}


void StopWorld() {



}
