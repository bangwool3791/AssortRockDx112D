#pragma once
#include <Engine/CScript.h>

class CQuarryScript :
    public CScript
{
private:
    BUILD_STATE                 m_eBuildState;
    Vec3                        m_vMousePos;
    CGameObject*                m_pTileObject;
    bool                        m_bCheck[40000]{};
    vector<tBlock>              m_vecBlock{};
    UINT                        m_iIndex = 0;
    vector<tTile>               m_vecMask{};
    vector<UINT>                m_result{};
    vector<UINT>                m_queue{};
    float m_fDt = 0.f;
    float m_fDt2 = 0.f;
    void SetTileInfo(UINT _iTile, UINT _iValue);
    void SetTile(UINT _iTile, UINT _iValue);
    void SetTileInfo(vector<UINT>& que, vector<UINT>& result, UINT _value);
    bool IsBlocked(UINT _iTile);
public :
    virtual void begin();
    virtual void tick();
    virtual void finaltick();
    virtual void BeginOverlap(CCollider2D* _pOther) {}
    virtual void Overlap(CCollider2D* _pOther) {}
    virtual void EndOverlap(CCollider2D* _pOther) {}
private:
    int m_iHp = 100;
public:
    int GetHp() { return m_iHp; }
    void SetHp(int _iHp) { m_iHp = _iHp; }
public:
    void clear();
    BUILD_STATE GetState() { return m_eBuildState; }
public:
    CLONE(CQuarryScript);
public:
    CQuarryScript();
    virtual ~CQuarryScript();
};

