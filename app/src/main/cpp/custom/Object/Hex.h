#pragma once
#include "../../utilities/math/structs/vxd.h"

class Hex {
public:

    V2D GetScreenCoord(float hexLen, float hexWidth);
    inline const uint32_t GetIdx() const {return idxLoc;}
    inline void SetIdx(uint32_t idx) {idxLoc = idx;}
    uint32_t level = 0;
private:
    uint32_t idxLoc = 0;
    
};




