#pragma once
#include <Engine/CScript.h>

class CEffectScript :
    public CScript
{
private:
public:
    virtual void begin();
    virtual void tick();
    virtual void finaltick();
    virtual void BeginOverlap(CCollider2D* _pOther);
    virtual void Overlap(CCollider2D* _pOther);
    virtual void EndOverlap(CCollider2D* _pOther);
public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
public:
    CLONE(CEffectScript);
public:
    CEffectScript();
    virtual ~CEffectScript();
};

