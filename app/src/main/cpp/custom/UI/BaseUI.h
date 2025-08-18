#pragma once
#include <vector>
#include <memory>

#include "../Object.h"
#include "../../utilities/math/structs/vxd.h"
#include "../../rendering/core_include.h"



struct UI_G_Data {
    V2D pos;
    V2D size;
};

struct UI_GL_Data {
    V3Du8 color;
    float alpha = 1.0f;

    GLuint VAO = 0;
    GLuint VBO = 0;
};


class BaseUI : public Object {
    public:
    virtual void Init(const UI_G_Data& gDataParam, const UI_GL_Data& glDataParam) {
        Init(gDataParam);
        glData = glDataParam;
        noGLData = false;
    }; 
    virtual void Init(const UI_G_Data& gDataParam) {
        //Position always in respect to parent
        gData.pos = gDataParam.pos * (parent ? (parent->gData.size / 2.0f) : V2D{1.0f, 1.0f}) + parent->gData.pos;
        gData.size = gDataParam.size * (parent ? (parent->gData.size / 2.0f) : V2D{1.0f, 1.0f});
    };

    virtual int InitGL() override {
        if(!noGLData){
            glGenVertexArrays(1, &glData.VAO);
            glGenBuffers(1, &glData.VBO);

            // Bind VAO first
            glBindVertexArray(glData.VAO);

            // Bind and fill VBO
            glBindBuffer(GL_ARRAY_BUFFER, glData.VBO);
            float data[28];
            RectGetGLData(data);
            glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_DYNAMIC_DRAW);

            // Tell OpenGL how to interpret vertex data
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            // Unbind for safety
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
        for(auto &e : children){
                e->InitGL();
        }
        return 0;
    }
    //Base touching mechanism
    virtual void Touch(V2D coord){
        //Check if this container was touched
        if(coord.x > gData.pos.x - gData.size.x/2.0f && coord.x < gData.pos.x + gData.size.x/2.0f
        && coord.y > gData.pos.y - gData.size.y/2.0f && coord.y < gData.pos.y + gData.size.y/2.0f
        ){
            for(auto &e : children){
                e->Touch(coord);
            }
        }
    }

    virtual int Draw() override {
        if(!noGLData){
            glBindVertexArray(glData.VAO);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }

        for(auto &e : children){
                e->Draw();
            }
        return 0;
    }
    virtual ~BaseUI() {};
    
    
    UI_G_Data gData;
    UI_GL_Data glData;
    bool noGLData = true;
    std::shared_ptr<BaseUI> parent;
    std::vector<std::shared_ptr<BaseUI>> children;


    void RectGetGLData(float (&data)[28]){
        V2D vertices[4] =  
            {gData.pos - gData.size/2.0f, 
            gData.pos + V2D{ - gData.size.x/2.0f, + gData.size.y/2.0f}, 
            gData.pos + V2D{ + gData.size.x/2.0f, - gData.size.y/2.0f} , 
            gData.pos + gData.size/2.0f};

        const int nElem = 7;
        for(uint8_t i = 0; i < 4; ++i){
            data[i * nElem] = vertices[i].x;data[i * nElem + 1] = vertices[i].y;data[i * nElem + 2] = 0.0;
            data[i * nElem + 3] = glData.color.x / 255.0f;data[i * nElem + 4] = glData.color.y / 255.0f;data[i * nElem + 5] = glData.color.z / 255.0f;data[i * nElem + 6] = glData.alpha;
        }
    }
};