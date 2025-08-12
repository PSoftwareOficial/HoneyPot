#pragma once
#include <string>
#include <vector>

#include "../../Engine.h"


#include "../../utilities/math/structs/mxt.h"
#include "../Object/Hex.h"


//Rendering
#include "../../rendering/core_include.h"


class Map {
    public:
    void Init(uint32_t hexLayers);
    void ChangeInstanceCount(uint16_t newCount);
    void Draw(const M4& uViewProj);

    Engine* engine;

private:
    int nHexLayers = 0;
    std::vector<Hex> stoHexes;

    //Rendering
    shader_prog SHADER;
    
    GLuint TEX_ARRAY = 0;
    uint16_t nTexInArray = 4;

    GLuint VAO, VBO_VEX, IBO; 
    GLuint VBO_MAT = 0;
    std::vector<M4> stoMatrix;
    GLuint VBO_TEX_IDX;
    std::vector<uint16_t> stoTEXIDX;

    void AddMesh();

    void AddTextures();
};
