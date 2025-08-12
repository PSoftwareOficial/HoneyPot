#include "2DRenderer.h"

static const char* D2RendererVSrc = R"(#version 320 es
precision mediump float;

layout(location = 0) in vec2 a_position;           // vertex position

void main() {
    gl_Position = vec4(a_position,0.0, 1.0);
}
)";


static const char* D2RendererFSrc = R"(#version 320 es
precision mediump float;
uniform vec4 uColor;

out vec4 fragColor;

void main() {
    fragColor = uColor;
}
)";




void D2Renderer::Init(){
    SHADER.Init(D2RendererVSrc,D2RendererFSrc);
    uColorLoc  = glGetUniformLocation(SHADER.program, "uColor");
}

inline static void colorToVec4(uint32_t c, float* out) {
    const float inv255 = 1.0f / 255.0f;
    out[0] = ((c >> 24) & 0xFF) * inv255;
    out[1] = ((c >> 16) & 0xFF) * inv255;
    out[2] = ((c >> 8)  & 0xFF) * inv255;
    out[3] = (c         & 0xFF) * inv255;
}

void D2Renderer::DrawLine(V2D start, V2D end, uint32_t color){
    D2Decal dec { {start,end}, {0.0f,0.0f,0.0f,0.0f}, D2Decal::LINE };
    colorToVec4(color, dec.color);
    decals.push_back(dec);
}


void D2Renderer::DrawTris(V2D v1, V2D v2,V2D v3, uint32_t color){
    D2Decal dec { {v1,v2,v3}, {0.0f,0.0f,0.0f,0.0f}, D2Decal::TRIS };
    colorToVec4(color, dec.color);
    decals.push_back(dec);
}


void D2Renderer::DrawSquare(V2D pos, V2D size, uint32_t color){
    D2Decal dec { {pos - size / 2.0f,pos + V2D{- size.x / 2.0f, size.y/2.0f} ,pos + V2D{size.x / 2.0f, -size.y/2.0f} ,pos + size / 2.0f}, {0.0f,0.0f,0.0f,0.0f}, D2Decal::STRIP };
    colorToVec4(color, dec.color);
    decals.push_back(dec);
}

void D2Renderer::Draw(){
    glUseProgram(SHADER.program);
    for(const auto &e : decals){
        GLenum mode;
        switch (e.type) {
            case D2Decal::LINE: mode = GL_LINES; break;
            case D2Decal::STRIP: mode = GL_TRIANGLE_STRIP; break;
            case D2Decal::FAN:  mode = GL_TRIANGLE_FAN; break;
            case D2Decal::TRIS:  mode = GL_TRIANGLES; break;
            default: return; // invalid type
        }

        glUniform4fv(uColorLoc, 1, e.color);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, e.vertex.data());
        glEnableVertexAttribArray(0);

        glDrawArrays(mode, 0, e.vertex.size());
    }
    decals.clear();
}