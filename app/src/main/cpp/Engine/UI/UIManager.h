#pragma once
#include <cstdint>
#include <vector>
#include <memory>
#include "../../utilities/math/structs/vxd.h"
#include <chrono>

struct InputAction {
    V2D coord;
    std::chrono::steady_clock::time_point time;
};

struct InputEvent{
    uint8_t bIsActive = 0;
    InputAction start;
    InputAction end;
    enum {CLICK, SLASH} type;
};


class BaseUI {
    public:
    //The z value defines the priority at input handling
    float fZValue = 0.0f;
    bool bConsumeInput = true;
    bool bIsActive = true;
    class UIManager* pUImanager = nullptr;

    BaseUI();
    virtual int UIEvent(const InputEvent& event) = 0;
    virtual ~BaseUI();
};


class UIManager {
    public:
    UIManager();
    int Init();
    int Update(uint64_t EuS, uint64_t TuS);
    int Destroy();
    int RegisterElement(BaseUI* elem);
    int UnregisterElement(BaseUI* elem);

    protected:
    std::vector<BaseUI*> vElements;
    std::shared_ptr<class KeyBoard> pKeyboard;

};