#pragma once

#include <vector>
#include "Object.h"
#include "utilities/Buffer/Buffer.h"


class World : public Object {
    public:
    World();
    virtual int Init() override; 
    virtual int InitGL() override;
    virtual int Update(uint64_t EuS, uint64_t TuS) override;
    virtual int Draw() override;
    int AddObject(std::shared_ptr<Object>& vObjects);


    virtual ~World() {};

    float FPS;
    RingBuffer<float, 5> m5FPS;

    protected:
    std::vector<std::shared_ptr<Object>> vObjects;
}; 
