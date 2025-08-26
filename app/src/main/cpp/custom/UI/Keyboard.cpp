#pragma once
#include "Keyboard.h"
#include "../API.h"
#include "../../rendering/text/text.h"



int KeyBoard::Init() {
    //Setup the data
    constexpr char KeyLength[] = "1234567890qwertyuiopasdfghjklzxcvbnm";

    //Reduce count by one due to end char \0
    V2D initalButtonPos = pos - size / 2.0f + buttonSize / 2.0f;
    for(uint8_t i = 0; i < 36; ++i){
        Key key;
        key.type = Key::KEY;
        if(i < 20){
            key.pos = initalButtonPos + V2D{buttonSize.x * float(i % 10) , buttonSize.y * float(i / 10)}; 
            key.c = KeyLength[i];
        }
        else if(i < 29) {
            key.pos = initalButtonPos + V2D{buttonSize.x / 2.0f, 0.0f} + V2D{buttonSize.x * float(i % 10) , buttonSize.y * 2.0f}; 
            key.c = KeyLength[i];
        }
        else if(i < 36){
            key.pos = initalButtonPos + V2D{buttonSize.x * 3.0f / 2.0f, 0.0f} + V2D{buttonSize.x * float(i % 10) , buttonSize.y * 3.0f}; 
            key.c = KeyLength[i];
        }
        keys.push_back(key);
    }

    return 0;
}
int KeyBoard::InitGL();
int KeyBoard::Draw(){
    for(auto& e : keys){
        World::textRenderer.DrawText(e.pos,buttonSize, std::string(e.c));
    }
    return 0;
}
int KeyBoard::Update(uint64_t EuS, uint64_t TuS);