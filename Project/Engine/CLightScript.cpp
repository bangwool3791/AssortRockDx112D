#include "pch.h"
#include "CLightScript.h"

#include "CDevice.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CUIMgr.h"

#include "CTransform.h"
#include "CMaterial.h"
#include "CMeshRender.h"

#include "CPrefab.h"

CLightScript::CLightScript()
{
}

CLightScript::~CLightScript()
{
}


void CLightScript::begin()
{
}

void CLightScript::tick()
{
}

void CLightScript::BeginOverlap(CCollider2D* _pOther)
{
	if (!lstrcmp(L"MouseDrag", _pOther->GetOwner()->GetName().c_str()))
	{
		Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"UnitSelectUIPrefab");
		Instantiate(pUIPrefab->Instantiate(), GetOwner());
	}
	else if (!lstrcmp(L"Player", _pOther->GetOwner()->GetName().c_str()))
	{
	}
}

void CLightScript::Overlap(CCollider2D* _pOther)
{
	if (!lstrcmp(L"Player", _pOther->GetOwner()->GetName().c_str()))
	{
	}
}

void CLightScript::EndOverlap(CCollider2D* _pOther)
{
	if (!lstrcmp(L"MouseDrag", _pOther->GetOwner()->GetName().c_str()))
	{
	}
}