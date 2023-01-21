#pragma once
#include <Engine\CScript.h>
class CSoldierScript :
    public CScript
{
private:
    UNIT_STATE       m_eState;
    float            m_fAccTime;
    float            m_fSpeed;
    Vec3             m_vDest;
    CGameObject*     m_pTarget;
    Ptr<CPrefab>     m_Prefab;
    vector<Vec3>     m_vecJps;
private:
    void SaveToFile(FILE* _File);
    void LoadFromFile(FILE* _File);
public:
    virtual void begin();
    virtual void tick();
    virtual void BeginOverlap(CCollider2D* _pOther);
    virtual void Overlap(CCollider2D* _pOther);
    virtual void EndOverlap(CCollider2D* _pOther);
    CLONE(CSoldierScript);
public :
    void JpsAlgorithm(Int32 x, Int32 z);
    void SetDestPos(Vec3 _vPos);
public:
    CSoldierScript();
    virtual ~CSoldierScript();
};

