#pragma once

#include "../../rendering/core_include.h"
#include "BaseUI.h"
#include "Button.h"
#include "../../Engine.h"


static const char* vertexShaderSrc =
    	R"(#version 320 es

        layout(location = 0) in vec3 aPos;    // vertex position
        layout(location = 1) in vec3 aColor;  // vertex color

        float yScreenAspect;

        out vec3 vColor;

        void main() {
            gl_Position = vec4(aPos.x,aPos.y * yScreenAspect,aPos.z, 1.0);
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
        yScreenAspect = glGetUniformLocation(SHADER.program, "yScreenAspect");
        GLCheck("Gettin Screen Aspect Location");
        auto button = std::make_shared<Button>();
        button->Init(V2D{0.0f,0.0f},V2D{0.3f,0.30f}, V3Du8{125,125,0});
        parent = button;
    }

    void Draw(){
        glUseProgram(SHADER.program);
        GLCheck("Program Selection");

        float screenAspect = (float)Engine::openGLEngine.width/ (float)Engine::openGLEngine.height;
        glUniform1f(yScreenAspect,screenAspect);
        GLCheck("Setting Screen Size");
        parent->Draw();
    }
    shader_prog SHADER;
    GLint yScreenAspect;

    std::shared_ptr<BaseUI> parent;
};
