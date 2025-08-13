#pragma once

#include "../../rendering/core_include.h"
#include "BaseUI.h"
#include "Button.h"
#include "../../Engine.h"


static const char* vertexShaderSrc =
    	R"(#version 320 es

        layout(location = 0) in vec3 aPos;    // vertex position
        layout(location = 1) in vec3 aColor;  // vertex color

        out vec3 vColor;

        void main() {
            gl_Position = vec4(aPos.x,aPos.y,aPos.z, 1.0);
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
        auto button = std::make_shared<Button>();
        button->Init(V2D{0.0f,0.0f},V2D{1.0f,1.0f}, V3Du8{125,125,0});
        parent = button;
    }

    void Update(){
        while(Engine::touchEvents.GetElemNum()){
            Engine::InputEvent event;
            Engine::touchEvents.PopElem(event);
            parent->Touch(event.coord);
        }
    }

    void Draw(){
        glUseProgram(SHADER.program);
        GLCheck("Program Selection");

        GLCheck("Setting Screen Size");
        parent->Draw();
    }
    shader_prog SHADER;

    std::shared_ptr<BaseUI> parent;
};
