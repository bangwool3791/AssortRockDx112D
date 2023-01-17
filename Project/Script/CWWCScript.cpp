#include "pch.h"
#include "CWWCScript.h"

#include <Engine\CDevice.h>
#include <Engine\CLevel.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CTransform.h>

#include <Engine\CInterfaceMgr.h>
#include <Script\CMouseScript.h>

CWWCScript::CWWCScript()
	:CScript{ SCRIPT_TYPE::WWCSCRIPT }
	, m_eBuildState{ BUILD_STATE::BUILD }
{
	SetName(L"CWWCScript");
}

CWWCScript::~CWWCScript()
{
}

void CWWCScript::begin()
{
	GetOwner()->GetRenderComponent()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"BuildMtrl"));
	GetOwner()->GetRenderComponent()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\buildings\\Atlas1_LQ.dds"));
	GetOwner()->GetRenderComponent()->SetInstancingType(INSTANCING_TYPE::NONE);
}

void CWWCScript::tick()
{

}

void CWWCScript::finaltick()
{

	m_fDt += DT;
	m_fDt2 += DT;

	if (m_eBuildState == BUILD_STATE::BUILD)
	{
		if (m_fDt > 5.f)
		{
			//GetOwner()->GetRenderComponent()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ObjectMtrl"));
			//GetOwner()->GetRenderComponent()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\buildings\\Atlas1_LQ.dds"));
			//GetOwner()->GetRenderComponent()->SetInstancingType(INSTANCING_TYPE::USED);
			m_eBuildState = BUILD_STATE::COMPLETE;
			m_fDt = 0.f;
		}
	}
	else if (m_eBuildState == BUILD_STATE::COMPLETE)
	{
	}
}

void CWWCScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CWWCScript::Overlap(CCollider2D* _pOther)
{
}

void CWWCScript::EndOverlap(CCollider2D* _pOther)
{
}
void CWWCScript::SaveToFile(FILE* _File)
{
	CScript::SaveToFile(_File);
}

void CWWCScript::LoadFromFile(FILE* _File)
{
	CScript::LoadFromFile(_File);
}















































































