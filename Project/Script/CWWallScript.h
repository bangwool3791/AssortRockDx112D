#pragma once
#include <Engine/CScript.h>

class CWWallScript :
    public CScript
{
private:
    BUILD_STATE                 m_eBuildState;
    Vec3                        m_vMousePos;
    CGameObject*                m_pLevelMouseObject;
    CGameObject*                m_pTileObject;
    bool                        m_bCheck[40000]{};
    UINT                        m_iIndex = 0;

    float                       m_fDt = 0.f;
    float                       m_fDt2 = 0.f;

    Ptr<CMesh>                  m_pMesh;
    static array<tWoodWall, 200 * 200> m_arr;
    void SetTileInfo(UINT _iTile);
    bool IsBlocked(UINT _iTile);
    bool Node(UINT _iTile, UINT _iIndex);
    void ChildWallProcess();
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
public:
    CLONE(CWWallScript);
public:
    CWWallScript();
    virtual ~CWWallScript();
};

