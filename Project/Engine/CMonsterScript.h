#pragma once
#include "CScript.h"

class CMonsterScript :
    public CScript
{

public:
    virtual void tick() override;
    virtual void BeginOverlap(CCollider2D* _pOther) {};
    virtual void Overlap(CCollider2D* _pOther) {};
    virtual void EndOverlap(CCollider2D* _pOther) {};

    CLONE(CMonsterScript);
public:
    CMonsterScript();
    ~CMonsterScript();
};

