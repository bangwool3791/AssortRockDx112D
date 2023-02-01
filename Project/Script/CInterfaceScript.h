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
    const int   m_iGoldTotal = 1000;
    const int   m_iWoodTotal = 1000;
    const int   m_iIronTotal = 1000;
    float                     m_fDeltaTime = 60.f;
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

    CGameObject* m_pGoldBar{};
    CGameObject* m_pWoodBar{};
    CGameObject* m_pIronBar{};
private:
    void Resource();
    void ProgressBar(int _iValue, int iTotal, CGameObject* pProObj);
    void BarText();
    void IncText();
public:
    virtual void begin();
    virtual void tick();
    virtual void finaltick();
    virtual void BeginOverlap(CCollider2D* _pOther);
    virtual void Overlap(CCollider2D* _pOther);
    virtual void EndOverlap(CCollider2D* _pOther);
    virtual bool Create(const wstring& _str);
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

