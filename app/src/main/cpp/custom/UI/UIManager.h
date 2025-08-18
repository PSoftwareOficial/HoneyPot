#pragma once

#include "../../rendering/core_include.h"
#include "BaseUI.h"





class UIManager : public Object {
public:
    virtual int Init() override;
    virtual int InitGL() override {parent->InitGL(); return 0;}
    virtual int Update();
    virtual int Draw() override;

    shader_prog SHADER;
    std::shared_ptr<BaseUI> parent;

    void SETUP();
};
