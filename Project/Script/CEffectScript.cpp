#include "pch.h"
#include "CEffectScript.h"

#include <Engine\CDevice.h>
#include <Engine\CLevel.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CTransform.h>
#include <Engine\CTerrain.h>

#include <Engine\CInterfaceMgr.h>
#include <Script\CMouseScript.h>

CEffectScript::CEffectScript()
	:CScript{ SCRIPT_TYPE::EFFECTSCRIPT }
{
	SetName(L"CEffectScript");
}

CEffectScript::~CEffectScript()
{
}


void CEffectScript::begin()
{

}

void CEffectScript::tick()
{
	if (GetOwner()->Animator2D()->IsEnd())
		GetOwner()->Destroy();
}

void CEffectScript::finaltick()
{
}

void CEffectScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CEffectScript::Overlap(CCollider2D* _pOther)
{
}

void CEffectScript::EndOverlap(CCollider2D* _pOther)
{
}
void CEffectScript::SaveToFile(FILE* _File)
{

}

void CEffectScript::LoadFromFile(FILE* _File)
{

}

