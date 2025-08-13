#pragma once

#include "../../rendering/core_include.h"
#include "BaseUI.h"





class UIManager{
public:
    void Init();

    void Update();

    void Draw();
    shader_prog SHADER;

    std::shared_ptr<BaseUI> parent;

    void SETUP();
};
