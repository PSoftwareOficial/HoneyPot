#pragma once
#include "Keyboard.h"
#include "../API.h"
#include "../../rendering/text/text.h"
#include "../../utilities/log/API.h"


#define dbg_FL(msg) if(World::debugKeyboard) LOGI_FL(msg)
#define dbg(msg) if(World::debugKeyboard) LOGI(msg)

int KeyBoard::Init() {

    //Ensure keys are empty;
    keys.clear();

    //Setup the data
    constexpr char KeyLength[] = "1234567890qwertyuiopasdfghjklzxcvbnm";

    //Reduce count by one due to end char \0
    for(uint8_t i = 0; i < 36; ++i){
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
    dbg_FL("Finished with "+ std::to_string(keys.size()) + " keys");

    size = V2D{10 * buttonSize.x, 4 * buttonSize.y};

    return 0;
}
int KeyBoard::InitGL() {

    dbg_FL("Starting");

    //Calculate the gl data sizes.
    //This is to ensure, that we have consitent position and size
    V2D factor {1.0f , 1.0f / Engine::openGLEngine.yAspect};
    glButtonSize = buttonSize * factor;
    glSize = size * factor;
    glPos = pos * factor;

    //Calculate the position of the upper left key
    V2D initalButtonPos = V2D{glPos.x - glSize.x / 2.0f + glButtonSize.x / 2.0f, glPos.y + glSize.y / 2.0f - glButtonSize.y / 2.0f};

    
    
    dbg_FL("Starting Keys Positions for " + std::to_string(keys.size()) + " keys");

    for(int8_t i = keys.size() - 1; i > -1; --i){
        Key& key = keys[i];
        if(i < 20){
            key.pos = initalButtonPos + V2D{glButtonSize.x * float((i % 10)), -glButtonSize.y * float((i / 10))}; 
        }
        else if(i < 29){
            key.pos = initalButtonPos + V2D{glButtonSize.x / 2.0f + glButtonSize.x * float(i - 20), -glButtonSize.y * 2.0f}; 
        }
        else {
            key.pos = initalButtonPos + V2D{glButtonSize.x * 3.0f / 2.0f + buttonSize.x * float(i - 29), -glButtonSize.y * 3.0f}; 
        }
    }

    if(World::debugKeyboard){
        for(auto& key : keys){
            LOGI("key " + std::string(1,key.c) + " has position " + key.pos.string());
        }
    }

    dbg_FL("Finished Keys Positions");
    return 0;
}
int KeyBoard::Draw(){
    for(auto& e : keys){
        World::textRenderer.DrawChar(e.pos,currButtonSize, e.c);
    }
    return 0;
}
int KeyBoard::Update(uint64_t EuS, uint64_t TuS) {}