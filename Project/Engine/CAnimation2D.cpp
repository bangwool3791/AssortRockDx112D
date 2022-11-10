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
	m_AtlasTex->UpdateData(12, PIPELINE_STAGE::PS);

	static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::ANIMATION2D);

	tAnim2DInfo info{};
	info.iAnim2DUse		= 1;
	info.vLeftTop		= m_vecFrm[m_iCurIdx].vLeftTop;
	info.vSlice			= m_vecFrm[m_iCurIdx].vSlice;
	info.vOffset		= m_vecFrm[m_iCurIdx].vOffset;
	info.vFullSize		= m_vecFrm[m_iCurIdx].vFullSize;
	
	pCB->SetData(&info);
	pCB->UpdateData(PIPELINE_STAGE::PS);
}

void CAnimation2D::Clear()
{
	CTexture::Clear(12);

	static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::ANIMATION2D);

	tAnim2DInfo info = {};
	info.iAnim2DUse = 0;

	pCB->SetData(&info);
	pCB->UpdateData(PIPELINE_STAGE::PS);
}

