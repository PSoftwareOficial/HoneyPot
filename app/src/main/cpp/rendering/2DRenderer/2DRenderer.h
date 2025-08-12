#pragma once
#include "../core_include.h"
#include "../../utilities/math/structs/vxd.h"
#include <vector>

struct D2Decal{
    std::vector<V2D> vertex;
    float color[4];
    enum {LINE, TRIS ,STRIP, FAN} type;
};

class D2Renderer {
    public:
    void Init();
    void DrawLine(V2D start, V2D end, uint32_t color);
    void DrawTris(V2D v1, V2D v2,V2D v3, uint32_t color);
    void DrawSquare(V2D pos, V2D size, uint32_t color);
    void Draw();

private:
    std::vector<D2Decal> decals;
    shader_prog SHADER;
    int uColorLoc; 
};

