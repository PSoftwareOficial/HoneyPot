#include "Button.h"


void Container::Init(V2D PosParam,V2D SizeParam,V3Du8 ColorParam, float AlphaParam) {
    pos = PosParam;
    size = SizeParam;
    color = ColorParam;
    alpha = alpha;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind VAO first
    glBindVertexArray(VAO);

    // Bind and fill VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    float data[28];
    GetGLData(data);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_DYNAMIC_DRAW);

    // Tell OpenGL how to interpret vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind for safety
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
};

void Container::Draw() {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
};


void Container::Touch(V2D coord){

    //Check if this container was touched
    if(coord.x > pos.x - size.x/2.0f && coord.x < pos.x + size.x/2.0f
    && coord.y > pos.y - size.y/2.0f && coord.y < pos.y + size.y/2.0f
    ){
        for(auto &e : children){
            e->Touch(coord);
        }

    } else
    //Return and do nothing
        return;
}


void Container::GetGLData(float (&data)[28]){
V2D vertices[4] =  
    {pos - size/2.0f, 
    pos + V2D{ - size.x/2.0f, + size.y/2.0f}, 
    pos + V2D{ + size.x/2.0f, - size.y/2.0f} , 
    pos + size/2.0f};

    const int nElem = 7;
    for(uint8_t i = 0; i < 4; ++i){
        data[i * nElem] = vertices[i].x;data[i * nElem + 1] = vertices[i].y;data[i * nElem + 2] = 0.0;
        data[i * nElem + 3] = color.x / 255.0f;data[i * nElem + 4] = color.y / 255.0f;data[i * nElem + 5] = color.z / 255.0f;data[i * nElem + 6] = alpha;
    }
}