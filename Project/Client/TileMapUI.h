#pragma once
#include "UI.h"

class CGameObject;
class ComponentUI;

class TileMapUI :
    public UI
{
private:
public:
private:
    virtual void update() override;
    virtual void render_update() override;

public:
    TileMapUI();
    ~TileMapUI();
};

