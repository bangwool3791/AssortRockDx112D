#include "pch.h"
#include "CAnimation2D.h"

#include "CDevice.h"

#include "CTimeMgr.h"
#include "CConstBuffer.h"

CAnimation2D::CAnimation2D()
	: m_iCurIdx(-1)
	, m_pOwner(nullptr)
	, m_fAccTime(0.f)
{
}

CAnimation2D::~CAnimation2D()
{

}

void CAnimation2D::finaltick()
{
	if (m_bFinish)
		return;

	m_fAccTime += DT;

	if (m_vecFrm[m_iCurIdx].fDuration < m_fAccTime)
	{
		m_fAccTime = 0;
		++m_iCurIdx;

		if (m_iCurIdx >= m_vecFrm.size())
		{
			m_iCurIdx = (int)m_vecFrm.size() - 1;
			m_bFinish = true;
		}

	}
}

void CAnimation2D::Create(const wstring& _strKey, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, float _fStep, int _iMaxFrm, float _FPS)
{
	SetName(_strKey);

	m_AtlasTex = _AtlasTex;

	float fWidth	= (float)_AtlasTex->GetWidth();
	float fHeight	= (float)_AtlasTex->GetHeight();

	for (UINT i{ 0 }; i < _iMaxFrm; ++i)
	{
		tAnim2DFrm frm{};
		frm.vLeftTop	= Vec2{ (_vLeftTop.x + _fStep * (float)i) / fWidth, _vLeftTop.y / fHeight };
		frm.vSlice		= Vec2{ _vSlice.x / fWidth, _vSlice.y / fHeight };
		frm.fDuration = 1.f / _FPS;
		frm.vFullSize	= Vec2{ 200.f / fWidth, 200.f / fHeight };
		m_vecFrm.push_back(frm);
	}
}

void CAnimation2D::UpdateData()
{
	m_AtlasTex->UpdateData(59, PIPELINE_STAGE::PS);

	static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::ANIMATION2D);

	m_info.iAnim2DUse		= 1;
	m_info.vLeftTop			= m_vecFrm[m_iCurIdx].vLeftTop;
	m_info.vSlice			= m_vecFrm[m_iCurIdx].vSlice;
	m_info.vOffset			= m_vecFrm[m_iCurIdx].vOffset;
	m_info.vFullSize		= m_vecFrm[m_iCurIdx].vFullSize;
	
	pCB->SetData(&m_info);
	pCB->UpdateData(PIPELINE_STAGE::PS);
}

void CAnimation2D::Clear()
{
	CTexture::Clear(59);

	static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::ANIMATION2D);

	memset(&m_info, 0, sizeof(tAnim2DInfo));

	pCB->SetData(&m_info);
	pCB->UpdateData(PIPELINE_STAGE::PS);
}

void CAnimation2D::SetLeftTop(Vec2 _vLeftTop, int _index)
{
	m_vecFrm[_index].vLeftTop = _vLeftTop;
}

void CAnimation2D::SetSlice(Vec2 _vSlice, int _index)
{
	m_vecFrm[_index].vSlice = _vSlice;
}

void CAnimation2D::SetOffset(Vec2 _vOffset, int _index)
{
	m_vecFrm[_index].vOffset = _vOffset;
}

void CAnimation2D::SetFullSize(Vec2 _vFullSize, int _index)
{
	m_vecFrm[_index].vFullSize = _vFullSize;
}

void CAnimation2D::SetDuration(float _fDuration, int _index)
{
	m_vecFrm[_index].fDuration = _fDuration;
}