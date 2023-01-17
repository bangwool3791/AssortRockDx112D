#pragma once
#include <Engine/CScript.h>

class CTreeScript :
    public CScript
{
private:
    BUILD_STATE                 m_eBuildState;
    Vec3                        m_vMousePos;
    CGameObject* m_pTileObject;
    bool                        m_bCheck[40000]{};
    UINT                        m_iIndex = 0;

    void SetTileInfo(UINT _iTile, UINT _iValue);
    void SetTile(UINT _iTile, UINT _iValue);
    bool IsBlocked(UINT _iTile);
    virtual void begin();
    virtual void tick();
    virtual void finaltick();
    virtual void BeginOverlap(CCollider2D* _pOther) {}
    virtual void Overlap(CCollider2D* _pOther) {}
    virtual void EndOverlap(CCollider2D* _pOther) {}
public:
    CLONE(CTreeScript);
public:
    CTreeScript();
    virtual ~CTreeScript();
};

