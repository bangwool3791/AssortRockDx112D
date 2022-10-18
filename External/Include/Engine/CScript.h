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

public :
    virtual void tick() = 0;
    virtual void finaltick() final {};

public :
    CScript();
    ~CScript();
};

