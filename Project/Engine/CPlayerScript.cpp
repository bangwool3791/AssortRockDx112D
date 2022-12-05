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

	/*
	* Occur Frame Drop
	*/
	if ((m_vTarget - vPos).Length() > 1.f)
	{
		auto vMove = m_vTarget - vPos;
		vPos += vMove * DT;
		Transform()->SetRelativePos(vPos);
	}
}

void CPlayerScript::BeginOverlap(CCollider2D* _pOther) 
{
	if (!lstrcmp(L"MouseDrag", _pOther->GetOwner()->GetName().c_str()))
	{
		//Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"UnitSelectUIPrefab");
		//Instantiate(pUIPrefab->Instantiate(), GetOwner());
	}
	else if(!lstrcmp(L"Player", _pOther->GetOwner()->GetName().c_str()))
	{
		Vec3 vSource = GetOwner()->Transform()->GetRelativePos();
		Vec3 vTarget = _pOther->GetOwner()->Transform()->GetRelativePos();
		Vec3 vDiff = vTarget - vSource;
		vDiff.Normalize();
		vSource += (vTarget - vSource) * -DT * 20.f;
		GetOwner()->Transform()->SetRelativePos(vSource);
		GetOwner()->Transform()->SetRelativeRotationX(vDiff.x);
		GetOwner()->Transform()->SetRelativeRotationY(vDiff.y);
		GetOwner()->Transform()->SetRelativeRotationZ(vDiff.z);
	}
}

void CPlayerScript::Overlap(CCollider2D* _pOther) 
{
	if (!lstrcmp(L"Player", _pOther->GetOwner()->GetName().c_str()))
	{
		Vec3 vSource = GetOwner()->Transform()->GetRelativePos();
		Vec3 vTarget = _pOther->GetOwner()->Transform()->GetRelativePos();
		Vec3 vDiff = vTarget - vSource;
		vDiff.Normalize();
		vSource += (vTarget - vSource) * -DT * 20.f;
		GetOwner()->Transform()->SetRelativePos(vSource);
		GetOwner()->Transform()->SetRelativeRotationX(vDiff.x);
		GetOwner()->Transform()->SetRelativeRotationY(vDiff.y);
		GetOwner()->Transform()->SetRelativeRotationZ(vDiff.z);
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

		//Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"UnitSelectUIPrefab");
		//CGameObject* pUnit_UI = pUIPrefab->Instantiate();
		//Instantiate(pUnit_UI, GetOwner());
		//CUIMgr::GetInst()->AddUI(GetOwner(), UI_TYPE::GAMEOBJECT);
		//CUIMgr::GetInst()->AddUI(pUnit_UI, UI_TYPE::UNIT_UI);
	}
}