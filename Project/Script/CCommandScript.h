#pragma once
#include <Engine/CScript.h>

class CCommandScript :
    public CScript
{
private:
    BUILD_STATE                  m_eBuildState;
    Vec3                         m_vMousePos;
    CGameObject*                 m_pLevelMouseObject;
    CGameObject*                 m_pTileObject;
    bool                         m_bCheck[40000]{};
    vector<tBlock>               m_vecBlock{};
    void SetTileInfo(queue<UINT>& que, queue<UINT>& result, UINT data);
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
private:
    int m_iHp = 100;
public:
    int GetHp() { return m_iHp; }
    void SetHp(int _iHp) { m_iHp = _iHp; }
public:
    BUILD_STATE GetState() { return m_eBuildState; }
public:
    CLONE(CCommandScript);
public:
    CCommandScript();
    virtual ~CCommandScript();
};

