#include "pch.h"
#include "CMonsterScript.h"

#include "CDevice.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"

#include "CTransform.h"
#include "CMaterial.h"
#include "CMeshRender.h"

#include "CPrefab.h"

CMonsterScript::CMonsterScript()
	: m_fSpeed{ 100.f }
{
}

CMonsterScript::~CMonsterScript()
{
}


void CMonsterScript::begin()
{
}

void CMonsterScript::tick()
{
	//Vec3 vPos = Transform()->GetRelativePos();
	////static Vec3 vTarget;

	//if ((m_vTarget - vPos).Length() > 0.5)
	//{
	//	auto vMove = m_vTarget - vPos;
	//	vPos += vMove * DT;
	//	Transform()->SetRelativePos(vPos);
	//}

	//static float fTimeDelta{};
	//fTimeDelta += DT;

	//if (fTimeDelta > 3.f)
	//{
	//	Vec3 vMissilePos = Transform()->GetRelativePos();
	//	vMissilePos.y += Transform()->GetRelativeScale().y / 2.f;

	//	Ptr<CPrefab> pMissilePrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"MissilePrefab");
	//	Instantiate(pMissilePrefab->Instantiate(), vMissilePos);

	//	DebugDrawCircle(Vec4(1.f, 0.2f, 0.2f, 1.f), vMissilePos, 50.f, 2.f);
	//	fTimeDelta = 0.f;
	//}

	// 플레이어 위치에 미사일 생성
	GetOwner()->SetDead();
	//GetOwner()->GetChildObject()[0]->SetDead();

}

void CMonsterScript::BeginOverlap(CCollider2D* _pOther)
{
	if (!lstrcmp(L"MouseDrag", _pOther->GetOwner()->GetName().c_str()))
	{
		Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"UnitSelectUIPrefab");
		Instantiate(pUIPrefab->Instantiate(), GetOwner());
	}
	else if (!lstrcmp(L"SelectUnit", _pOther->GetOwner()->GetName().c_str())
		|| !lstrcmp(L"Player", _pOther->GetOwner()->GetName().c_str()))
	{
		Vec3 vSource = GetOwner()->Transform()->GetRelativePos();
		Vec3 vTarget = _pOther->GetOwner()->Transform()->GetRelativePos();
		vSource += (vTarget - vSource) * -DT;
		GetOwner()->Transform()->SetRelativePos(vSource);
	}
}

void CMonsterScript::Overlap(CCollider2D* _pOther)
{
	if (!lstrcmp(L"SelectUnit", _pOther->GetOwner()->GetName().c_str())
		|| !lstrcmp(L"Player", _pOther->GetOwner()->GetName().c_str()))
	{
		Vec3 vSource = GetOwner()->Transform()->GetRelativePos();
		Vec3 vTarget = _pOther->GetOwner()->Transform()->GetRelativePos();
		vSource += (vTarget - vSource) * -DT;
		GetOwner()->Transform()->SetRelativePos(vSource);
	}
}

void CMonsterScript::EndOverlap(CCollider2D* _pOther)
{
	if (!lstrcmp(L"MouseDrag", _pOther->GetOwner()->GetName().c_str()))
	{
		GetOwner()->GetChild(L"UnitSelectUI")->SetDead();
	}
}