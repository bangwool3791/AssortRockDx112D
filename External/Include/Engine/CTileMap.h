#pragma once
#include "pch.h"
#include "CRenderComponent.h"

#include "Ptr.h"
#include "CTexture.h"

class CStructuredBuffer;

class CTileMap :
    public CRenderComponent
{
public:
    enum class TILE_MODE {INGAME, EDIT};
private:
    Vec2                    m_vTileSize;
    TILE_MODE               m_eTIieMode;
public:
    void    SetTileMode(TILE_MODE _eType) { m_eTIieMode = _eType; }
    void    SetTileSizeX(float _fSize) { m_vTileSize.x = _fSize; }
    void    SetTileSizeY(float _fSize) { m_vTileSize.y = _fSize; }
    Vec2    GetTileSize() { return m_vTileSize; }
private:
    vector<Ptr<CTexture>>   m_AtlasTex;     // Ÿ�ϸ��� ����ϴ� ��Ʋ�� �̹���   
    Vec2                    m_vTileCount;   // Ÿ�ϸ� ���� ���� ����

    vector<tTile>           m_vecTile;
    vector<list<tTile>>	    m_vecAdjacency;
    CStructuredBuffer*      m_TileBuffer;   // �� Ÿ���� ��Ʋ�� �������� ����ü

    Vec2                    m_vLeftTop;
    Vec2                    m_vSlice;

    CGameObject*            m_pCamera;
    Vec4                    m_vCameraPos;
private:
    void Ready_Adjacency();
public:
    void SetTileAtlas(Ptr<CTexture> _AtlasTex) { m_AtlasTex.push_back(_AtlasTex); }
    void SetTileCount(UINT _iWidth, UINT _iHeight);
    void EidtApply();
    vector<tTile>& GetTiles() {return m_vecTile;}
public:
    virtual void begin() override;
    virtual void finaltick() override;
    virtual void render() override;
    virtual void render_Instancing() override{}

    CLONE(CTileMap);
public:
    CTileMap();
    ~CTileMap();
};

