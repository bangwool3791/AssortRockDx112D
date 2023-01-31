#pragma once
#include <Engine/CScript.h>

class CGameObjectEx;



class CInterfaceScript :
    public CScript
{
private:
    bool                      m_bUiClicked = false;
    bool                      m_bActiveDrag = false;
    bool                      m_bActiveMouse = false;
    bool                      m_bChecked = false;
    UINT                      m_id;
    CGameObject*              m_pTarget;
    CGameObject*              m_pCameraObject;
    CGameObject*              m_pMouseObject;
    CGameObject*              m_pDestObject;
    CGameObject*              m_pRallyPoint{};
    CGameObject*              m_pTile;
    vector<CGameObject*>	  m_vecDragObj;
    CGameObject*              m_pDragObj;
    array<CGameObject*, 6>    m_arrTapButton;
    pair<Vec3, Vec3>          m_pairPoint;
    UINT                      m_iIndex = 0;
public:
    virtual void begin();
    virtual void tick();
    virtual void finaltick();
    virtual void BeginOverlap(CCollider2D* _pOther);
    virtual void Overlap(CCollider2D* _pOther);
    virtual void EndOverlap(CCollider2D* _pOther);

public :
    void Add_TapButton(CGameObject* _pGameobject)
    {
        m_arrTapButton[m_iIndex] = _pGameobject;
        ++m_iIndex;
    }
public:
    CLONE(CInterfaceScript);
public:
    CInterfaceScript();
    virtual ~CInterfaceScript();
};

