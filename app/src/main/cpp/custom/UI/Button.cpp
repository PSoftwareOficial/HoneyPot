#include "Button.h"


void Button::Init(V2D PosParam,V2D SizeParam,V3Du8 ColorParam) {
    pos = PosParam;
    size = SizeParam;
    color = ColorParam;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind VAO first
    glBindVertexArray(VAO);

    // Bind and fill VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    float data[20];
    GetGLData(data);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_DYNAMIC_DRAW);

    // Tell OpenGL how to interpret vertex data
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind for safety
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
};

void Button::Draw() {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
};


void Button::Touch(V2D coord){
    color = color + V3Du8{(uint8_t)10,(uint8_t)10,(uint8_t)10};
    // Bind and fill VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    float data[20];
    GetGLData(data);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);
}


void Button::GetGLData(float (&data)[20]){
V2D vertices[4] =  
    {pos - size/2.0f, 
    pos + V2D{ - size.x/2.0f, + size.y/2.0f}, 
    pos + V2D{ + size.x/2.0f, - size.y/2.0f} , 
    pos + size/2.0f};

    const int nElem = 5;
    for(uint8_t i = 0; i < 4; ++i){
        data[i * nElem] = vertices[i].x;data[i * nElem + 1] = vertices[i].y;
        data[i * nElem + 2] = color.x / 255.0f;data[i * nElem + 3] = color.y / 255.0f;data[i * nElem + 4] = color.z / 255.0f;
    }
}