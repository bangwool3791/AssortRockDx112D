#include "pch.h"

#include "CTexture.h"
#include "CDevice.h"
#include "CConstBuffer.h"
#include "CAnimator2D.h"
#include "CAnimation2D.h"

CAnimator2D::CAnimator2D()
    :CComponent(COMPONENT_TYPE::ANIMATOR2D)
    , m_bRepeat(false)
    , m_mapAnim{}
    , m_strKey{}
{
}

CAnimator2D::CAnimator2D(const CAnimator2D& _rhs)
    :CComponent(COMPONENT_TYPE::ANIMATOR2D)
    , m_bRepeat(_rhs.m_bRepeat)
    , m_mapAnim{}
    , m_strKey{_rhs.m_strKey}
{
    CAnimation2D* pAnimation2D{};

    for (auto iter = _rhs.m_mapAnim.begin(); iter != _rhs.m_mapAnim.end(); ++iter)
    {
        pAnimation2D = nullptr;
        m_mapAnim.insert(make_pair(iter->first, pAnimation2D = iter->second->Clone()));
        pAnimation2D->SetOwner(this);
    }

    Play(m_strKey, m_bRepeat);
}

CAnimator2D::~CAnimator2D()
{
    Safe_Del_Map(m_mapAnim);
}

void CAnimator2D::CreateAnimation(const wstring& _strKey, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, float _fStep, int _iMaxFrm, float _FPS)
{
    assert(_AtlasTex.Get());

    CAnimation2D* pAnimation2D;

    pAnimation2D = FindAnimation(_strKey);
    assert(!pAnimation2D);

    pAnimation2D = new CAnimation2D;
    pAnimation2D->Create(_strKey, _AtlasTex, _vLeftTop, _vSlice, _fStep, _iMaxFrm, _FPS);
    pAnimation2D->SetName(_strKey);
    pAnimation2D->m_pOwner = this;
    m_mapAnim[_strKey] = pAnimation2D;
}

void CAnimator2D::finaltick()
{
    if (!IsValid(m_pCurAnim))
        return;

    if (m_pCurAnim->m_bFinish && m_bRepeat)
    {
        m_pCurAnim->Reset();
    }

    m_pCurAnim->finaltick();
}


CAnimation2D* CAnimator2D::FindAnimation(const wstring& _strKey)
{
    CAnimation2D* iter = m_mapAnim[_strKey];

    if (iter == nullptr)
        return nullptr;

    return iter;
}

void CAnimator2D::Play(const wstring& _strKey, bool _bRepeat)
{
    m_strKey = _strKey;
    CAnimation2D* pAnimation = FindAnimation(_strKey);

    assert(pAnimation);

    if (IsValid(pAnimation))
    {
        m_pCurAnim = pAnimation;
    }
    m_bRepeat = _bRepeat;
    pAnimation->Reset();
}

void CAnimator2D::UpdateData()
{
    if (!IsValid(m_pCurAnim))
        return;

    if(m_bActive)
        m_pCurAnim->UpdateData();
}

void CAnimator2D::Clear()
{
    if (!IsValid(m_pCurAnim))
        return;

    m_pCurAnim->Clear();
}


int  CAnimator2D::Add_Animation2D(Vec2 _vLeftTop, Vec2 _vSlice, float _FPS, Vec2 _vFullSize)
{
    return m_pCurAnim->Add_Animation2D(_vLeftTop, _vSlice, _FPS, _vFullSize);
}

int CAnimator2D::Delete_Animation2D()
{
    return m_pCurAnim->Delete_Animation2D();
}

void CAnimator2D::SaveToFile(FILE* _File)
{
    COMPONENT_TYPE type = GetType();
    fwrite(&type, sizeof(UINT), 1, _File);

    // Animation ����
    size_t iAnimCount = m_mapAnim.size();
    fwrite(&iAnimCount, sizeof(size_t), 1, _File);

    for (const auto& pair : m_mapAnim)
    {
        pair.second->SaveToFile(_File);
    }
}

void CAnimator2D::LoadFromFile(FILE* _File)
{
    // Animation
    size_t iAnimCount = 0;
    fread(&iAnimCount, sizeof(size_t), 1, _File);

    for (size_t i = 0; i < iAnimCount; ++i)
    {
        CAnimation2D* pAnim = new CAnimation2D;
        pAnim->LoadFromFile(_File);

        pAnim->m_pOwner = this;
        m_mapAnim.insert(make_pair(pAnim->GetName(), pAnim));
    }
}

void CAnimator2D::SetTexture(Ptr<CTexture> _AtlasTex)
{

    for (auto iter{ m_mapAnim.begin() }; iter != m_mapAnim.end(); ++iter)
    {
        iter->second->SetTexture(_AtlasTex);
    }
}

const tAnim2DInfo& CAnimator2D::GetAniInfo()
{
    return m_pCurAnim->GetAniInfo();
}

const tAnim2DFrm& CAnimator2D::GetAniFrame()
{
    return m_pCurAnim->GetAniFrame();
}

void CAnimator2D::SetLeftTopX(float _fValue, int _index)
{
    m_pCurAnim->SetLeftTopX(_fValue, _index);
}

void CAnimator2D::SetLeftTopY(float _fValue, int _index)
{
    m_pCurAnim->SetLeftTopY(_fValue, _index);
}

void CAnimator2D::SetLeftTop(Vec2 _vLeftTop, int _index)
{
    m_pCurAnim->SetLeftTop(_vLeftTop, _index);
}

void CAnimator2D::SetSliceX(float _fValue, int _index)
{
    m_pCurAnim->SetSliceX(_fValue, _index);
}

void CAnimator2D::SetSliceY(float _fValue, int _index)
{
    m_pCurAnim->SetSliceY(_fValue, _index);
}

void CAnimator2D::SetSlice(Vec2 _vSlice, int _index)
{
    m_pCurAnim->SetSlice(_vSlice, _index);
}

void CAnimator2D::SetOffsetX(float _fValue, int _index)
{
    m_pCurAnim->SetOffsetX(_fValue, _index);
}

void CAnimator2D::SetOffsetY(float _fValue, int _index)
{
    m_pCurAnim->SetOffsetY(_fValue, _index);
}

void CAnimator2D::SetOffset(Vec2 _vOffset, int _index)
{
    m_pCurAnim->SetOffset(_vOffset, _index);
}

void CAnimator2D::SetFullSizeX(float _fValue, int _index)
{
    m_pCurAnim->SetFullSizeX(_fValue, _index);
}

void CAnimator2D::SetFullSizeY(float _fValue, int _index)
{
    m_pCurAnim->SetFullSizeY(_fValue, _index);
}

void CAnimator2D::SetFullSize(Vec2 _vFullSize, int _index)
{
    m_pCurAnim->SetFullSize(_vFullSize, _index);
}

void CAnimator2D::SetDuration(float _fDuration, int _index)
{
    m_pCurAnim->SetDuration(_fDuration, _index);
}
