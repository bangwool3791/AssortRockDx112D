#pragma once
#include <Engine/CScript.h>

class CHuntScript :
    public CScript
{
private:
    BUILD_STATE                 m_eBuildState;
    Vec3                        m_vMousePos;
    CGameObject* m_pLevelMouseObject;
    CGameObject* m_pTileObject;
    bool                        m_bCheck[40000]{};
    vector<tBlock>              m_vecBlock{};
    UINT                        m_iIndex = 0;
    UINT                        m_iGold = 0;
private:
    vector<UINT> m_result{};
    vector<UINT> m_vec{};
    float m_fDt = 0.f;
    float m_fDt2 = 0.f;

    void SetTile(UINT _iTile);
    void SetTile(UINT _iTile, UINT _iValue);
    bool IsBlocked(UINT _iTile);
    void SetTileInfo(vector<UINT>& que, vector<UINT>& result, UINT _value);
public:
    virtual void begin();
    virtual void tick();
    virtual void finaltick();
    virtual void BeginOverlap(CCollider2D* _pOther);
    virtual void Overlap(CCollider2D* _pOther);
    virtual void EndOverlap(CCollider2D* _pOther);

public:
    UINT GetGold() { return m_iGold; }
    void SetGold(UINT _iGold) { m_iGold = _iGold; }
    BUILD_STATE GetState() { return m_eBuildState; }
    void clear();
private:
    int m_iHp = 100;
public:
    int GetHp() { return m_iHp; }
    void SetHp(int _iHp) { m_iHp = _iHp; }
public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
public:
    CLONE(CHuntScript);
public:
    CHuntScript();
    virtual ~CHuntScript();
};

