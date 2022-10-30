#pragma once
#include "CScript.h"
class CMouseScript :
    public CScript
{
public:
    virtual void begin();
    virtual void tick();

    virtual void BeginOverlap(CCollider2D* _pOther);
    virtual void Overlap(CCollider2D* _pOther);
    virtual void EndOverlap(CCollider2D* _pOther);

    CLONE(CMouseScript);

private:
    bool bClicked;
    Vec2 StartPos;
    Vec2 EndPos;
    vector<CGameObject*>    m_vecUnit;
public:
    CMouseScript();
    virtual ~CMouseScript();

};

