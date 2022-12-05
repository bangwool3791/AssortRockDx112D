#pragma once
#include "ComponentUI.h"

#include <Engine\Ptr.h>
#include <Engine\CTexture.h>
#include <Engine\CMesh.h>
#include <Engine\CMaterial.h>

class CAnimator2D;

class Animator2DUI :
    public ComponentUI
{
private:
    string                m_strRes;
    string                m_strPause;
    Ptr<CTexture>         m_pTexture;
    vector<string>        m_items;
    CAnimator2D*          m_pAnimator;
    tAnim2DFrm            m_tInfo;
    vector<tAnim2DFrm>    m_vecAniInfo;
    int                   m_iIndex;

    ImVec2                m_vMouseStart;
    ImVec2                m_vMouseEnd;
public:
    virtual void begin() override;
    virtual void update() override;
    virtual void render_update() override;

private:

public:
    Animator2DUI();
    ~Animator2DUI();
};

