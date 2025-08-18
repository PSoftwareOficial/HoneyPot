#define STB_IMAGE_IMPLEMENTATION
#include "AssetIO/API.h"

bool GetTexture(const std::string& path, texture2D& tex){
    std::unique_ptr<unsigned char[]> bufferTex;
    std::size_t sizeTex;
    if(!AssetIO::getInstance().read(path, bufferTex, sizeTex)) {
        LOG("Asset " + std::string(path) + "could not be opened");
        return false;
    }
    
    int channels;
    tex.tex.reset(stbi_load_from_memory(
        bufferTex.get(),          // buffer pointer
        sizeTex,          // buffer size
        &tex.width, &tex.height, &channels,
        STBI_rgb_alpha          // force 4 channels (RGBA)
    ));

    if(tex.tex.get()){
        return true;
    } else 
        return false;
}