#pragma once
#include "UI.h"

class CGameObject;
class ComponentUI;
class CTileMap;

class TileMapUI :
    public UI
{
private:
    CGameObject*  m_pEditTileObject;
    CTileMap*     m_pEditTileMap;
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

