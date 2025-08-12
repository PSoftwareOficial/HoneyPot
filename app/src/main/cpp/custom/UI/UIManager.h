#pragma once

#include "../../rendering/core_include.h"
#include "BaseUI.h"
#include "Button.h"
#include "../../Engine.h"


static const char* vertexShaderSrc =
    	R"(#version 320 es

        layout(location = 0) in vec3 aPos;    // vertex position
        layout(location = 1) in vec3 aColor;  // vertex color

        uniform vec2 uScreenSize;

        out vec3 vColor;

        void main() {
            gl_Position = vec4(aPos.x/uScreenSize.x * 2.0 - 1.0,aPos.y/uScreenSize.y * 2.0 - 1.0,aPos.z, 1.0);
            vColor = aColor;
        })";

const char* fragmentShaderSrc = R"(#version 320 es
    precision mediump float;

    in vec3 vColor;

    out vec4 fragColor;

    void main() {   
        
        fragColor = vec4(vColor.xyz, 1.0);
    })";

class UIManager{
public:
    void Init() 
    {
        SHADER.Init(vertexShaderSrc,fragmentShaderSrc);
        screenSizeLoc = glGetUniformLocation(SHADER.program, "uScreenSize");
        auto button = std::make_shared<Button>();
        button->Init(V2D{0.0f,0.0f},V2D{200.0f,200.0f}, V3Du8{125,125,0});
        parent = button;
    }

    void Draw(){
        glUseProgram(SHADER.program);
        GLCheck("Program Selection");

        glUniform2f(screenSizeLoc,Engine::openGLEngine.width,Engine::openGLEngine.height);
        GLCheck("Setting Screen Size");
        parent->Draw();
    }
    shader_prog SHADER;
    GLint screenSizeLoc;

    std::shared_ptr<BaseUI> parent;
};
