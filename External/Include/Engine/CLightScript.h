#pragma once
#include "CScript.h"
class CLightScript :
    public CScript
{
public:
public:
    virtual void begin();
    virtual void tick();
    virtual void BeginOverlap(CCollider2D* _pOther);
    virtual void Overlap(CCollider2D* _pOther);
    virtual void EndOverlap(CCollider2D* _pOther);
    CLONE(CLightScript);
public:
    CLightScript();
    virtual ~CLightScript();
};

