#pragma once
#include "CScript.h"
class CPlayerScript :
    public CScript
{
public :
    virtual void begin();
    virtual void tick();

    CLONE(CPlayerScript);
public:
    CPlayerScript();
    ~CPlayerScript();
};

