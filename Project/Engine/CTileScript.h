#pragma once
#include "CScript.h"

class CTileMap;

class CTileScript :
    public CScript
{
private:
    tTile           m_tTileInfo;
    CTileMap*       m_pTileMap;
    CGameObject*    m_pCamera;
    Vec3            m_vCameraPos;
    Vec2			m_vMousePos;
    Vec2            m_vRenderResolution;

    Vec2            m_vTileSize;
private:
    bool Picking(const Vec3& vPos, UINT& iIndex);
public:
    virtual void begin();
    virtual void tick();
    virtual void BeginOverlap(CCollider2D* _pOther);
    virtual void Overlap(CCollider2D* _pOther);
    virtual void EndOverlap(CCollider2D* _pOther);
public :
    void    SetTileInfo(UINT _ibyOption){ m_tTileInfo.ibyOption = _ibyOption;}
    CLONE(CTileScript);
public:
    CTileScript();
    virtual ~CTileScript();
};

