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
    float                        m_fDt = 0.f;
    Ptr<CTexture>                m_Atlas;
    Ptr<CTexture>                m_IconAtlas;
    Ptr<CTexture>                m_AtlasAlpha;
    array<array<tTapInfo, 6>, 6> m_arrTab;
    UINT                         m_iColumn;
    UINT                         m_iRow;
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
    void Release() { m_bClicked = false; }
    bool IsClicked() { return m_bClicked; }
    void SetColumn(UINT _iColumn) { m_iColumn = _iColumn;}
    UINT GetColumn() { return m_iColumn; }
    void SetRow(UINT _index) { m_iRow = _index; }
    UINT GetRow() { return m_iRow; }
public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
public:
    CLONE(CButtonScript);
public:
    CButtonScript();
    virtual ~CButtonScript();
};

