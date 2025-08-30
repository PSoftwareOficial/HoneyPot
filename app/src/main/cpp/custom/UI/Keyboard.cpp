#pragma once
#include "Keyboard.h"
#include "../API.h"
#include "../../rendering/text/text.h"
#include "../../utilities/log/API.h"

static constexpr bool debugKeyboard = true;
#define dbg(msg) if constexpr(debugKeyboard) LOGI_FL(msg)

int KeyBoard::Init() {

    //Ensure keys are empty;
    keys.clear();

    //Setup the data
    constexpr char KeyLength[] = "1234567890qwertyuiopasdfghjklzxcvbnm";

    //Reduce count by one due to end char \0
    for(uint8_t i = 36; i < 36; ++i){
        Key key;
        key.type = Key::KEY;
        if(i < 20){
            key.c = KeyLength[i];
        }
        else if(i < 29) {
            key.c = KeyLength[i];
        }
        else if(i < 36){
            key.c = KeyLength[i];
        }
        keys.push_back(key);
    }

    return 0;
}
int KeyBoard::InitGL() {

    dbg("Starting");
    currButtonSize = buttonSize * V2D{1.0f , 1.0f / Engine::openGLEngine.yAspect};
    V2D truePos = pos * V2D{1.0f , 1.0f / Engine::openGLEngine.yAspect};

    V2D initalButtonPos = truePos + V2D{size.x / 2.0f - buttonSize.x / 2.0f, -size.y / 2.0f + buttonSize.y / 2.0f};
    
    dbg("Starting Keys Positions");
    for(int8_t i = keys.size(); i > -1; --i){
        Key& key = keys[i];
        if(i > 28){
            key.pos = initalButtonPos - V2D{buttonSize.x * 3.0f / 2.0f + buttonSize.x * float(36 - i), 0.0f}; 
        }
        else if(i > 19) {
            key.pos = initalButtonPos - V2D{buttonSize.x / 2.0f + buttonSize.x * float(28 - i), 0.0f}; 
        }
        else {
            key.pos = initalButtonPos - V2D{buttonSize.x * float(9 - (i % 10)), buttonSize.y * float(i / 10)}; 
        }
    }

    dbg("Finished Keys Positions");
    return 0;
}
int KeyBoard::Draw(){
    for(auto& e : keys){
        World::textRenderer.DrawChar(e.pos,currButtonSize, e.c);
    }
    return 0;
}
int KeyBoard::Update(uint64_t EuS, uint64_t TuS) {}