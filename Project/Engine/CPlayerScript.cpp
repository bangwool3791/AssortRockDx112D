#include "pch.h"
#include "CPlayerScript.h"

#include "CDevice.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CUIMgr.h"

#include "CTransform.h"
#include "CMaterial.h"
#include "CMeshRender.h"

#include "CPrefab.h"

CPlayerScript::CPlayerScript()
	: m_fSpeed{100.f}
{
}

CPlayerScript::~CPlayerScript()
{
}


void CPlayerScript::begin()
{
	m_vTarget = GetOwner()->Transform()->GetRelativePos();
}

void CPlayerScript::tick()
{
	Vec3 vPos = Transform()->GetRelativePos();
	vPos.z = 1.f;
	//static Vec3 vTarget;

	float a = (m_vTarget - vPos).Length();

	if ((m_vTarget - vPos).Length() > 1.f)
	{
		auto vMove = m_vTarget - vPos;
		vPos += vMove * DT;
		Transform()->SetRelativePos(vPos);
	}

	//if(KEY_PRESSED(KEY::A))
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
}

void CPlayerScript::BeginOverlap(CCollider2D* _pOther) 
{
	if (!lstrcmp(L"MouseDrag", _pOther->GetOwner()->GetName().c_str()))
	{
		Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"UnitSelectUIPrefab");
		Instantiate(pUIPrefab->Instantiate(), GetOwner());
	}
	else if(!lstrcmp(L"Player", _pOther->GetOwner()->GetName().c_str()))
	{
		Vec3 vSource = GetOwner()->Transform()->GetRelativePos();
		Vec3 vTarget = _pOther->GetOwner()->Transform()->GetRelativePos();
		vSource += (vTarget - vSource) * -DT * 20.f;
		GetOwner()->Transform()->SetRelativePos(vSource);
	}
}

void CPlayerScript::Overlap(CCollider2D* _pOther) 
{
	if (!lstrcmp(L"Player", _pOther->GetOwner()->GetName().c_str()))
	{
		Vec3 vSource = GetOwner()->Transform()->GetRelativePos();
		Vec3 vTarget = _pOther->GetOwner()->Transform()->GetRelativePos();
		vSource += (vTarget - vSource) * -DT * 20.f;
		GetOwner()->Transform()->SetRelativePos(vSource);
	}
}

void CPlayerScript::EndOverlap(CCollider2D* _pOther) 
{
	if (!lstrcmp(L"MouseDrag", _pOther->GetOwner()->GetName().c_str()))
	{
		const vector<CGameObject*>& childs = GetOwner()->GetChilds();
		for (auto iter{ childs.begin() }; iter != childs.end(); ++iter)
		{
			if(!lstrcmp(L"UnitSelectUI", (*iter)->GetName().c_str()))
			{
				(*iter)->Destroy();
			}
		}

		Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"UnitSelectUIPrefab");
		CGameObject* pUnit_UI = pUIPrefab->Instantiate();
		Instantiate(pUnit_UI, GetOwner(), 0);
		CUIMgr::GetInst()->AddUI(GetOwner(), UI_TYPE::GAMEOBJECT);
		CUIMgr::GetInst()->AddUI(pUnit_UI, UI_TYPE::UNIT_UI);

	}
}