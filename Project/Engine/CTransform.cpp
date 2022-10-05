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
	// 키 입력으로 삼각형 움직이기
	if (KEY_PRESSED(KEY::LEFT))
	{
		m_vRelativePos.x -= DT * 1.f;
	}

	if (KEY_PRESSED(KEY::RIGHT))
	{
		m_vRelativePos.x += DT * 1.f;
	}

	if (KEY_PRESSED(KEY::UP))
	{
		m_vRelativePos.y += DT * 1.f;
	}

	if (KEY_PRESSED(KEY::DOWN))
	{
		m_vRelativePos.y -= DT * 1.f;
	}
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
