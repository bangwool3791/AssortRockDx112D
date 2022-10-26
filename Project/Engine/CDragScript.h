#pragma once
#include "CScript.h"

class CDragScript :
    public CScript
{
public:
    virtual void begin();
    virtual void tick();

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

