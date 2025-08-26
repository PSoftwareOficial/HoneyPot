#pragma once
#include <EGL/egl.h>
#include <GLES3/gl32.h>
#include "../shaders/shaders.h"
#include "../../utilities/math/structs/vxd.h"

class TextRenderer{
    public:


    void InitGL();
    void DrawText(V2D Pos, V2D TextSize, const std::string& text);
    void Draw();
    private:
    GLuint VAO, VBO, EBO, instPosVBO, instSizeVBO,instIdxVBO, TEX;
    V2D instPos[1000];
    V2D instSize[1000];
    int instIdx[1000];
    shader_prog SHADER;
    void UpdateData();

    uint32_t currCharCount = 0;
};