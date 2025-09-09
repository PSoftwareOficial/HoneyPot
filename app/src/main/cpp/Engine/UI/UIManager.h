#pragma once
#include <cstdint>
#include <vector>
#include <memory>
#include "../../utilities/math/structs/vxd.h"


struct InputEvent{
    V2D coord;
    enum {TOUCH_DOW, TOUCH_UP} type;
};


class BaseUI {
    //The z value defines the priority at input handling
    float fZValue = 0.0f;
    bool bConsumeInput = true;
    bool bIsActive = true;
    class UIManager* pUImanager = nullptr;

    BaseUI();
    virtual int UIEvent(const InputEvent& event);
    virtual ~BaseUI();
};


class UIManager {
    public:
    int Init();
    int Update(uint64_t EuS, uint64_t TuS);
    int Destroy();
    int RegisterElement(BaseUI* elem);
    int UnregisterElement(BaseUI* elem);

    protected:
    std::vector<BaseUI*> vElements;


};