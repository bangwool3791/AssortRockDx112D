#pragma once
#include <Engine/CScript.h>

class CTileMap;
class CGameObjectEx;

class CTileScript :
    public CScript
{
private:
    UINT                m_id;
    CGameObject*        m_pCameraObject;
    CGameObject*        m_pMouseObject;
    Vec3                m_vCameraPos;
    Vec3			    m_vMousePos;
    Vec2                m_vRenderResolution;

    Vec2                m_vTileSize;
public:
    virtual void begin();
    virtual void tick();
    virtual void BeginOverlap(CCollider2D* _pOther);
    virtual void Overlap(CCollider2D* _pOther);
    virtual void EndOverlap(CCollider2D* _pOther);
public:
    void    SetTileInfo(UINT _ibyOption);
    void    Initialize(void* _pAddr);
    CLONE(CTileScript);
public:
    CTileScript();
    virtual ~CTileScript();
};

