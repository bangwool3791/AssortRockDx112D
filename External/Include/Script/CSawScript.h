#pragma once
#include <Engine/CScript.h>

class CSawScript :
    public CScript
{
private:
    BUILD_STATE                 m_eBuildState;
    Vec3                        m_vMousePos;
    CGameObject* m_pTileObject;
    bool                        m_bCheck[40000]{};
    UINT                        m_iIndex = 0;
    bool                        m_bMask = false;
    queue<UINT> m_result{};
    queue<UINT> m_queue{};
    float m_fDt = 0.f;
    float m_fDt2 = 0.f;
    void SetTileInfo(UINT _iTile, UINT _iValue);
    void SetTile(UINT _iTile, UINT _iValue);
    void SetTileInfo(queue<UINT>& que, queue<UINT>& result, UINT _value);
    bool IsBlocked(UINT _iTile);
    virtual void begin();
    virtual void tick();
    virtual void finaltick();
    virtual void BeginOverlap(CCollider2D* _pOther) {}
    virtual void Overlap(CCollider2D* _pOther) {}
    virtual void EndOverlap(CCollider2D* _pOther) {}
public:
    CLONE(CSawScript);
public:
    CSawScript();
    virtual ~CSawScript();
};

