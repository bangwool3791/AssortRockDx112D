#pragma once
#include <Engine\CScript.h>
class CMouseScript :
    public CScript
{
public:
    virtual void begin();
    virtual void tick();
    virtual void finaltick();

    virtual void BeginOverlap(CCollider2D* _pOther);
    virtual void Overlap(CCollider2D* _pOther);
    virtual void EndOverlap(CCollider2D* _pOther);

    CLONE(CMouseScript);

private:
    bool bClicked;
    Vec2 StartPos;
    Vec2 EndPos;
    vector<CGameObject*>    m_vecUnit;
    CGameObject*            m_pCamera;
    CGameObject*            m_pUiCamera;
    CGameObject*            m_pTerrainObject;
    float                   m_fCameraScale;
    Vec3                    m_vCameraPos;

    Vec3                    m_vMousePos;
    Vec3                    m_vTarget;
    Ray                     m_uiRay;
    Ray                     m_Ray;
public :
    const Ray& GetUiRay() { return m_uiRay; }
    const Ray& GetRay() { return m_Ray; }
    Vec3  GetMousePos() { return m_vMousePos; }
public:
    CMouseScript();
    virtual ~CMouseScript();

};

