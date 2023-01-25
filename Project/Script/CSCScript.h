#pragma once
#include <Engine/CScript.h>

class CSCScript :
    public CScript
{
private:
    wstring                     m_strPrefab;
    BUILD_STATE                 m_eBuildState;
    Vec3                        m_vMousePos;
    CGameObject* m_pLevelMouseObject;
    CGameObject* m_pTileObject;
    CGameObject* m_pGameObject;
    CGameObject* m_pCircleArrow{};
    bool                        m_bCheck[40000]{};
    vector<tBlock>              m_vecBlock{};
    UINT                        m_iIndex = 0;

    float                       m_fDt  = 0.f;
    float                       m_fDt2 = 0.f;
    void SetTileInfo(UINT _iTile);
    void SetTile(UINT _iTile);
    void SetTileInfo(UINT _iTile, UINT _iValue);
    void SetTile(UINT _iTile, UINT _iValue);
    void RefreshTile(UINT _iTile);
    void RefreshTileInfo(UINT _iTile);
    bool IsBlocked(int _iTile);
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
public :
    void CreateUnit(const wstring& _str);
public:
    CLONE(CSCScript);
public:
    CSCScript();
    virtual ~CSCScript();
};

