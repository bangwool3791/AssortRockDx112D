#include "pch.h"

#include "CKeyMgr.h"
#include "CTimeMgr.h"

#include "CCameraScript.h"

#include "CCamera.h"
#include "CTransform.h"

CCameraScript::CCameraScript()
	:m_fSpeed{ 1.f }
{
}

CCameraScript::~CCameraScript()
{
}

void CCameraScript::begin()
{
	m_fSpeed = 100.f;
}

void CCameraScript::tick()
{
	Move();
}

void CCameraScript::Move()
{
	//Vec3& vPos			= Camera()->Transform()->GetRelativePos_();
	//Vec3& vFront		= Camera()->Transform()->GetRelativeDir_(DIR::FRONT);
	//Vec3& vRight		= Camera()->Transform()->GetRelativeDir_(DIR::RIGHT);
	//Vec3& vRotation		= Camera()->Transform()->GetRelativeRotation_();
	//Vec2& vMouse		= CKeyMgr::GetInst()->GetMouseDir_();
	//float& fScale		= Camera()->GetOrthographicScale_();

	//switch (Camera()->GetProjType())
	//{
	//case PROJ_TYPE::PERSPECTIVE:
	//	if (KEY_PRESSED(KEY::W))
	//		vPos += DT * vFront * m_fSpeed;
	//	else if (KEY_PRESSED(KEY::S))
	//		vPos -= DT * vFront * m_fSpeed;
	//	else if (KEY_PRESSED(KEY::A))
	//		vPos -= DT * vRight * m_fSpeed;
	//	else if (KEY_PRESSED(KEY::D))
	//		vPos += DT * vRight * m_fSpeed;

	//	if (KEY_PRESSED(KEY::RBTN))
	//	{
	//		vRotation.y += vMouse.x * DT * XM_PI;
	//		vRotation.x -= vMouse.y * DT * XM_PI;
	//	}

	//	break;
	//case PROJ_TYPE::ORTHOGRAHPICS:
	//	if (KEY_PRESSED(KEY::W))
	//		vPos.y += DT * m_fSpeed;
	//	else if (KEY_PRESSED(KEY::S))
	//		vPos.y -= DT * m_fSpeed;
	//	else if (KEY_PRESSED(KEY::A))
	//		vPos.x -= DT * m_fSpeed;
	//	else if (KEY_PRESSED(KEY::D))
	//		vPos.x += DT * m_fSpeed;
	//	else if (KEY_PRESSED(KEY::Q))
	//	{
	//		vRotation.z += DT * 1.f * XM_PI;
	//	}	
	//	else if (KEY_PRESSED(KEY::E))
	//	{
	//		vRotation.z -= DT * 1.f * XM_PI;
	//	}

	//	if (KEY_PRESSED(KEY::NUM_1))
	//		fScale += DT;

	//	if (KEY_PRESSED(KEY::NUM_2))
	//		fScale -= DT;
	//	break;
	//}
}
