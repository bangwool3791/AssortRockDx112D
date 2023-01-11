#pragma once
#include <Engine/CScript.h>

struct tTapInfo
{
    Ptr<CTexture> Texture;
    Vec2		  vUV{};
    Vec2		  vSlice{};

    Vec2          vUV2{};
    Vec2          vSlice2{};
    TAP_CATEGORY  Category{};
};

class CButtonScript :
    public CScript
{
private:
    Ptr<CTexture>                m_Atlas;
    Ptr<CTexture>                m_IconAtlas;
    Ptr<CTexture>                m_AtlasAlpha;
    array<array<tTapInfo, 6>, 6> m_arrTab;
    UINT                         m_iColum;
    UINT                         m_iIndex;
    CGameObject*                 m_pUiCameraObject;
    CGameObject*                 m_pLevelMouseObject;

    Vec3                         m_vMousePos;

    bool                         m_bClicked;
public:
    virtual void begin();
    virtual void tick();
    virtual void finaltick();
    virtual void BeginOverlap(CCollider2D* _pOther);
    virtual void Overlap(CCollider2D* _pOther);
    virtual void EndOverlap(CCollider2D* _pOther);

public :
    void SetTapIndex(UINT _index) { m_iIndex = _index; }
    void Release() { m_bClicked = false; }
    bool IsClicked() { return m_bClicked; }
public:
    CLONE(CButtonScript);
public:
    CButtonScript();
    virtual ~CButtonScript();
};

