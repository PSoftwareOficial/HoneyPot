#pragma once

#include "BaseUI.h"

class Button : public BaseUI {
    public:
    virtual void Touch(V2D coord) override {
        //Check if this container was touched
        if(coord.x > gData.pos.x - gData.size.x/2.0f && coord.x < gData.pos.x + gData.size.x/2.0f
        && coord.y > gData.pos.y - gData.size.y/2.0f && coord.y < gData.pos.y + gData.size.y/2.0f
        ){
            glData.color = glData.color + V3Du8{(uint8_t)10,(uint8_t)10,(uint8_t)10};
            // Bind and fill VBO
            glBindBuffer(GL_ARRAY_BUFFER, glData.VBO);
            float data[28];
            RectGetGLData(data);
            glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_DYNAMIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER,0);

        } else
        //Return and do nothing
            return;
    }

    virtual ~Button() override {}
};