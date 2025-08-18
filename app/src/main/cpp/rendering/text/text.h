#pragma once
#include "../core_include.h"
#include "../../utilities/math/structs/vxd.h"

class TextRenderer{
    public:


    void InitGL();
    void DrawText(V2D Pos, V2D TextSize, const std::string& text);

    private:
    GLuint VAO, VBO, EBO, instPosVBO, instIdxVBO, TEX;
    V2D instPos[100];
    int instIdx[100];
    shader_prog SHADER;
    void UpdateData();
};