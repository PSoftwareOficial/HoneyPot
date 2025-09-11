#pragma once
#include "TextField.h"
#include "World/World.h"
#include "rendering/text/text.h"
#include "utilities/log/API.h"
#include "Engine/Main/Engine.h"
#include "Keyboard.h"


int TextField::Init() {

    pUImanager->RegisterElement(this);

    return 0;
}
int TextField::InitGL() {

    //Calculate the gl data sizes.
    //This is to ensure, that we have consitent position and size
    V2D factor {1.0f , 1.0f / Engine::openGLEngine.yAspect};
    glPos =  pos * factor;
    return 0;
}
int TextField::Draw(){
            Engine::textRenderer->DrawText(glPos,buttonSize, Engine::uiManager->pKeyboard->inputedText);
    return 0;
}



int KeyBoard::UIEvent(const InputEvent& event) {
            return 0;
}