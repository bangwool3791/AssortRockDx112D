#include "pch.h"

#include "CDevice.h"
#include "CRenderMgr.h"
#include "CCamera.h"

#include "CConstBuffer.h"
#include "CStructuredBuffer.h"

CRenderMgr::CRenderMgr()
	:m_pLight2DBuffer{}
{
	m_pLight2DBuffer = new CStructuredBuffer;
	m_pLight2DBuffer->Create(sizeof(tLightInfo), 2, SB_TYPE::NONE, nullptr);
}

CRenderMgr::~CRenderMgr()
{
	Safe_Delete(m_pLight2DBuffer);
}

void CRenderMgr::init()
{
}
	
void CRenderMgr::tick()
{
	m_vecCam.clear();
	//광원 정보 매 프레임 마다 삭제
	m_vecLight2D.clear();
}

void CRenderMgr::render()
{
	UpdateLight2D();

	static CConstBuffer* pGlobalCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL);
	pGlobalCB->SetData(&g_global);
	pGlobalCB->UpdateData(PIPELINE_STAGE::ALL_STAGE);
	for (auto elem{ m_vecCam.begin() }; elem != m_vecCam.end(); ++elem)
	{
		(*elem)->render();
	}
}

/*
* Light2D Component final Tick에서 Update된 m_vecLight2D 데이터를 구조체 버퍼 저장
*/
void CRenderMgr::UpdateLight2D()
{
	/*
	* 
	*/
	if (m_pLight2DBuffer->GetElementsCount() <= m_vecLight2D.size())
	{
		m_pLight2DBuffer->Create(m_pLight2DBuffer->GetElementsSize(), m_vecLight2D.size(), SB_TYPE::NONE, nullptr);
	}
		
	/*
	* 구조체 버퍼안에, 조명 정보를 입력한다.
	*/
	m_pLight2DBuffer->SetData(m_vecLight2D.data(), m_vecLight2D.size());

	m_pLight2DBuffer->UpdateData(13, PIPELINE_STAGE::VS | PIPELINE_STAGE::PS);

	g_global.iLight2DCount = (UINT)m_vecLight2D.size();
}
