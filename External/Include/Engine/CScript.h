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
private :
    const int m_iScriptType;
    vector<tScriptParam>    m_vecParam;
public :
    int GetScriptType() { return m_iScriptType; }
protected:
    Vec3  m_vTarget;
public :
    void Set_Target(Vec3 _vTarget) { m_vTarget = _vTarget; }
    void AddScriptParam(SCRIPT_PARAM _eParam, const string& _name, void* _pParam)
    {
        m_vecParam.push_back(tScriptParam{ _name, _eParam ,_pParam });
    }
    const vector<tScriptParam>& GetScriptParam() { return m_vecParam; }
public :
    virtual void tick() = 0;
    virtual void finaltick() {};

    virtual CScript* Clone() = 0;
public :
    CScript(int _iScript);
    virtual ~CScript();

public:
    virtual void BeginOverlap(CCollider2D* _pOther) = 0;
    virtual void Overlap(CCollider2D* _pOther) = 0;
    virtual void EndOverlap(CCollider2D* _pOther) = 0;
};

