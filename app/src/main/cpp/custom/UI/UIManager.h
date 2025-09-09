#pragma once

#include "../../rendering/core_include.h"
#include "BaseUI.h"
#include "Keyboard.h"




class UIManager : public Object {
public:
    virtual int Init() override;
    virtual int Update();
    virtual int Draw() override;

    shader_prog SHADER;
    std::shared_ptr<BaseUI> parent;

    //A vector containing all registered UI Elements. Important consideration is, that all will receive the input signals in
    //order from the back.
    std::vector<std::shared_ptr<BaseUI>> parents;

    void SETUP();

    KeyBoard keyboard;
};
