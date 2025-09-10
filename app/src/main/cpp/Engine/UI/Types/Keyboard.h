#pragma once
#include "Engine/UI/UIManager.h"
#include "World/Object.h"
#include "utilities/math/structs/vxd.h"
#include <string>
#include <vector>



struct Key {
    char c;
    V2D pos;
    enum {KEY, ACTION} type;
};

class KeyBoard : public Object, public BaseUI {
public:
    virtual int Init() override;
    virtual int InitGL() override;
    virtual int Draw() override;
    virtual int Update(uint64_t EuS, uint64_t TuS) override;
    virtual int UIEvent(const InputEvent& event) override;
    virtual int Touch(V2D coord);

    
    const V2D buttonSize = {0.2f, 0.3f};
    V2D pos;
    V2D size;


    //UI related data
    bool isActive = true;
    std::string inputedText;
    

    V2D glButtonSize;
    V2D glSize;
    V2D glPos;


    //Data
    std::vector<Key> keys;
};