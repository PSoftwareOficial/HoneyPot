#pragma once
#include "../utilities/AssetIO/API.h"
#include "../utilities/log/API.h"
#include "../rendering/2DRenderer/2DRenderer.h"
#include <vector>
#include "../Engine/Engine.h"
#include "UI/UIManager.h"
#include "Object.h"
#include "../rendering/core_include.h"


class World : public Object {
    public:
    virtual int Init() { LOGI("STARTING WORLD"); uiManager.Init();return 0;}
    virtual int InitGL();
    virtual int Update(uint64_t EuS, uint64_t TuS);
    virtual int Draw() {uiManager.Draw();return 0;}


    virtual ~World() {};
    UIManager uiManager;
    TextRenderer textRenderer;
}; 
