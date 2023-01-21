#include "pch.h"
#include "CSoldierScript.h"

#include <Engine\CDevice.h>
#include <Engine\CLayer.h>
#include <Engine\CLevel.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CCollisionMgr.h>
#include <Engine\CScript.h>
#include <Engine\CCollider2D.h>
#include <Engine\CTransform.h>
#include <Engine\CMaterial.h>
#include <Engine\CMeshRender.h>
#include <Engine\CJpsMgr.h>

#include <Engine\CInterfaceMgr.h>
#include <Engine\CScript.h>

CSoldierScript::CSoldierScript()
	:CScript{ SOLDIERSCRIPT }
	, m_fSpeed{ 100.f }
	, m_eState{UNIT_STATE::NORMAL}
{
	SetName(L"CSoldierScript");

	AddScriptParam(SCRIPT_PARAM::FLOAT, "Player MoveSpeed", &m_fSpeed);
}

CSoldierScript::~CSoldierScript()
{
}


void CSoldierScript::begin()
{
	CAnimator2D* Animator = GetOwner()->Animator2D();

	if (nullptr == Animator)
		GetOwner()->AddComponent(new CAnimator2D());

	Animator = GetOwner()->Animator2D();

	Animator->CloneAnimation(L"Soldier", *Animator);
}

#define OFFSET 5.625f

void CSoldierScript::tick()
{

	if (KEY_PRESSED(KEY::LBTN))
	{
		Vec2 p = CKeyMgr::GetInst()->GetMousePos();
		Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();

		p.x = (2.0f * p.x) / vResolution.x - 1.0f;
		p.y = 1.0f - (2.0f * p.y) / vResolution.y;

		XMVECTOR det; //Determinant, needed for matrix inverse function call
		Vector3 origin = Vector3(p.x, p.y, 0);
		Vector3 faraway = Vector3(p.x, p.y, 1);

		XMMATRIX invViewProj = XMMatrixInverse(&det, g_transform.matView * g_transform.matProj);
		Vector3 rayorigin = XMVector3Transform(origin, invViewProj);
		Vector3 rayend = XMVector3Transform(faraway, invViewProj);
		Vector3 raydirection = rayend - rayorigin;
		raydirection.Normalize();
		Ray ray;
		ray.position = rayorigin;
		ray.direction = raydirection;

		if (GetOwner()->Transform()->Picking(ray, m_vDest))
		{
			CInterfaceMgr::GetInst()->SetTarget(GetOwner());
		}
	}

	if (UNIT_STATE::NORMAL == m_eState)
	{

	}
	else if (UNIT_STATE::RUN == m_eState)
	{

	}
	else if (UNIT_STATE::ATTACK == m_eState)
	{

	}
	else if (UNIT_STATE::LOADATTACK == m_eState)
	{

	}
	else if (UNIT_STATE::DEAD == m_eState)
	{

	}


	//Set Animation according to radian

}

void CSoldierScript::SetDestPos(Vec3 _vPos)
{
	m_vTarget = _vPos;
	/*
	* 방향 벡터 
	* x축 기저 벡터
	* 내적
	*/

	Vec3 vDir = m_vTarget - GetOwner()->Transform()->GetRelativePos();
	vDir = vDir.Normalize();

	Vec3 vAxis{ 1.f, 0.f, 0.f };

	if (0 <= vAxis.Dot(vDir))
	{
		GetOwner()->Transform()->SetRelativeRotationX(XM_PI * - 0.25f);
		GetOwner()->Transform()->SetRelativeRotationY(0.f);
		GetOwner()->Transform()->SetRelativeRotationZ(0.f);
	}
	else if (0 > vAxis.Dot(vDir))
	{
		GetOwner()->Transform()->SetRelativeRotationX(XM_PI * 0.25f);
		GetOwner()->Transform()->SetRelativeRotationY(0.f);
		GetOwner()->Transform()->SetRelativeRotationZ(XM_PI);
	}

	float fAngle = acosf(vAxis.Dot(vDir));

	if (m_vTarget.z < GetOwner()->Transform()->GetRelativePos().z)
		fAngle = 2 * XM_PI - fAngle;

	if (XM_PI * 0.5f < fAngle && XM_PI > fAngle)
		fAngle = XM_PI - fAngle;

	if (XM_PI < fAngle && XM_PI * 1.5f > fAngle)
	{
		fAngle = fAngle - XM_PI;
		fAngle = 2.f * XM_PI - fAngle;
	}
	
	fAngle = fAngle / XM_PI * 180;

	wstring str{};

	if (UNIT_STATE::NORMAL == m_eState)
		str = L"Soldier_A_Normal";
	else if (UNIT_STATE::LOADATTACK == m_eState)
		str = L"Soldier_A_LoadAttack";
	else if (UNIT_STATE::ATTACK == m_eState)
		str = L"Soldier_A_Attack";
	else if (UNIT_STATE::RUN == m_eState)
		str = L"Soldier_A_Run";

	if (fAngle <= 90 + OFFSET && 90 - OFFSET < fAngle)
	{
		str += L"_234";
	}
	else if (fAngle <= 67.5 + OFFSET && 67.5 - OFFSET < fAngle)
	{
		str += L"_252";
	}
	else if (fAngle <= 45.f + OFFSET && 45.f - OFFSET < fAngle)
	{
		str += L"_270";
	}
	else if (fAngle <= 22.5f + OFFSET && 22.5f - OFFSET < fAngle)
	{
		str += L"_288";
	}
	else if (fAngle <= 0.f + OFFSET && 0.f - OFFSET < fAngle)
	{
		str += L"_306";
	}
	else if (fAngle <= 360.f - 22.5f + OFFSET && 360.f - 22.5f - OFFSET < fAngle)
	{
		str += L"_000";
	}
	else if (fAngle <= 360.f - 45.f + OFFSET && 360.f - 45.f - OFFSET < fAngle)
	{
		str += L"_018";
	}
	else if (fAngle <= 360.f - 67.5f + OFFSET && 360.f - 67.5f - OFFSET < fAngle)
	{
		str += L"_036";
	}

	GetOwner()->Animator2D()->Play(str);
}

void CSoldierScript::JpsAlgorithm(Int32 x, Int32 z)
{
	m_vecJps = CJpsMgr::GetInst()->Update(x, z);
}

void CSoldierScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CSoldierScript::Overlap(CCollider2D* _pOther)
{
}

void CSoldierScript::EndOverlap(CCollider2D* _pOther)
{
}

void CSoldierScript::SaveToFile(FILE* _File)
{
	CScript::SaveToFile(_File);
	fwrite(&m_fSpeed, sizeof(float), 1, _File);

	SaveResourceRef(m_Prefab, _File);
}

void CSoldierScript::LoadFromFile(FILE* _File)
{
	CScript::LoadFromFile(_File);
	fread(&m_fSpeed, sizeof(float), 1, _File);

	LoadResourceRef(m_Prefab, _File);
}
