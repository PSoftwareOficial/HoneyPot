#include "text.h"
#include "../../utilities/AssetIO/API.h"
#include "../core_include.h"



// Vertex data for a square
float vertices[] = {
        // positions    // texture coords
        -0.5f, -0.5f,   0.0f, 1.0f,
        0.5f, -0.5f,   1.0f, 1.0f,
        0.5f,  0.5f,   1.0f, 0.0f,
        -0.5f,  0.5f,   0.0f, 0.0f
    };

// Vertex and fragment shader source code
const char* vertexShaderSource = R"(#version 320 es
    layout(location = 0) in vec2 aPos;
    layout(location = 1) in vec2 aTexCoord;   // Quad texture coordinates
    layout(location = 2) in vec2 charPos;       //Position of the character
    layout(location = 3) in int texIdx;         //texture idx, basically the character


    uniform vec2 uCharSize; // Size of all characters (constant for the whole batch)

    out vec2 UV;

    #define ATLAS_NUM_X 16
    #define ATLAS_NUM_Y 6
    #define CHAR_TEX_SIZE_X 0.0625
    #define CHAR_TEX_SIZE_Y 0.1666666666666667

    void main() {

        // Calculate the row and column of the character in the texture atlas
        int row = texIdx / ATLAS_NUM_X;  // Row in the atlas
        int col = texIdx % ATLAS_NUM_X;  // Column in the atlas

        // Calculate the UV coordinates for the character in the atlas
        fragUV = aTexCoord * vec2(CHAR_TEX_SIZE_X, CHAR_TEX_SIZE_Y) + vec2(float(col) * CHAR_TEX_SIZE_X, float(row) * CHAR_TEX_SIZE_Y);

        UV = aTexCoord;
        gl_Position = vec4(charPos + aPos * uCharSize, 0.0, 1.0);
    })";

const char* fragmentShaderSource = R"(#version 320 es
    
    precision mediump float;
    in vec2 UV;
    out vec4 FragColor;

    uniform sampler2D uTexAtlas;  // Texture atlas containing all the characters

    void main() {
        FragColor = texture(uTexAtlas, UV);
    })";

void TextRenderer::InitGL(){
    SHADER.Init(vertexShaderSource, fragmentShaderSource);

    // Generate buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    // Upload vertex data to GPU
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Set vertex attribute pointers
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Vertex Texture Coordinate Attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(sizeof(float) * 2));
    glEnableVertexAttribArray(1);



    // Bind position VBO (per-instance)
    glBindBuffer(GL_ARRAY_BUFFER, instPosVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(instPos), instPos, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(V2D), (void*)0);
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1); // Instanced attribute

    // Bind index VBO (per-instance)
    glBindBuffer(GL_ARRAY_BUFFER, instIdxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(instIdx), instIdx, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(3, 1, GL_INT, GL_FALSE, sizeof(int), (void*)0);
    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1); // Instanced attribute


    glBindVertexArray(0); // Unbind VAO


    glGenTextures(1, &TEX);
    glBindTexture(GL_TEXTURE_2D, TEX);
    texture2D tex;
    if(GetTexture("images/glyphAtlas.png",tex)){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.width, tex.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.tex.get());
        GLCheck("glBufferData GlyphAtlas");
        LOGI("TEXT RENDERER LOADED TEXTURE ATLAS: " + std::to_string(tex.width) + " " + std::to_string(tex.height));
    }

    // Set texture parameters — very important!
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

}


void TextRenderer::DrawText(V2D Pos, V2D TextSize, const std::string& text){


    V2D currPos = Pos;
    int instI = 0;
    for(uint16_t i = 0; i < text.size() && instI < 100; ++i){
        if(text[i] == '\n'){
            currPos.y += TextSize.y;
        } else if( text[i] < 96 && text[i] > 32){
            instIdx[instI] = text[i] - 32;
            instPos[instI] = currPos;
            currPos.x += TextSize.x;
            ++instI;
        }
    }

    




    // Use the shader program
        glUseProgram(SHADER.program);
        GLCheck("Setting Program");

        UpdateData();
        // Draw the square
        glBindVertexArray(VAO);
        GLCheck("Setting VAO");

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TEX);
        glUniform1i(glGetUniformLocation(SHADER.program, "uTexAtlas"), 0);  // Set uniform to texture unit 0

        glUniform2f(glGetUniformLocation(SHADER.program, "uCharSize"), TextSize.x, TextSize.y);

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);  // Starting from vertex 0, drawing 4 vertices
        GLCheck("Drawing Elements");
        glBindVertexArray(0);
}




void TextRenderer::UpdateData(){
    // Bind index VBO (per-instance)
    glBindBuffer(GL_ARRAY_BUFFER, instIdxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(instIdx), instIdx, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, instPosVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(instPos), instPos, GL_DYNAMIC_DRAW);
}











/*static const char* textvSrc = R"(#version 320 es
#define ATLAS_NUM_X 16
#define ATLAS_NUM_Y 6
#define CHAR_SIZE_X 0.0625
#define CHAR_SIZE_Y 0.1666666666666667

precision mediump float;

layout(location = 0) in vec2 aPos;        // Quad vertex position
layout(location = 1) in vec2 aTexCoord;   // Quad texture coordinates
layout(location = 2) in vec2 instancePos; // Position of the character (per-instance)
layout(location = 3) in int instanceIndex; // Index of the character in the texture atlas (per-instance)

out vec2 fragUV; // UV to pass to fragment shader

uniform vec2 instanceSize; // Size of all characters (constant for the whole batch)

void main()
{
    // Compute the final position of the character (offset the quad position)
    vec2 finalPos = instancePos + aPos * instanceSize;

    // Calculate the row and column of the character in the texture atlas
    int row = instanceIndex / ATLAS_NUM_X;  // Row in the atlas
    int col = instanceIndex % ATLAS_NUM_X;  // Column in the atlas

    // Calculate the UV coordinates for the character in the atlas
    fragUV = aTexCoord * vec2(CHAR_SIZE_X, CHAR_SIZE_Y) + vec2(float(col) * CHAR_SIZE_X, float(row) * CHAR_SIZE_Y);

    // Output the final position of the quad
    gl_Position = vec4(finalPos, 0.0, 1.0);
}
)";
static const char* textvSrc = R"(#version 320 es
precision mediump float;

layout(location = 0) in vec2 aPos;        // Quad vertex position

void main()
{
    // Output the final position of the quad
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)";


static const char* textfSrc = R"(#version 320 es
precision mediump float;

in vec2 fragUV;  // Receive the UV from the vertex shader

uniform sampler2D utexAtlas;  // Texture atlas containing all the characters
out vec4 fragColor;

void main() {
    fragColor = texture(utexAtlas, fragUV);
}
)";
static const char* textfSrc = R"(#version 320 es
precision mediump float;

out vec4 fragColor;

void main() {
    fragColor = vec4(1.0,0.0,0.0,1.0);
}
)";


void TextRenderer::InitGL(){
    SHADER.Init(textvSrc, textfSrc);
    glUseProgram(SHADER.program);

    // Set up the quad geometry (a single quad per character)
    float quadVertices[] = {
        // positions    // texture coords
        -0.5f, -0.5f,   0.0f, 0.0f,
        0.5f, -0.5f,   1.0f, 0.0f,
        0.5f,  0.5f,   1.0f, 1.0f,
        -0.5f,  0.5f,   0.0f, 1.0f
    };

    // Set up the VAO, VBO, and EBO for instanced rendering
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    //glGenBuffers(1, &instPosVBO);
    //glGenBuffers(1, &instIdxVBO);

    // Bind VAO and VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    // Vertex Position Attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(quadVertices), (void*)0);
    glEnableVertexAttribArray(0);

    // // Vertex Texture Coordinate Attribute
    // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(quadVertices), (void*)(sizeof(float) * 2));
    // glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    unsigned int indices[] = { 0, 1, 2, 0, 2, 3 };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Bind position VBO (per-instance)
    glBindBuffer(GL_ARRAY_BUFFER, instPosVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(instPos), instPos, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(V2D), (void*)0);
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1); // Instanced attribute

    // Bind index VBO (per-instance)
    glBindBuffer(GL_ARRAY_BUFFER, instIdxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(instIdx), instIdx, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(3, 1, GL_INT, GL_FALSE, sizeof(int), (void*)0);
    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1); // Instanced attribute


        glGenTextures(1, &TEX);
        glBindTexture(GL_TEXTURE_2D, TEX);
        texture2D tex;
        if(GetTexture("images/glyphAtlasTest.png",tex)){
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.width, tex.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.tex.get());
            GLCheck("glBufferData GlyphAtlas");
            LOGI("TEXT RENDERER LOADED TEXTURE ATLAS: " + std::to_string(tex.width) + " " + std::to_string(tex.height));
        }

        // Set texture parameters — very important!
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
}


void TextRenderer::UpdateData(){
    // Bind index VBO (per-instance)
    glBindBuffer(GL_ARRAY_BUFFER, instIdxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(instIdx), instIdx, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, instPosVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(instPos), instPos, GL_DYNAMIC_DRAW);
}


void TextRenderer::DrawText(V2D Pos, V2D TextSize, const std::string& text){

    V2D currPos = Pos;
    int instI = 0;
    for(uint16_t i = 0; i < text.size() && instI < 100; ++i){
        if(text[i] == '\n'){
            currPos.y += TextSize.y;
        } else if( text[i] < 96 && text[i] > 32){
            instIdx[instI] = text[i] - 32;
            instPos[instI] = currPos;
            currPos.x += TextSize.x;
            ++instI;
        }
    }

    glUseProgram(SHADER.program);
    GLCheck("Setting Program TEXT");

    //UpdateData();
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, TEX);  // Assuming `atlasTextureID` is the texture handle of your atlas.
    //glUniform1i(glGetUniformLocation(SHADER.program, "utexAtlas"), 0);  // Set uniform to texture unit 0
    //GLCheck("Setting Texture Atlas");
    //glUniform2f(glGetUniformLocation(SHADER.program, "instanceSize"), TextSize.x, TextSize.y);  // Constant size for all characters
    //GLCheck("Setting Instance Size");
    // Render the characters
    glBindVertexArray(VAO);
    GLCheck("SEtting VAO");
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    GLCheck("Drawing");
}



*/