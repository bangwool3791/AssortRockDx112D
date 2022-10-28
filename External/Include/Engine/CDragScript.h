#pragma once
#include "CScript.h"

class CDragScript :
    public CScript
{
public:
    virtual void begin();
    virtual void tick();

    virtual void BeginOverlap(CCollider2D* _pOther);
    virtual void Overlap(CCollider2D* _pOther);
    virtual void EndOverlap(CCollider2D* _pOther);

    CLONE(CDragScript);

private:
    bool bClicked;
    Vec2 StartPos;
    Vec2 EndPos;
public:
    CDragScript();
    CDragScript(const CDragScript& rhs);
    ~CDragScript();

};

