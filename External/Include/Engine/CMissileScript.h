#pragma once
#include "CScript.h"

class CMissileScript :
    public CScript
{
private:
    float       m_fSpeed;

public:
    virtual void tick() override;
    virtual void BeginOverlap(CCollider2D* _pOther) {};
    virtual void Overlap(CCollider2D* _pOther) {};
    virtual void EndOverlap(CCollider2D* _pOther) {};
    CLONE(CMissileScript);
public:
    CMissileScript();
    virtual ~CMissileScript();
};

