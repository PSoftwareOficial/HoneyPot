#include "../rendering/core_include.h"


#pragma once

static const char* vDebugVSrc = R"(#version 320 es
precision mediump float;

layout(location = 0) in vec3 a_position;           // vertex position

uniform vec2 u_transition;
uniform vec2 u_scale;

void main() {
    gl_Position = vec4(u_scale, 1.0,1.0) * (vec4(u_transition, 0.0, 0.0) + vec4(a_position, 1.0));
}
)";


static const char* vDebugFSrc = R"(#version 320 es
precision mediump float;
uniform vec4 color;

out vec4 fragColor;

void main() {
    fragColor = color;
}
)";


struct vDebugDecal{
    enum {Tris, Square, Sphere} type;
    V2D pos {0.0f,0.0f}, 
    V2D scale {1.0f,1.0f},
    uint32_t color = 0xFF000000;
};

class VisualDebugging {
    void Init(){
        SHADER.Init(vDebugVSrc, vDebugFSrc);
        glUseProgram(SHADER.program);


         // 1. Hexagon mesh (shared per-vertex data)
        glGenVertexArrays(1, &VAO_Tris);
        glBindVertexArray(VAO_Tris);

        glGenBuffers(1, &VBO_VEX);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_VEX);
        //Generate the array
        //Make a vertex array
		float* vertices = new float[7 * 5]; 
		for(uint8_t i = 0; i < 7; ++i){
			//POS
			vertices[i * 5] = hex_base_vertices[i].x; vertices[i * 5 + 1] = hex_base_vertices[i].y;vertices[i * 5 + 2] = hex_base_vertices[i].z;
			vertices[i * 5 + 3] = hex_base_uv[i].x;vertices[i * 5 + 4] = hex_base_uv[i].y;
		}
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 7 * 5, vertices, GL_STATIC_DRAW);
        delete[] vertices;

        // Setup IBO
        glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * 3 * sizeof(uint8_t), hex_base_indices, GL_STATIC_DRAW);
        GLCheck("glBufferData IBO");
        // location = 0: vec3 a_position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    }


    private:
    shader_prog SHADER;

    unsigned int VAO_Tris;
    unsigned int VBO_Tris;
    unsigned int VAO_Square;
    unsigned int VBO_Square;
    unsigned int VBO
    unsigned int VAO_Sphere;

    std::vector<vDebugDecal> stoDebugDecals;

    


};