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
	//���� ���� �� ������ ���� ����
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
* Light2D Component final Tick���� Update�� m_vecLight2D �����͸� ����ü ���� ����
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
	* ����ü ���۾ȿ�, ���� ������ �Է��Ѵ�.
	*/
	m_pLight2DBuffer->SetData(m_vecLight2D.data(), m_vecLight2D.size());

	m_pLight2DBuffer->UpdateData(13, PIPELINE_STAGE::VS | PIPELINE_STAGE::PS);

	g_global.iLight2DCount = (UINT)m_vecLight2D.size();
}
