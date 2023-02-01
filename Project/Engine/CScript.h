#pragma once
#include "CComponent.h"

#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CResMgr.h"
#include "GlobalComponent.h"

enum class SCRIPT_PARAM
{
    INT,
    FLOAT,
    VEC2,
    VEC3,
    VEC4,
    TEXTURE,
    MATERIAL,
    PREFAB,
    SOUND,
};

struct tScriptParam
{
    string strParamName;
    SCRIPT_PARAM Type;
    void* pParam;

};
class CScript :
    public CComponent
{
private:
    const int               m_iScriptType;
    vector<tScriptParam>    m_vecParam;
protected:
    bool m_bDesc = false;
    BUILD_STATE m_eBuildState;
    CGameObject* m_pPortrait;
    vector<CGameObject*>    m_vecIcon;
public:
    int GetScriptType() { return m_iScriptType; }
protected:
    bool                    m_bActive;
    Vec3                    m_vTarget;
    int                     m_iGold;
    int                     m_iWorker;
    int                     m_iFood;
    int                     m_iStorage;
    int                     m_iWood;
    int                     m_iColony;
    int                     m_iIron;

    int                     m_iAttack;
    int                     m_iArmor;
    int                     m_iSpeed;
    int                     m_iAttackSpeed;
    int                     m_iAttackRange;
    float                   m_fHP;
    float                   m_fFullHp;

    int                     m_iGoldOut;
    int                     m_iWoodOut;
    int                     m_iIronOut;
protected:
    void SetIconUI(int iValue, UINT idx);
    virtual bool Create(const wstring& _str);
    virtual bool Create(const wstring& _str, Vec3 vPos);
public:
    void Set_Target(Vec3 _vTarget) { m_vTarget = _vTarget; }
    void AddScriptParam(SCRIPT_PARAM _eParam, const string& _name, void* _pParam)
    {
        m_vecParam.push_back(tScriptParam{ _name, _eParam ,_pParam });
    }
    const vector<tScriptParam>& GetScriptParam() { return m_vecParam; }

    void Activate() { m_bActive = true; }
    void Deactivate() { m_bActive = false; }
    bool IsActive() { return m_bActive; }

    float GetHp() { return m_fHP; }
    void SetHp(float _iHp) { m_fHP = _iHp; }

    int GetGold() { return m_iGold; }
    int GetWood() { return m_iWood; }
    int GetIron() { return m_iIron; }
    void SetHp(UINT _iGold) { m_iGold = _iGold; }

    float GetFullHp() { return m_fFullHp; }
    void SetFullHp(float _iGold) { m_fFullHp = _iGold; }

    int GetGoldOut() { return m_iGoldOut; }
    int GetWoodOut() { return m_iWoodOut; }
    int GetIronOut() { return m_iIronOut; }

    int GetWorker() { return m_iWorker; }
    int GetFood() { return m_iFood; }
    int GetColony() { return m_iColony; }
public:
    virtual void tick();
    virtual void finaltick() {};

    virtual CScript* Clone() = 0;
public:
    CScript(int _iScript);
    virtual ~CScript();

public:
    virtual void BeginOverlap(CCollider2D* _pOther) = 0;
    virtual void Overlap(CCollider2D* _pOther) = 0;
    virtual void EndOverlap(CCollider2D* _pOther) = 0;
    void ActiveIcon();
    void DeactiveIcon();
    virtual void PhaseEventOn();
    virtual void PhaseEventOff();
    virtual void Move(Int32 x, Int32 z) {};
    virtual void clear() {};
};

