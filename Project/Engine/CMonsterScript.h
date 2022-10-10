#pragma once
#include "CScript.h"

class CMonsterScript :
    public CScript
{

public:
    virtual void tick() override;

public:
    CMonsterScript();
    ~CMonsterScript();
};

