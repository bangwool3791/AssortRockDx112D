#pragma once
#include "ComponentUI.h"

#include <Engine\Ptr.h>
#include <Engine\CKeyMgr.h>
#include <Engine\CTexture.h>
#include <Engine\CMesh.h>
#include <Engine\CMaterial.h>

#include "ListUI.h"
#include "imgui_filedialog.h"
class CGameObjectEx;
class CAnimator2D;

class Animator2DUI :
    public ComponentUI
{
    //정리 필요
private:
    char                  m_szAnimation[128];
    string                m_strAnimationName;
    Ptr<CTexture>         m_pAtlasTexture;

    vector<tAnim2DFrm>    m_vecFrames;
    CAnimator2D*          m_pAnimator;
    vector<tAnim2DFrm>    m_vecAniInfo;
    int                   m_iCurIdx;
    int                   m_iSelectedIdx;

    bool                  m_fileDialogOpen{};
    ImFileDialogInfo      m_fileDialogInfo;

    bool**                m_dfs_visited;
    bool**                m_bfs_visited;
private:
    CGameObjectEx*        m_pGameObject;
    CGameObjectEx*        m_MouseObject;
    CGameObjectEx*        m_pCameraObject;
    CGameObjectEx*        m_pPointObject;

    ImTextureID           m_pImage;
    vector<string>        m_vecAnimation;
    vector<string>        m_vecFrameIndex;
private:
    float                 m_fMinX;
    float                 m_fMinY;
                          
    float                 m_fMaxX;
    float                 m_fMaxY;

    Vec2                  m_vLeftTop;
    Vec2                  m_vRightBottom;
    Vec2                  m_vFullSize;
    Vec2                  m_vSlice;
    Vec2                  m_vUV;
    Vec2                  m_vOffset;

    Vec2                  m_uvLeftTop;
    Vec2                  m_uvRightBottom;
    Vec2                  m_uvFullSize;
    Vec2                  m_uvSlice;
    Vec2                  m_uvUV;
    Vec2                  m_uvOffset;
    float                 m_fDuration;

    Vec2                  m_uvDiff;
private:
    int                   m_iPixel_Width;
    int                   m_iPixel_Height;
   // vector<unsigned char> m_vec_pixel;
    uint8_t*              m_pPixels;
    //
    void Initialize_Aimation_Pixel();
    void Initialize_Animation_Info();
    void SetTextureUI();
    void Set_Texture_Pixel(UINT x, UINT y);
    Vec2 Get_Pixel_Bfs(UINT x, UINT y);
    void Render_Text(ImVec4 _vColor, Vec2 _vSize, string _str);
    void Render_Pos();
    void Refresh_Animation(float width, float height);
    void Clear_Visited(bool** _visited, size_t width, size_t height);
    bool Click_Pixel_LBtn();
    void Click_Pixel_KeyBoard(KEY _key);
    bool Load_Image(std::vector<unsigned char>& image, const std::string& filename, int& x, int& y);
    tRGBA GetRGBA(int _x, int _y);
public:
    virtual void begin() override;
    virtual void update() override;
    virtual void render_update() override;

private:

public:
    Animator2DUI();
    ~Animator2DUI();
};

