#include "pch.h"
#include "CTransform.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CDevice.h"
#include "CConstBuffer.h"

#include "extern.cpp"

CTransform::CTransform()
	:CComponent{COMPONENT_TYPE::TRANSFORM}
	, m_vRelativeScale{1.f,1.f,1.f}
	, m_blgnParentScale(false)
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
}

void CTransform::finaltick()
{
	m_matWorld = XMMatrixIdentity();
	Matrix		matScail = XMMatrixScaling(m_vRelativeScale.x, m_vRelativeScale.y, m_vRelativeScale.z);

	Matrix		matPos = XMMatrixTranslation(m_vRelativePos.x, m_vRelativePos.y, m_vRelativePos.z);
	//Degree 단위로 저장 된 회전 x,y,z 정보를 행렬에 담는다.
	Matrix		matRot = XMMatrixRotationX(m_vRelativeRotation.x);
	matRot *= XMMatrixRotationY(m_vRelativeRotation.y);
	matRot *= XMMatrixRotationZ(m_vRelativeRotation.z);

	//행렬에 담긴 x,y,z축에 대한 회전 정보(Vector3)를 담는다.
	m_vRelativeDir[(UINT)DIR::RIGHT] = m_vWorldDir[(UINT)DIR::RIGHT] = XMVector3TransformNormal(Vec3{1.f, 0.f, 0.f}, matRot);
	m_vRelativeDir[(UINT)DIR::UP] = m_vWorldDir[(UINT)DIR::UP] = XMVector3TransformNormal(Vec3{ 0.f, 1.f, 0.f }, matRot);
	m_vRelativeDir[(UINT)DIR::FRONT] = m_vWorldDir[(UINT)DIR::FRONT] = XMVector3TransformNormal(Vec3{ 0.f, 0.f, 1.f }, matRot);

	m_matWorld = matScail * matRot * matPos ;

	if (GetOwner()->Get_Parent())
	{
		if (m_blgnParentScale)
		{
			CGameObject* pParent = GetOwner()->Get_Parent();
			Vec3         vParentScale = Vec3(1.f, 1.f, 1.f);
			while (pParent)
			{
				vParentScale *= pParent->Transform()->GetRelativeScale();

				if (pParent->Transform()->m_blgnParentScale)
					pParent = nullptr;
				else
					pParent = pParent->Get_Parent();
			}

			if (0.f == vParentScale.x)
			{
				vParentScale.x = 1.f;
			}
			else if (0.f == vParentScale.y)
			{
				vParentScale.y = 1.f;
			}
			else if (0.f == vParentScale.z)
			{
				vParentScale.z = 1.f;
			}

			Matrix matParentScaleInv = XMMatrixScaling(vParentScale.x, vParentScale.y, vParentScale.z);
			matParentScaleInv		 = XMMatrixInverse(nullptr, matParentScaleInv);

			m_matWorld = m_matWorld * matParentScaleInv * GetOwner()->Get_Parent()->Transform()->GetWorldMat();
		}
		else
		{
			m_matWorld = m_matWorld * GetOwner()->Get_Parent()->Transform()->GetWorldMat();
		}

		if (!lstrcmp(L"SpotLight", GetOwner()->GetName().c_str()))
		{
			int a{};

		}
		//부모가 있을 경우, 스케일이 포함 된 월드의 right, up, look이 방향 벡터이므로, 노멀라이즈한다.
		m_vWorldDir[(UINT)DIR::RIGHT] = XMVector3TransformNormal(Vec3(1.f, 0.f, 0.f), m_matWorld);
		m_vWorldDir[(UINT)DIR::UP] = XMVector3TransformNormal(Vec3(0.f, 1.f, 0.f), m_matWorld);
		m_vWorldDir[(UINT)DIR::FRONT] = XMVector3TransformNormal(Vec3(0.f, 0.f, 1.f), m_matWorld);

		// 회전, 크기 변환이 이루어졌기 때문에 변경된 크기를 초기화 하기 위해 Normalize 해준다.
		m_vWorldDir[(UINT)DIR::RIGHT].Normalize();
		m_vWorldDir[(UINT)DIR::UP].Normalize();
		m_vWorldDir[(UINT)DIR::FRONT].Normalize();
	}
}

void CTransform::UpdateData()
{
	CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
	
	g_transform.matWorld = m_matWorld;
	g_transform.matWV	 = g_transform.matWorld * g_transform.matView;
	g_transform.matWVP	 = g_transform.matWV * g_transform.matProj;

	pCB->SetData(&g_transform);
	pCB->UpdateData(ALL_STAGE);
}

Vec3 CTransform::GetWorldScale()
{
	Vec3 vWorldScale = m_vRelativeScale;

	if (m_blgnParentScale)
		return m_vRelativeScale;

	CGameObject* pParentObject = GetOwner()->Get_Parent();

	while (pParentObject)
	{
		vWorldScale *= pParentObject->Transform()->GetRelativeScale();

		if (pParentObject->Transform()->m_blgnParentScale)
			pParentObject = nullptr;
		else
			pParentObject = pParentObject->Get_Parent();

	}
	return vWorldScale;
}


void CTransform::SaveToFile(FILE* _File)
{
	COMPONENT_TYPE eType = GetType();

	fwrite(&eType, sizeof(COMPONENT_TYPE), 1, _File);

	fwrite(&m_vRelativePos , sizeof(Vec3), 1, _File);
	fwrite(&m_vRelativeScale , sizeof(Vec3), 1, _File);
	fwrite(&m_vRelativeRotation , sizeof(Vec3), 1, _File);
	fwrite(&m_blgnParentScale , sizeof(bool), 1, _File);
}

void CTransform::LoadFromFile(FILE* _File)
{
	fread(&m_vRelativePos, sizeof(Vec3), 1, _File);
	fread(&m_vRelativeScale, sizeof(Vec3), 1, _File);
	fread(&m_vRelativeRotation, sizeof(Vec3), 1, _File);
	fread(&m_blgnParentScale, sizeof(bool), 1, _File);
}
