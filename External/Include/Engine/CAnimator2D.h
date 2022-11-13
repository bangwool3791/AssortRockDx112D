#pragma once
#include "CComponent.h"
#include "CAnimation2D.h"
#include "CTexture.h"

class CAnimator2D :
    public CComponent
{
private:
    std::map<wstring, CAnimation2D*> m_mapAnim;
    CAnimation2D*                    m_pCurAnim;

    bool                             m_bRepeat;

public:
    virtual void finaltick() override;
public:
    const tAnim2DInfo& GetAniInfo();
public:
    void CreateAnimation(const wstring& _strKey, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, float _fStep, int _iMaxFrm, float _FPS);
    CAnimation2D* FindAnimation(const wstring& _strKey);

    void Play(const wstring& _strKey, bool _bRepeat);

    void UpdateData();
    void Clear();

public:
    CLONE(CAnimator2D);

public:
    CAnimator2D();
    virtual ~CAnimator2D();
};

