#pragma once
#include "../utilities/AssetIO/API.h"
#include "../rendering/2DRenderer/2DRenderer.h"
#include <vector>
#include "../Engine/Engine.h"
#include "UI/UIManager.h"
#include "Object.h"



class World : public Object {
    public:
    virtual int Init() { uiManager.Init();return 0;}
    virtual int InitGL() {uiManager.InitGL();return 0;}
    virtual int Update(uint64_t EuS, uint64_t TuS) {uiManager.Update();return 0;}
    virtual int Draw() {uiManager.Draw();return 0;}


    virtual ~World() override;
    UIManager uiManager;
}; 
