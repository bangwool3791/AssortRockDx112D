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
    wstring                          m_strKey;

public:
    virtual void finaltick() override;
public:
    const vector<tAnim2DFrm>& GetFames() { return m_pCurAnim->GetFames(); }
    const tAnim2DInfo& GetAniInfo();
    const tAnim2DFrm& GetAniFrame();
    tAnim2DFrm GetAniFrameCopy();

    /*
    Vec2 vLeftTop;
	Vec2 vSlice;
	Vec2 vOffset;
	Vec2 vFullSize;
	float fDuration;
    */
    void SetLeftTop(Vec2 _vLeftTop, int _index);
    void SetSlice(Vec2 _vSlice, int _index);
    void SetOffset(Vec2 _vOffset, int _index);
    void SetFullSize(Vec2 _vFullSize, int _index);
    void SetDuration(float _fDuration, int _index);
public:
    void CreateAnimation(const wstring& _strKey, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, float _fStep, int _iMaxFrm, float _FPS);
    CAnimation2D* FindAnimation(const wstring& _strKey);

    void Play(const wstring& _strKey, bool _bRepeat);

    void UpdateData();
    void Clear();

    Ptr<CTexture> GetTexture() { return m_pCurAnim->GetTexture(); }
public:
    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);
    CLONE(CAnimator2D);

public:
    CAnimator2D();
    CAnimator2D(const CAnimator2D& _rhs);
    virtual ~CAnimator2D();
};

