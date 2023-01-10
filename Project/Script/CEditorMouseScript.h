#pragma once
#include <Engine\CScript.h>
class CEditorMouseScript :
    public CScript
{
public:
    virtual void begin();
    virtual void tick();
    virtual void finaltick();

    virtual void BeginOverlap(CCollider2D* _pOther);
    virtual void Overlap(CCollider2D* _pOther);
    virtual void EndOverlap(CCollider2D* _pOther);

    CLONE(CEditorMouseScript);

private:
    CGameObject*            m_pCameraObejct;
    float                   m_fCameraScale;
    Vec3                    m_vCameraPos;
    Vec2                    m_vRenderResolution;
    Ray                     m_Ray;
public:
    const Ray&              GetRay() { return m_Ray; }
public :
    void Initialize(void* pAddr);
public:
    CEditorMouseScript();
    virtual ~CEditorMouseScript();

};

