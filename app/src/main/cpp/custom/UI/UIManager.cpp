#include "UIManager.h"
#include "Button.h"
#include "../../Engine/Engine.h"


void UIManager::SETUP(){

    auto container = std::make_shared<BaseUI>();
    container->Init(nullptr, {V2D{0.0f,0.0f},V2D{1.0f,1.0f}}, {V3Du8{0,255,0}, 0.6f});
    parent = container;

    auto button1 = std::make_shared<Button>();
    button1->Init(parent,{V2D{0.0f,0.25f},V2D{1.0f,0.5f}}, {V3Du8{50,100,0}, 1.0f});
    parent->children.push_back(button1);

    auto button12 = std::make_shared<Button>();
    button12->Init(parent,{V2D{0.0f,-0.25f},V2D{1.0f,0.5f}}, {V3Du8{50,100,0}, 1.0f});
    parent->children.push_back(button12);

}







static const char* vertexShaderSrc =
    	R"(#version 320 es

        layout(location = 0) in vec3 aPos;    // vertex position
        layout(location = 1) in vec4 aColor;  // vertex color

        out vec4 vColor;

        void main() {
            gl_Position = vec4(aPos.x,aPos.y,aPos.z, 1.0);
            vColor = aColor;
        })";

static const char* fragmentShaderSrc = R"(#version 320 es
    precision mediump float;

    in vec4 vColor;

    out vec4 fragColor;

    void main() {   
        
        fragColor = vColor;
    })";


   int UIManager::Init() 
    {
        SETUP();
        keyboard.init();
        LOGI("UI MANAGER SET UP");
        return 0;
    }


    int UIManager::InitGL()
    {   
        SHADER.Init(vertexShaderSrc,fragmentShaderSrc);
        parent->InitGL();
        return 0;
    }

    int UIManager::Update(){
        while(Engine::touchEvents.GetElemNum()){
            Engine::InputEvent event;
            Engine::touchEvents.PopElem(event);
            parent->Touch(event.coord);
        }
        return 0;
    }

    int UIManager::Draw(){
        glUseProgram(SHADER.program);
        GLCheck("Program Selection");

        parent->Draw();

        keyboard.Draw();
        return 0;
    } 



