#include "pch.h"
#include "CArrowScript.h"

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
#include <Script\CInfectedGiantScript.h>
#include <Script\CInfectedMedium_A.h>
#include <Script\CInfectedMedium_B.h>
#include <Script\CInfectedStrong_A.h>
#include <Script\CInfectedVenom.h>

CArrowScript::CArrowScript()
	:CScript{ ARROWSCRIPT }
	, m_fSpeed{ 300.f }
	, m_pTargetObject{}
	, m_fAccTime{0.f}
{
	m_iAttack = 7;
	SetName(L"CArrowScript");

	//AddScriptParam(SCRIPT_PARAM::FLOAT, "Player MoveSpeed", &m_fSpeed);
}

CArrowScript::~CArrowScript()
{
}


void CArrowScript::begin()
{
	CAnimator2D* Animator = GetOwner()->Animator2D();

	if (nullptr == Animator)
		GetOwner()->AddComponent(new CAnimator2D());

	Animator = GetOwner()->Animator2D();

	Animator->ActiveRepeat();
	Animator->CloneAnimation(L"Arr", *Animator);
	Animator->CloneAnimation(L"BigA", *Animator);

	m_vSource = Transform()->GetRelativePos();

	m_vSource = Transform()->GetRelativePos();
	m_vSource.x += 1.f;
	m_vSource.z -= 1.f;
}

void CArrowScript::tick()
{
	if (m_pTargetObject && m_pTargetObject->IsDead())
	{
		Destroy();
		m_pTargetObject = nullptr;
	}

	if (m_pTargetObject)
	{
		m_vTarget = m_pTargetObject->Transform()->GetRelativePos();

		Vec3 vDir = m_vTarget - GetOwner()->Transform()->GetRelativePos();
		vDir = vDir.Normalize();

		Transform()->SetRelativeRotation(vDir.x, vDir.y, vDir.z);

		//Vec3 vAxis{ 1.f, 0.f, 0.f };

		//float fAngle = acosf(vAxis.Dot(vDir));

		//if (m_vTarget.z < GetOwner()->Transform()->GetRelativePos().z)
		//	fAngle = 2 * XM_PI - fAngle;

		//Transform()->SetRelativeRotationY(fAngle);

		m_vSource = Transform()->GetRelativePos();

		Vec3 vPos = vDir * DT * m_fSpeed + m_vSource;

		Transform()->SetRelativePos(vPos);
	}
}

void CArrowScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CArrowScript::Overlap(CCollider2D* _pOther)
{
	if (m_pTargetObject)
	{
		wstring wstrName = m_pTargetObject->GetName();

		float fHp{};

		fHp = m_pTargetObject->GetScripts()[0]->GetHp();
		fHp -= m_iAttack;
		m_pTargetObject->GetScripts()[0]->SetHp(fHp);

		Destroy();
	}
}

void CArrowScript::EndOverlap(CCollider2D* _pOther)
{
}

void CArrowScript::SaveToFile(FILE* _File)
{
	CScript::SaveToFile(_File);
	fwrite(&m_fSpeed, sizeof(float), 1, _File);
}

void CArrowScript::LoadFromFile(FILE* _File)
{
	CScript::LoadFromFile(_File);
	fread(&m_fSpeed, sizeof(float), 1, _File);
}

void CArrowScript::SetTarget(CGameObject* _pObj)
{
	m_pTargetObject = _pObj;
}