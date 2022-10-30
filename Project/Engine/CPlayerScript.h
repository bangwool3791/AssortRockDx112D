#pragma once
#include "CScript.h"
class CPlayerScript :
    public CScript
{
private:
    float m_fSpeed;
    bool m_bDrag;
public:
    bool IsDrag() { return m_bDrag; }
public :
    virtual void begin();
    virtual void tick();
    virtual void BeginOverlap(CCollider2D* _pOther);
    virtual void Overlap(CCollider2D* _pOther);
    virtual void EndOverlap(CCollider2D* _pOther);
    CLONE(CPlayerScript);
public:
    CPlayerScript();
    ~CPlayerScript();
};

