#pragma once

#include "BaseUI.h"

class Container : public BaseUI {
    public:
        virtual void Init(V2D PosParam,V2D SizeParam,V3Du8 ColorParam,float AlphaParam);
        virtual void Draw() override;
        virtual void Touch(V2D coord) override;
        virtual void GetGLData(float (&data)[20]);

        virtual ~Button() override {}
};