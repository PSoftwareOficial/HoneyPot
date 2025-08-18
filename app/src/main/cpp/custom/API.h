#pragma once
#include "../utilities/AssetIO/API.h"
#include "../rendering/2DRenderer/2DRenderer.h"
#include <vector>
#include "../Engine/Engine.h"
#include "UI/UIManager.h"
#include "Object.h"



class World : public Object {
    virtual int Init() { UIManager.Init();return 0;}
    virtual int InitGL() {UIManager.InitGL();return 0;}
    virtual int Update(uint64_t EuS, uint64_t TuS) {UIManager.Update();return 0;}
    virtual int Draw() {UIManager.Draw();return 0;}



    UIManager uiManager;
}; 
