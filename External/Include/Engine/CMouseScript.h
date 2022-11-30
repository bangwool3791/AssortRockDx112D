#pragma once
#include "CScript.h"
class CMouseScript :
    public CScript
{
public:
    virtual void begin();
    virtual void tick();
    virtual void finaltick();

    virtual void BeginOverlap(CCollider2D* _pOther);
    virtual void Overlap(CCollider2D* _pOther);
    virtual void EndOverlap(CCollider2D* _pOther);

    CLONE(CMouseScript);

private:
    bool bClicked;
    Vec2 StartPos;
    Vec2 EndPos;
    vector<CGameObject*>    m_vecUnit;
    CGameObject*            m_pCamera;
    float                   m_fCameraScale;
    Vec3                    m_vCameraPos;

    Vec2                    m_vRenderResolution;
    Vec2                    m_vMousePos;
    Vec3                    m_vTarget;
public:
    CMouseScript();
    virtual ~CMouseScript();

};

