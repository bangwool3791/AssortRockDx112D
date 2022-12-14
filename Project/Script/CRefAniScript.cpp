#include "pch.h"
#include "CRefAniScript.h"

#include <Engine\CUIMgr.h>
#include <Engine\CScript.h>
#include <Engine\CTransform.h>
#include <Engine\CMaterial.h>
#include <Engine\CMeshRender.h>

CRefAniScript::CRefAniScript()
	:CScript{ REFANISCRIPT}
{
}

CRefAniScript::~CRefAniScript()
{
}


void CRefAniScript::begin()
{
}

void CRefAniScript::tick()
{
}

void CRefAniScript::BeginOverlap(CCollider2D* _pOther)
{
	if (CUIMgr::GetInst()->Get_Object(UI_TYPE::ANIMATION_BOX))
		return;
	static bool bfalg = false;

	if (!lstrcmp(L"MouseDrag", _pOther->GetOwner()->GetName().c_str()))// && !bfalg)
	{
		/*
		* Prefab으로 객체 생성 필요
		*/
		CGameObject* pGameObject = _pOther->GetOwner();
		Vec3 vOhterPos = _pOther->GetOwner()->Transform()->GetRelativePos();
		Vec3 vOhterScale = _pOther->GetOwner()->Transform()->GetRelativeScale();

		Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
		Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();

		if (vOhterPos.x + vOhterScale.x * 0.5f <= vPos.x + vScale.x * 0.5f
			&& vOhterPos.x - vOhterScale.x * 0.5f >= vPos.x - vScale.x * 0.5f
			&& vOhterPos.y + vOhterScale.y * 0.5f <= vPos.y + vScale.y * 0.5f
			&& vOhterPos.y - vOhterScale.y * 0.5f >= vPos.y - vScale.y * 0.5f)
		{
			CGameObject* pTemp{};
			Instantiate(pTemp = pGameObject->Clone(), 30);
			pTemp->SetName(L"Animation_Box");
			bfalg = true;
			CUIMgr::GetInst()->AddUI(pTemp, UI_TYPE::ANIMATION_BOX);
		}
	}
}

void CRefAniScript::Overlap(CCollider2D* _pOther)
{
}

void CRefAniScript::EndOverlap(CCollider2D* _pOther)
{
}