#pragma once
#include "CScript.h"
class CMonsterScript :
    public CScript
{
private:
    float m_fSpeed;
public:
    virtual void begin();
    virtual void tick();
    virtual void BeginOverlap(CCollider2D* _pOther);
    virtual void Overlap(CCollider2D* _pOther);
    virtual void EndOverlap(CCollider2D* _pOther);
    CLONE(CMonsterScript);
public:
    CMonsterScript();
    ~CMonsterScript();
};

