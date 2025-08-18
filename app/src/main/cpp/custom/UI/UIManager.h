#pragma once

#include "../../rendering/core_include.h"
#include "BaseUI.h"





class UIManager : public Object {
public:
    virtual void Init() override;
    virtual void InitGL() override {parent->InitGL()};
    virtual void Update() override;
    virtual void Draw() override;

    shader_prog SHADER;
    std::shared_ptr<BaseUI> parent;

    void SETUP();
};
