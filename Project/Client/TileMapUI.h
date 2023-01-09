#pragma once
#include "UI.h"

class CGameObject;
class ComponentUI;
class CTerrain;

class TileMapUI :
    public UI
{
private:
    CGameObject*  m_pEditTileObject;
    CTerrain*     m_pEditTileMap;
private:
    Vec2         m_vTileSize;
private:
    virtual void begin() override;
    virtual void update() override;
    virtual void render_update() override;

public :
    void    Initialize(void* pAddr);
public:
    TileMapUI();
    ~TileMapUI();
};

