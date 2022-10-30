#pragma once
#include "CScript.h"
class CSelectUnitScript :
    public CScript
{
private:
    float   m_fSpeed;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) {};
    virtual void Overlap(CCollider2D* _pOther) {};
    virtual void EndOverlap(CCollider2D* _pOther) {};

public:
    CLONE(CSelectUnitScript);
public:
    CSelectUnitScript();
    virtual ~CSelectUnitScript();

};

