#pragma once
#include "../utilities/AssetIO/API.h"
#include "../rendering/2DRenderer/2DRenderer.h"
#include <vector>
#include "../Engine.h"
#include "custom/UI/UIManager.h"

static UIManager uiManager;
void InitWorld(){
    uiManager.Init();
}



void UpdateWorld(uint64_t uElapsedMicros, uint64_t uTotalMicros) {
	uiManager.Draw();
}


void StopWorld() {



}
