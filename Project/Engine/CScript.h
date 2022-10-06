#pragma once
#include "CComponent.h"
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

