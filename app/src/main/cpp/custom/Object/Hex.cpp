#include "Hex.h"
#include <cstdint>
#include <vector>



enum HexDir : uint8_t {
    E = 0,
    NE = 1,
    NW = 2,
    W = 3,
    SW = 4,
    SE = 5
};

static const V2Di HexNeighbors[6] {
    
    //East
    {2,0},
    //North East 
    {1,2},
    //Nort West
    {-1,2},
    //West
    {-2,0},
    //Sout West
    {-1,-2},
    //South East
    {1,-2},
    
    
};


static V2Di FromIdxToDouble(uint32_t idx){
    V2Di pos {0,0};
    if(idx == 0) return pos;

    uint32_t radius = 1;
    pos += HexNeighbors[SW];

    for(uint32_t i = 1; i < idx; ++i){
        for(uint8_t dir = E; dir <= SE && i < idx; ++dir){
            if(dir == SE){
                for(uint8_t sets = 0; sets < radius - 1 && i < idx; ++sets, ++i){
                pos += HexNeighbors[dir];
            }
            } else
            for(uint8_t sets = 0; sets < radius && i < idx; ++sets, ++i){
                pos += HexNeighbors[dir];
            }
        }

        //If a complete Loop was done
        if(i < idx){
            //Go directly south
            pos += V2D{0,-4};
        }
        ++radius;
    }
    return pos;
}

V2D Hex::GetScreenCoord(float hexLen, float hexWidth){
    V2Di pos = FromIdxToDouble(idxLoc);
    return V2D{hexWidth * (float)pos.x ,(hexWidth / 2.0f + hexLen) * (float)pos.y * 0.25f};
}

/*
void Hex::FromIdx(uint32_t idx){
    idxLoc = idx;
    if(idx == 0) {
        gridLoc = {0,0};
    } else {
        //Find the radius
        // radius * (radius + 1) >=  > (radius -1) * (radius) 
        uint16_t radius = 1;
        while(true){
            if( radius * (radius + 1) >= idx / 3 && idx / 3 > (radius - 1) * (radius)){
                break;
            } else 
                ++radius;
        }
        
        uint32_t startIdxOfRadius = LayerToTotalNum(radius);

        V2Di startLocOfRadius = radius % 2 ? V2Di{0, 2 * radius} : V2Di{0, 2 * (radius-1)} + HexNeighbors[HexDir::NE];

        uint32_t ringIdx = idx - startIdxOfRadius;
        
        std::vector<V2Di> nextDir;
        for(uint8_t i = 0; i < radius / 2; ++i){
            nextDir.push_back(HexNeighbors[E]);
        }
        for(uint8_t dir = SE; dir <= NW; ++dir){
            for(uint8_t i = 0; i < radius; ++i){
                nextDir.push_back(HexNeighbors[dir]);
            }
        }

        for(uint16_t i = 0; i < ringIdx; ++i){
            startLocOfRadius = startLocOfRadius + nextDir[i];
        }
        gridLoc = startLocOfRadius;
    }
}*/



