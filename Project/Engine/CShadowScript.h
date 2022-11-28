#pragma once
#include "CScript.h"
class CShadowScript :
    public CScript
{
private:
    float m_fSpeed;
public:
    Vec4  m_vColor;
public:
    void SetColor(Vec4 _vColor);
public:
    virtual void begin();
    virtual void tick();
    virtual void BeginOverlap(CCollider2D* _pOther);
    virtual void Overlap(CCollider2D* _pOther);
    virtual void EndOverlap(CCollider2D* _pOther);
    CLONE(CShadowScript);
public:
    CShadowScript();
    virtual ~CShadowScript();
};

