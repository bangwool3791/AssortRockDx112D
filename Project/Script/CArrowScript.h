#pragma once
#include <Engine\CScript.h>
class CArrowScript :
    public CScript
{
private:
    float            m_fDeltaTime = 0.f;
    float            m_fAccTime;
    float            m_fSpeed;
    Vec3             m_vDest;
    Vec3             m_vSource;
    CGameObject*     m_pTargetObject;
private:
    void SaveToFile(FILE* _File);
    void LoadFromFile(FILE* _File);
public:
    virtual void begin();
    virtual void tick();
    virtual void BeginOverlap(CCollider2D* _pOther);
    virtual void Overlap(CCollider2D* _pOther);
    virtual void EndOverlap(CCollider2D* _pOther);
    CLONE(CArrowScript);
public :
    void SetTarget(CGameObject* _pObj);
public:
    CArrowScript();
    virtual ~CArrowScript();
};

