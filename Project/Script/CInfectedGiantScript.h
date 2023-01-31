#pragma once
#include <Engine\CScript.h>

class CInfectedGiantScript :
    public CScript
{
private:
    UNIT_STATE       m_eState;
    bool             m_bActiveJps = false;
    bool             m_bAttack = false;
    float            m_fAccTime;
    float            m_fSpeed;
    float            m_fTick;
    Vec3             m_vSource;
    Vec3             m_vDest;
    CGameObject* m_pTargetObject;
    Ptr<CPrefab>     m_Prefab;
    vector<Vec3>     m_vecJps;
    wstring          m_strBackUp{};
private:
    void SaveToFile(FILE* _File);
    void LoadFromFile(FILE* _File);
public:
    virtual void begin();
    virtual void tick();
    virtual void BeginOverlap(CCollider2D* _pOther);
    virtual void Overlap(CCollider2D* _pOther);
    virtual void EndOverlap(CCollider2D* _pOther);
    CLONE(CInfectedGiantScript);
public:
    void JpsAlgorithm(Int32 x, Int32 z);
    void SetDestPos(Vec3 _vPos);
private:
    bool Func(CGameObject* _lhs, CGameObject* _rhs);
    void ProcessEnemy();
    void ChaseEnemy();
    void Move(Int32 _x, Int32 _z);
    void SetPlayerHP();
public:
    CInfectedGiantScript();
    virtual ~CInfectedGiantScript();
};


