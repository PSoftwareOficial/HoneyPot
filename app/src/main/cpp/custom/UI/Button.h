#pragma once

#include "BaseUI.h"

class Button : public BaseUI {
    public:
        virtual void Init(V2D PosParam,V2D SizeParam,V3Du8 ColorParam);
        virtual void Draw() override;
        virtual void GetGLData(float (&data)[20]);

        virtual ~Button() override {}

        V3Du8 color;
};