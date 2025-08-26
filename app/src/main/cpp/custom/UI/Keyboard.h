#pragma once
#include "../Object.h"
#include "../../utilities/math/structs/vxd.h"
#include <string>
#include <vector>



struct Key {
    char c;
    V2D pos;
    enum {KEY, ACTION} type;
};

class KeyBoard : public Object {
public:
    virtual int Init() override;
    virtual int InitGL() override;
    virtual int Draw() override;
    virtual int Update(uint64_t EuS, uint64_t TuS) override;

    
    V2D size {2.0f, 1.0f};
    V2D pos {0.0f, -0.4f};



    //UI related data
    std::string inputedText;
    V2D buttonSize = {0.2f, 0.3f};


    //Data
    std::vector<Key> keys;
};