#include "pch.h"
#include "CCamera.h"

#include "CDevice.h"
#include "CTransform.h"

CCamera::CCamera()
	:CComponent(COMPONENT_TYPE::CAMERA)
	, m_eProjType{ PROJ_TYPE::ORTHOGRAHPICS }
	, m_matView{}
	, m_matProj{}
	, m_fAspectRatio{1.f}
	, m_fFar{1000.f}
	, m_fScale{1.f}
{
	Vec2 vRenderResolution	= CDevice::GetInst()->GetRenderResolution();
	m_fAspectRatio			= vRenderResolution.x / vRenderResolution.y;
}

CCamera::~CCamera()
{
}

void CCamera::finaltick()
{
	Vec3	vPos			= Transform()->GetRelativePos();
	Matrix	matViewTrnas	= XMMatrixTranslation(-vPos.x, -vPos.y, -vPos.z);
	Vec3	vRight			= Transform()->GetRelativeDir(DIR::RIGHT);
	Vec3	vUp				= Transform()->GetRelativeDir(DIR::UP);
	Vec3	vLook			= Transform()->GetRelativeDir(DIR::FRONT);

	Matrix matViewRot		= XMMatrixIdentity();
	/*
	* ���� ����, ���� ������ ��� ��ġ ����� �����, ���� ��İ� ��ġ��� �� ������ ���� ������ ����, ���� ����� �ȴ�.
	*/
	matViewRot._11 = vRight.x; 
	matViewRot._21 = vRight.y;
	matViewRot._31 = vRight.z;

	matViewRot._12 = vUp.x;
	matViewRot._22 = vUp.y;
	matViewRot._32 = vUp.z;

	matViewRot._13 = vLook.x;
	matViewRot._23 = vLook.y;
	matViewRot._33 = vLook.z;

	m_matView = matViewTrnas * matViewRot;
	//m_matView =  XMMatrixIdentity();
	Vec2 vRenderResolution = CDevice::GetInst()->GetRenderResolution();

	switch (m_eProjType)
	{
	case PERSPECTIVE:
		m_matProj = XMMatrixPerspectiveFovLH(XM_2PI / 6.f, m_fAspectRatio, 1.f, m_fFar);
		break;
	case ORTHOGRAHPICS:
		m_matProj = XMMatrixOrthographicLH(vRenderResolution.x * m_fScale, vRenderResolution.y * m_fScale, 1.f, m_fFar);
		break;
	}

	g_transform.matView = m_matView;
	g_transform.matProj = m_matProj;
}
