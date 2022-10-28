#pragma once
#include "CScript.h"
class CCameraScript :
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

    void         Move();

public :
    CLONE(CCameraScript);
public:
    CCameraScript();
    ~CCameraScript();

};

