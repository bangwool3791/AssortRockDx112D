#pragma once
#include <Engine\CScript.h>
class CSniperScript :
    public CScript
{
private:
    UNIT_STATE       m_eState;
    bool             m_bActiveJps = false;
    bool             m_bAttack = false;
    float            m_fDeltaTime = 0.f;
    float            m_fAccTime;
    float            m_fSpeed;
    Vec3             m_vDest;
    Vec3             m_vSource;
    CGameObject* m_pTargetObject;
    Ptr<CPrefab>     m_Prefab;
    vector<Vec3>     m_vecJps;
    wstring          m_strBackUp;
private:
    void SaveToFile(FILE* _File);
    void LoadFromFile(FILE* _File);
public:
    virtual void begin();
    virtual void tick();
    virtual void BeginOverlap(CCollider2D* _pOther);
    virtual void Overlap(CCollider2D* _pOther);
    virtual void EndOverlap(CCollider2D* _pOther);
    CLONE(CSniperScript);
private:
    bool Func(CGameObject* _lhs, CGameObject* _rhs);
    void ChaseEnemy();
    void ProcessEnemy();
    void SetMonsterHP();
public:
    virtual void PhaseEventOn() override;
    virtual void PhaseEventOff() override;
    virtual void Move(Int32 x, Int32 z) override;
    void Attck(Int32 x, Int32 z) {}
    void JpsAlgorithm(Int32 x, Int32 z);
    void SetDestPos(Vec3 _vPos);
public:
    CSniperScript();
    virtual ~CSniperScript();
};

