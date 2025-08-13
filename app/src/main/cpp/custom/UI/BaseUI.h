#pragma once
#include <vector>
#include <memory>


#include "../../utilities/math/structs/vxd.h"
#include "../../rendering/core_include.h"

class BaseUI {
    public:
    std::shared_ptr<BaseUI> parent;
    std::vector<std::shared_ptr<BaseUI>> children;
    virtual void Draw() = 0;
    virtual void Touch(V2D coord) = 0;
    virtual ~BaseUI() {};
    
    V2D pos;
    V2D size;
    V3Du8 color;
    float alpha;

    GLuint VAO;
    GLuint VBO;

};