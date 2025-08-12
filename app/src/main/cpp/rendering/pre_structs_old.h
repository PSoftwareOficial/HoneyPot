#pragma once
#include "core_include.h"
#include "../utilities/math/structs/vxd.h"
#include "../utilities/math/structs/mxt.h"
#include "../utilities/log/API.h"



class ID_Counter{
public:
	ID_Counter() {store.push_back(0);}
	unsigned int gen() {
		//Add one to the value
		store.push_back(1);
		return counter++;
	}
	void inc(unsigned int ID){
		store[ID] += 1;
	}
	bool dec(unsigned int ID){
		if (ID == 0) return false;
		store[ID] -= 1;
		return !store[ID];
	}
private:
	std::vector<unsigned int> store;
	unsigned int counter = 1;
};



static ID_Counter gpu_geometry_id;

struct gpu_geometry{
	gpu_geometry() {}
	unsigned int ID = 0;
	unsigned int VAO = 0;
	unsigned int VBO = 0;	
	unsigned int IBO = 0;
	unsigned int IBO_size = 0;
	enum {NONE, POLYGON, TRIANGLE} type;
	void SetPolygon(const uint8_t v_amount, const uint8_t tris_amount,const V3D v[],const V3Du8 indices[], const V3D* rgb = nullptr,const V2D uv[] = nullptr) {
		ID = gpu_geometry_id.gen();
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
	void Set(const gpu_geometry& other){
		//If a value is stored here, free the resources;
		Unset();
		gpu_geometry_id.inc(other.ID);
		ID = other.ID;
		VAO = other.VAO;
		VBO = other.VBO;	
		IBO = other.IBO;
		IBO_size = other.IBO_size;
		type = other.type;
	}

	void Unset(){
		if(gpu_geometry_id.dec(ID)){
			if(VBO)
				glDeleteBuffers(1, &VBO);
			if(VAO)
				glDeleteVertexArrays(1, &VAO);
			if(IBO)
				glDeleteBuffers(1, &IBO);
		}
		ID = 0;
		VBO = 0;
		IBO = 0;
		VAO = 0;
		type = NONE;
	}
	gpu_geometry(const gpu_geometry& other) { Set(other); }
	gpu_geometry& operator=(const gpu_geometry& other) {
    	if (this != &other) Set(other);
    	return *this;
	}
	gpu_geometry(gpu_geometry&& other) noexcept {
		ID = other.ID; VAO = other.VAO; VBO = other.VBO; IBO = other.IBO; IBO_size = other.IBO_size;type = other.type;
		other.ID = 0; other.VAO = 0; other.VBO = 0; other.IBO = 0; other.IBO_size = 0;other.type = NONE;
	}

	gpu_geometry& operator=(gpu_geometry&& other) noexcept {
		if (this != &other) {
			Unset();
			ID = other.ID; VAO = other.VAO; VBO = other.VBO; IBO = other.IBO; IBO_size = other.IBO_size;type = other.type;
			other.ID = 0; other.VAO = 0; other.VBO = 0; other.IBO = 0; other.IBO_size = 0;other.type = NONE;
		}
		return *this;
	}
	~gpu_geometry() {
		Unset();
	}
};



static ID_Counter gpu_texture_id;
struct gpu_texture{
	gpu_texture() {}
	unsigned int ID = 0;
	unsigned int TEX = 0;
	unsigned int DATA_TEX = 0;
	V3D vGlowColor;

	void Set(const texture2D* tex = nullptr, const texture2D* dataTex = nullptr) {
		ID = gpu_texture_id.gen();
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
	void Set(const gpu_texture& other){
		//If a value is stored here, free the resources;
		Unset();
		gpu_texture_id.inc(other.ID);
		ID = other.ID;
		TEX = other.TEX;
		DATA_TEX = other.DATA_TEX;
		vGlowColor = other.vGlowColor;	
	}

	void Unset(){
		if(gpu_texture_id.dec(ID)){
			if(TEX != dummy_tex)
				glDeleteTextures(1, &TEX);
			if(DATA_TEX != dummy_tex && DATA_TEX != TEX)
				glDeleteTextures(1, &DATA_TEX);
		}
		ID = 0;
		TEX = 0;
		DATA_TEX = 0;
		vGlowColor = V3D();
	}
	gpu_texture(const gpu_texture& other) { Set(other); }
	gpu_texture& operator=(const gpu_texture& other) {
    	if (this != &other) Set(other);
    	return *this;
	}
	gpu_texture(gpu_texture&& other) noexcept {
		ID = other.ID; TEX = other.TEX; DATA_TEX = other.DATA_TEX; vGlowColor = other.vGlowColor;
		other.ID = 0; other.TEX = 0; other.DATA_TEX = 0; other.vGlowColor = V3D();
	}

	gpu_texture& operator=(gpu_texture&& other) noexcept {
		if (this != &other) {
			Unset();
			ID = other.ID; TEX = other.TEX; DATA_TEX = other.DATA_TEX; vGlowColor = other.vGlowColor;
			other.ID = 0; other.TEX = 0; other.DATA_TEX = 0; other.vGlowColor = V3D();
		}
		return *this;
	}
	~gpu_texture() {
		Unset();
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