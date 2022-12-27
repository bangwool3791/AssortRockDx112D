#pragma once
#include <Engine\CScript.h>
class CPlayerScript :
    public CScript
{
private:
    float        m_fAccTime;
    float        m_fSpeed;
    float        m_fJumpHeight;
    Ptr<CPrefab> m_Prefab;
private:
    void SaveToFile(FILE* _File);
    void LoadFromFile(FILE* _File);
public :
    virtual void begin();
    virtual void tick();
    virtual void BeginOverlap(CCollider2D* _pOther);
    virtual void Overlap(CCollider2D* _pOther);
    virtual void EndOverlap(CCollider2D* _pOther);
    CLONE(CPlayerScript);
public:
    CPlayerScript();
    virtual ~CPlayerScript();
};

