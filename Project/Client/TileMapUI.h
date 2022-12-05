#pragma once
#include "UI.h"

class CGameObject;
class ComponentUI;
class CTileMap;

class TileMapUI :
    public UI
{
private:
    CGameObject*  m_pTileObject;
    CTileMap*     m_pTileMap;
private:
    Vec2         m_vTileSize;
public:
private:
    virtual void begin() override;
    virtual void update() override;
    virtual void render_update() override;

public:
    TileMapUI();
    ~TileMapUI();
};

