#pragma once
#include "Keyboard.h"
#include "World/World.h"
#include "rendering/text/text.h"
#include "utilities/log/API.h"


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
    glPos =  V2D{0.0f, -factor.y + size.y / 2.0f + Engine::systemData.navBarHeightPx / };

    //Calculate the position of the upper left key
    V2D initalButtonPos = V2D{glPos.x - size.x / 2.0f + buttonSize.x / 2.0f, glPos.y + size.y / 2.0f - buttonSize.y / 2.0f + Engine::systemData.glNavBarHeight};

    
    
    dbg_FL("Starting Keys Positions for " + std::to_string(keys.size()) + " keys");

    for(int8_t i = keys.size() - 1; i > -1; --i){
        Key& key = keys[i];
        if(i < 20){
            key.pos = initalButtonPos + V2D{buttonSize.x * float((i % 10)), -buttonSize.y * float((i / 10))}; 
        }
        else if(i < 29){
            key.pos = initalButtonPos + V2D{buttonSize.x / 2.0f + buttonSize.x * float(i - 20), -buttonSize.y * 2.0f}; 
        }
        else {
            key.pos = initalButtonPos + V2D{buttonSize.x * 3.0f / 2.0f + buttonSize.x * float(i - 29), -buttonSize.y * 3.0f}; 
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
        World::textRenderer.DrawChar(e.pos,buttonSize, e.c);
    }
    return 0;
}



int KeyBoard::UIEvent(const InputEvent& event) {
    //Check if this container was touched
        if(coord.x > glPos.x - size.x/2.0f && coord.x < glPos.x + size.x/2.0f
        && coord.y > glPos.y - size.y/2.0f && coord.y < glPos.y + size.y/2.0f
        ){
            //Find the pressed key.
            constexpr float deltaDist = 0.01;

            for(auto& e: keys){
                if(coord.x - deltaDist > e.pos.x - buttonSize.x/2.0f && coord.x + deltaDist < e.pos.x + buttonSize.x/2.0f
                && coord.y - deltaDist > e.pos.y - buttonSize.y/2.0f && coord.y + deltaDist < e.pos.y + buttonSize.y/2.0f
                ){
                    inputedText.append(1,e.c);
                    return 1;
                }
            }
        }
        //Not found
            return 0;
}