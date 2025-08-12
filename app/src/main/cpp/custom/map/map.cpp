#include "map.h"

#include "../../utilities/math/structs/vxd.h"

#include "core_data.h"
#include "mapHexBaseShader.h"
#include "../../utilities/AssetIO/API.h"

    void Map::Init(uint32_t hexLayers){
        SHADER.Init(mapHexBasevSrc,mapHexBasefSrc);
        glUseProgram(SHADER.program);
        GLCheck("glUseProgram");

        nHexLayers = hexLayers;
        stoHexes.resize( 3* nHexLayers * (nHexLayers - 1 ) + 1 );
        
        float aspect = (float)Engine::openGLEngine.width / (float)Engine::openGLEngine.height;
	    M4 uModel = M4::identity();
		uModel[1][1] = aspect;
        
        stoMatrix.resize(stoHexes.size(), uModel);
        stoTEXIDX.resize(stoHexes.size(), 0);

        

        for(uint16_t i = 0; i < stoHexes.size(); ++i){
            stoHexes[i].SetIdx(i);
            V2D pos = stoHexes[i].GetScreenCoord(hex_base_len, hex_base_h);
            LOGI("Position at: " + std::to_string(pos.x) + ", " + std::to_string(pos.y));
            stoMatrix[i][3][0] = pos.x;
            stoMatrix[i][3][1] = pos.y;
            stoTEXIDX[i] = i % 4;
        }
        /*
        for(uint16_t i = 0; i < stoHexes.size(); ++i){
            
            if(i > 4){
                stoHexes[i].FromIdx(i);
                V2D loc = stoHexes[i].GetScreenCoord(hex_base_len, hex_base_h);
                stoMatrix[i][3][0] = loc.x;
                stoMatrix[i][3][1] = loc.y;
                stoTEXIDX[i] = stoHexes[i].level;
            } else {
                if(i == 1){
                    stoMatrix[i][3][0] = 0.3;
                    stoMatrix[i][3][1] = 0.3;
                }
                if(i == 2){
                    stoMatrix[i][3][0] = -0.3;
                    stoMatrix[i][3][1] = 0.3;
                }
                if(i == 3){
                    stoMatrix[i][3][0] = -0.3;
                    stoMatrix[i][3][1] = -0.3;
                }
            }
        }*/

        AddMesh();
        AddTextures();
    }

    
    


    void Map::Draw(const M4& uViewProj){
        glUseProgram(SHADER.program);
        glBindVertexArray(VAO);
        

        // Bind texture array
        {
            GLint uTexArrayLoc = glGetUniformLocation(SHADER.program, "u_texArray");
            GLCheck("Uniform Get Texture Loc");
            glUniform1i(uTexArrayLoc, 0);
            GLCheck("Uniform Set Texture 0");
        }
	    glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, TEX_ARRAY);

        // Set view-projection
        {
            GLint uviewProjLoc = glGetUniformLocation(SHADER.program, "u_viewProj");
            GLCheck("Uniform Get biewProj Loc");
            glUniformMatrix4fv(uviewProjLoc, 1, GL_FALSE, &uViewProj[0][0]);
            GLCheck("Uniform u_viewroj");
        }

        
        glDrawElementsInstanced(
            GL_TRIANGLES,
            18,
            GL_UNSIGNED_BYTE, // or GL_UNSIGNED_INT
            0,
            stoHexes.size()
        );
         GLCheck("GlDrawElements");
    }

    void Map::ChangeInstanceCount(uint16_t newCount){
        stoHexes.resize(newCount);
        float aspect = (float)Engine::openGLEngine.width / (float)Engine::openGLEngine.height;
		M4 uModel = M4::identity();
		uModel[1][1] = aspect;
        
        stoMatrix.resize(stoHexes.size(), uModel);
        stoMatrix.resize(stoHexes.size());
        stoTEXIDX.resize(stoHexes.size(), 0);

        // Resize matrix buffer
        glBindBuffer(GL_ARRAY_BUFFER, VBO_MAT);
        glBufferData(GL_ARRAY_BUFFER, newCount * sizeof(M4), stoMatrix.data(), GL_DYNAMIC_DRAW);

        // Resize tex index buffer
        glBindBuffer(GL_ARRAY_BUFFER, VBO_TEX_IDX);
        glBufferData(GL_ARRAY_BUFFER, newCount * sizeof(float), stoTEXIDX.data(), GL_DYNAMIC_DRAW);
    }

    void Map::AddMesh() {
        // 1. Hexagon mesh (shared per-vertex data)
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

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


        // location = 1: vec2 a_uv
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

        // 2. Instance data: mat4 a_model (location 1, 2, 3, 4)
        glGenBuffers(1, &VBO_MAT);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_MAT);
        glBufferData(GL_ARRAY_BUFFER, stoHexes.size() * sizeof(M4), stoMatrix.data(), GL_DYNAMIC_DRAW);
        GLCheck("glBufferData VBO_MAT");


        for (int i = 0; i < 4; ++i) {
            glEnableVertexAttribArray(2 + i);
            glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE, sizeof(M4), (void*)(sizeof(float) * i * 4));
            glVertexAttribDivisor(2 + i, 1); // <-- advance once per instance
        }

        // 3. Instance data: float a_texIndex (location 5)
        glGenBuffers(1, &VBO_TEX_IDX);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_TEX_IDX);
        glBufferData(GL_ARRAY_BUFFER, stoHexes.size() * sizeof(uint16_t), stoTEXIDX.data(), GL_DYNAMIC_DRAW);
        GLCheck("glBufferData VBO_TEX_IDX");
        glEnableVertexAttribArray(6);
        glVertexAttribIPointer(6, 1, GL_UNSIGNED_SHORT, sizeof(uint16_t), (void*)0);
        glVertexAttribDivisor(6, 1);

        // 4. Unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Map::AddTextures(){
        constexpr int width = 64, height = 64;  
        glGenTextures(1, &TEX_ARRAY);
        glBindTexture(GL_TEXTURE_2D_ARRAY, TEX_ARRAY);
        glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, width, height, nTexInArray, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        // Upload each layer individually
        for (int i = 0; i < nTexInArray; ++i) {
            texture2D tex;
            if(GetTexture("images/map/BaseHexagonLvl" + std::to_string(i) + ".png",tex)){
                glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, tex.tex.get());
                GLCheck("glBufferData TEXSUBIMAGE3D ");
            }
        }

        // Set texture parameters — very important!
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
