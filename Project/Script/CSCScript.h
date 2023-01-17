#pragma once
#include <Engine/CScript.h>

class CSCScript :
    public CScript
{
private:
    BUILD_STATE                 m_eBuildState;
    Vec3                        m_vMousePos;
    CGameObject* m_pLevelMouseObject;
    CGameObject* m_pTileObject;
    bool                        m_bCheck[40000]{};
    UINT                        m_iIndex = 0;
    UINT                        m_iGold = 0;
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
    UINT GetGold() { return m_iGold; }
    void SetGold(UINT _iGold) { m_iGold = _iGold; }
public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
public:
    CLONE(CSCScript);
public:
    CSCScript();
    virtual ~CSCScript();
};

