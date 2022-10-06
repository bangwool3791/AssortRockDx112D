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
	CConstBuffer* pConstBuffer = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);

	pConstBuffer->SetData(m_vRelativePos);
	pConstBuffer->UpdateData(PIPELINE_STAGE::VS);
}
