#include "pch.h"
#include "CTransform.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CDevice.h"
#include "CConstBuffer.h"

#include "extern.cpp"

CTransform::CTransform()
	:CComponent{COMPONENT_TYPE::TRANSFORM}
{
	m_vRelativeDir[(UINT)DIR::RIGHT]	= Vec3{ 1.0f, 0.0f, 0.0f };
	m_vRelativeDir[(UINT)DIR::UP]		= Vec3{ 0.0f, 1.0f, 0.0f };
	m_vRelativeDir[(UINT)DIR::FRONT]	= Vec3{ 0.0f, 0.0f, 1.0f };
}

CTransform::~CTransform()
{

}

void CTransform::tick()
{
	m_matWorld				= XMMatrixIdentity();
	Matrix		matScail	= XMMatrixScaling(m_vRelativeScale.x, m_vRelativeScale.y, m_vRelativeScale.z);
	Matrix		matPos		= XMMatrixTranslation(m_vRelativePos.x, m_vRelativePos.y, m_vRelativePos.z);
	//Degree ������ ���� �� ȸ�� x,y,z ������ ��Ŀ� ��´�.
	Matrix		matRot		= XMMatrixRotationX(m_vRelativeRotation.x);
	matRot					*= XMMatrixRotationY(m_vRelativeRotation.y);
	matRot					*= XMMatrixRotationZ(m_vRelativeRotation.z);

	//��Ŀ� ��� x,y,z�࿡ ���� ȸ�� ����(Vector3)�� ��´�.
	m_vRelativeDir[(UINT)DIR::RIGHT]	= XMVector3TransformNormal(Vec3{ 1.f, 0.f, 0.f }, matRot);
	m_vRelativeDir[(UINT)DIR::UP]		= XMVector3TransformNormal(Vec3{ 0.f, 1.f, 0.f }, matRot);
	m_vRelativeDir[(UINT)DIR::FRONT]	= XMVector3TransformNormal(Vec3{ 0.f, 0.f, 1.f }, matRot);
	m_matWorld = matScail * matPos * matRot;
}

void CTransform::finaltick()
{
}

void CTransform::UpdateData()
{
	CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);

	g_transform.matWorld = m_matWorld;
	pCB->SetData(&g_transform);
	pCB->UpdateData(PIPELINE_STAGE::VS);
}
