#pragma once
#include <Engine/CScript.h>

class CWWCScript :
    public CScript
{
private:
    BUILD_STATE                 m_eBuildState;
    UINT                        m_iIndex = 0;

    float                       m_fDt = 0.f;
    float                       m_fDt2 = 0.f;
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
    CLONE(CWWCScript);
public:
    CWWCScript();
    virtual ~CWWCScript();
};

