#pragma once
#include "../utilities/AssetIO/API.h"
#include "../utilities/log/API.h"
#include "../rendering/2DRenderer/2DRenderer.h"
#include <vector>
#include "../Engine/Main/Engine.h"
#include "UI/UIManager.h"
#include "Object.h"
#include "../rendering/core_include.h"
#include "../utilities/Buffer/Buffer.h"


class World : public Object {
    public:
    virtual int Init() override; 
    virtual int InitGL() override;
    virtual int Update(uint64_t EuS, uint64_t TuS) override;
    virtual int Draw() override;


    virtual ~World() {};

    float FPS;
    RingBuffer<float, 5> m5FPS;


    std::vector<std::shared_ptr<Object>> vObjects;
}; 
