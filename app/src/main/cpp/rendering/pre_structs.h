#pragma once
#include "core_include.h"
#include "../utilities/math/structs/vxd.h"
#include "../utilities/math/structs/mxt.h"
#include "../utilities/log/API.h"





struct gpu_geometry{
	gpu_geometry() {}
	unsigned int VAO = 0;
	unsigned int VBO = 0;	
	unsigned int IBO = 0;
	unsigned int IBO_size = 0;
	enum {NONE, POLYGON, TRIANGLE} type;
	void SetPolygon(const uint8_t v_amount, const uint8_t tris_amount,const V3D v[],const V3Du8 indices[], const V3D* rgb = nullptr,const V2D uv[] = nullptr) {
		type = POLYGON;
		//Make a vertex array
		float* vertices = new float[v_amount * 8]; 
		for(uint8_t i = 0; i < v_amount; ++i){
			//POS
			vertices[i * 8] = v[i].x; vertices[i * 8 + 1] = v[i].y;vertices[i * 8 + 2] = v[i].z;
			//RGB
			if(rgb != nullptr){
				vertices[i * 8 + 3] = rgb[i].x; vertices[i * 8 + 4] = rgb[i].y;vertices[i * 8 + 5] = rgb[i].z;
			} else {
				vertices[i * 8 + 3] = 1.0f; vertices[i * 8 + 4] = 1.0f;vertices[i * 8 + 5] = 1.0f;
			}
			if(uv){
				//UV
				vertices[i * 8 + 6] = uv[i].x;vertices[i * 8 + 7] = uv[i].y;
			} else {
				vertices[i * 8 + 6] = 0.0f;vertices[i * 8 + 7] = 0.0f;
			}
		}
		
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &IBO);

		// Bind VAO
		glBindVertexArray(VAO);

		// Setup VBO
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, v_amount * 8 * sizeof(float), vertices, GL_STATIC_DRAW);

		// Setup IBO
		IBO_size = (tris_amount) * 3;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IBO_size * sizeof(uint8_t), indices, GL_STATIC_DRAW);

		// Position attribute
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

		// Color attribute
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

		// UV attribute (location = 2): 2 floats offset 6 floats
    	glEnableVertexAttribArray(2);
    	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

		// Unbind VAO (optional)
		glBindVertexArray(0);

		delete[] vertices;
	}

	~gpu_geometry() {
		if(VBO)
				glDeleteBuffers(1, &VBO);
			if(VAO)
				glDeleteVertexArrays(1, &VAO);
			if(IBO)
				glDeleteBuffers(1, &IBO);
	}
};



struct gpu_texture{
	gpu_texture() {}
	unsigned int TEX = 0;
	unsigned int DATA_TEX = 0;
	V3D vGlowColor;

	void Set(const texture2D* tex = nullptr, const texture2D* dataTex = nullptr) {
		if(tex){
			// Upload texture data
			glGenTextures(1, &TEX);
			glBindTexture(GL_TEXTURE_2D, TEX);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->width, tex->height, 0,
					GL_RGBA, GL_UNSIGNED_BYTE, tex->tex.get());

			// Setup filtering (adjust as needed)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			// Wrap mode (optional)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		} else TEX = dummy_tex;

		if(dataTex){
			// Upload texture data
			glGenTextures(1, &DATA_TEX);
			glBindTexture(GL_TEXTURE_2D, DATA_TEX);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dataTex->width, dataTex->height, 0,
					GL_RGBA, GL_UNSIGNED_BYTE, dataTex->tex.get());

			// Setup filtering (adjust as needed)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// Wrap mode (optional)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		} else DATA_TEX = dummy_tex;
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	~gpu_texture() {
		if(TEX != dummy_tex)
				glDeleteTextures(1, &TEX);
			if(DATA_TEX != dummy_tex && DATA_TEX != TEX)
				glDeleteTextures(1, &DATA_TEX);
	}
};


struct gpu_object {

	void Init(){
		float aspect = (float)gles_width / (float)gles_height;
		uModel = M4::identity();
		uModel[1][1] = aspect;
	}

	void Draw(){
		glUniformMatrix4fv(u_model_loc, 1, GL_FALSE, (float*) uModel.el);
		glUniform3f(u_glowcolor_loc, texture.vGlowColor.x,texture.vGlowColor.y,texture.vGlowColor.z);
		switch(geometry.type){
			case gpu_geometry::TRIANGLE:
				glBindVertexArray(geometry.VAO);
				glDrawArrays(GL_TRIANGLES, 0, 3);
			break;
			case gpu_geometry::POLYGON:
				glActiveTexture(GL_TEXTURE0);
        		glBindTexture(GL_TEXTURE_2D, texture.TEX);
				glActiveTexture(GL_TEXTURE1);
        		glBindTexture(GL_TEXTURE_2D, texture.DATA_TEX);
				glBindVertexArray(geometry.VAO);
				glDrawElements(GL_TRIANGLES, geometry.IBO_size, GL_UNSIGNED_BYTE, 0);
			break;
			default:
			break;
		};


	}


	gpu_geometry geometry;
	gpu_texture texture;
	M4 uModel;
};