#pragma once
#include "ResUI.h"
class MaterialUI :
    public ResUI
{

private:
    virtual void update() override;
    virtual void render_update() override;

public:
    MaterialUI();
    ~MaterialUI();
};

