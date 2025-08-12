#pragma once

#include "BaseUI.h"

class Button : BaseUI {
    public:
        virtual void Init(V2D PosParam,V2D SizeParam,V3Du8 ColorParam);
        virtual void Draw() override;
        V3Du8 color;
        virtual Button() override {}
        virtual ~Button() override {}
};