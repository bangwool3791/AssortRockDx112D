#include "pch.h"
#include "CTransform.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CDevice.h"
#include "CConstBuffer.h"

CTransform::CTransform()
	:CComponent{COMPONENT_TYPE::TRANSFORM}
{

}

CTransform::~CTransform()
{
}

void CTransform::tick()
{
}

void CTransform::finaltick()
{
}

void CTransform::UpdateData()
{
	CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);

	tTransform transform = {};
	transform.matWorld = m_matWorld;
	transform.matWV;
	transform.matWVP;


	pCB->SetData(&transform);
	pCB->UpdateData(PIPELINE_STAGE::VS);
}
