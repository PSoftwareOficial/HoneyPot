#pragma once
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <memory>
#include <mutex>

class AssetIO {
public:
    static AssetIO& getInstance() {
	static AssetIO instance;
	return instance;
	}

    // Initialize once with the android_app path
    void init(AAssetManager* assetManagerP) {
        assetManager = assetManagerP;
    }


    //Open a file
    bool read(const std::string& path, std::unique_ptr<unsigned char[]>& buffer, std::size_t& size_asset){
        // Open asset
        //Delete if has
        buffer.reset();

        std::lock_guard<std::mutex> lock(mtx);
        AAsset* asset = AAssetManager_open(assetManager, path.c_str(), AASSET_MODE_UNKNOWN);
        if (asset) {
            off_t size = AAsset_getLength(asset);
            size_asset = size;
            buffer = std::make_unique<unsigned char[]>(size);
            AAsset_read(asset, buffer.get(), size);

            // Use buffer, e.g. decode PNG, upload to OpenGL texture, etc.
            AAsset_close(asset);
            return true;
        }
            return false;
    }

private:
    AssetIO() = default;
    ~AssetIO() {}

    AssetIO(const AssetIO&) = delete;
    AssetIO& operator=(const AssetIO&) = delete;


    AAssetManager* assetManager;
    std::mutex mtx;
};






#include "../../rendering/core_include.h"
#include "../../rendering/stb_image.h"

bool GetTexture(const std::string& path, texture2D& tex);