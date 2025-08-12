#pragma once
#include "../../rendering/pre_structs.h"
#include "../../utilities/AssetIO/API.h"
#include "../core_include.h"
#include <cstdlib>

class BasicHex {
    public:
    void Init(){
        texture2D tex, data_tex;
		if(GetTexture("images/BaseHexagon.png",tex)){
			
			
			gpu_hex.geometry.SetPolygon(7, 6, hex_base_vertices, hex_base_indices, nullptr, hex_base_uv);
			gpu_hex.texture.Set(&tex/*, &data_tex*/);
			gpu_hex.Init();

		} else {
			//Failed to decode
			LOG(std::string("Failed to decode image:") + stbi_failure_reason());
		}
        startMicros = (std::rand() % 10) * 10000 ;
    }

    void Update(uint64_t uElapsedMicros, uint64_t uTotalMicros){
        gpu_hex.texture.vGlowColor = (std::sin((float)(uTotalMicros + startMicros) / 5e5) + 1) * 0.3f * V3D(1.0f, 0.8f, 0.05f);
	    gpu_hex.Draw();
    }
    void End(){

    }

    void SetLocation(V2D newLoc){
        location = newLoc;
        gpu_hex.uModel[3][0] = location.x;
        gpu_hex.uModel[3][1] = location.y;
    }
    gpu_object gpu_hex;
    private:
    uint64_t startMicros = 0;
    V2D location;
    
};