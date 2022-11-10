#pragma once
#include "CComponent.h"

#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CResMgr.h"
#include "GlobalComponent.h"

class CScript :
    public CComponent
{
private :
    int m_iCriptType;
protected:
    Vec3  m_vTarget;
public :
    void Set_Target(Vec3 _vTarget) { m_vTarget = _vTarget; }
public :
    virtual void tick() = 0;
    virtual void finaltick() {};

    virtual CScript* Clone() = 0;
public :
    CScript();
    virtual ~CScript();

public:
    virtual void BeginOverlap(CCollider2D* _pOther) = 0;
    virtual void Overlap(CCollider2D* _pOther) = 0;
    virtual void EndOverlap(CCollider2D* _pOther) = 0;
};

