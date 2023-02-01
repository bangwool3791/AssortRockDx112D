#pragma once
#include <Engine/CScript.h>

class CHillScript :
    public CScript
{
private:
    bool                        m_bCheck[40000]{};
    UINT                        m_iIndex = 0;
    Vec3                        m_vMousePos;
    CGameObject* m_pTileObject;
    void SetTileInfo(UINT _iTile, UINT _iValue);
    void SetTile(UINT _iTile, UINT _iValue);
    bool IsBlocked(UINT _iTile);
    void SetTileInfo(queue<UINT>& que, queue<UINT>& result, UINT value);
    virtual void begin();
    virtual void tick();
    virtual void finaltick();
    virtual void BeginOverlap(CCollider2D* _pOther);
    virtual void Overlap(CCollider2D* _pOther);
    virtual void EndOverlap(CCollider2D* _pOther);
public:
    CLONE(CHillScript);
public:
    CHillScript();
    virtual ~CHillScript();
};

