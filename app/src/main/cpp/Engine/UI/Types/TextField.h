#pragma once
#include "Engine/UI/UIManager.h"
#include "World/Object.h"
#include "utilities/math/structs/vxd.h"
#include <string>
#include <vector>


class TextField : public Object, public BaseUI {
public:
    virtual int Init() override;
    virtual int InitGL() override;
    virtual int Draw() override;
    virtual int UIEvent(const InputEvent& event) override;
    virtual int Update(uint64_t EuS, uint64_t TuS) override;

    V2D buttonSize = {0.2f, 0.3f};
    V2D pos {0.0f, 0.5f};
    V2D size;


    //UI related data
    std::string inputedText;
    
    V2D glSize;
    V2D glPos;
    V2D glButtonSize;

};