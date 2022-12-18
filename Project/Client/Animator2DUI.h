#pragma once
#include "ComponentUI.h"

#include <Engine\Ptr.h>
#include <Engine\CTexture.h>
#include <Engine\CMesh.h>
#include <Engine\CMaterial.h>

class CGameObjectEx;
class CAnimator2D;

class Animator2DUI :
    public ComponentUI
{
    //정리 필요
private:
    string                m_strRes;
    string                m_strPause;
    Ptr<CTexture>         m_pTexture;
    vector<string>        m_items;


    CAnimator2D*          m_pAnimator;
    tAnim2DFrm            m_tInfo;
    vector<tAnim2DFrm>    m_vecAniInfo;
    int                   m_iIndex;

    Vec2                  m_vMouseStart;
    Vec2                  m_vMouseEnd;

private:
    CGameObjectEx*        m_MouseObject;
    CGameObjectEx*        m_CameraObject;
private:
    int m_pixel_width;
    int m_pixel_height;
   // vector<unsigned char> m_vec_pixel;
    uint8_t* m_pixel;
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

