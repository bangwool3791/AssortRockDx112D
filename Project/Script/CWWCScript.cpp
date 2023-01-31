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

	m_fFullHp = 125.f;

	m_iArmor = 10.f;

	Ptr<CPrefab> prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"CImageWallWoodPrefab");

	m_pPortrait = prefab->Instantiate();
	m_pPortrait->Transform()->SetRelativePos(-220.f, 0.f, -550.f);
	m_pPortrait->MeshRender()->Deactivate();
	Instantiate(m_pPortrait, 31);

	prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"CDescArmorPrefab");
	m_vecIcon.push_back(prefab->Instantiate());

	m_vecIcon[0]->Transform()->SetRelativePos(50.f, 0.f, -500.f);

	for (size_t i{}; i < m_vecIcon.size(); ++i)
		Instantiate(m_vecIcon[i], 31);

	for (size_t i{}; i < m_vecIcon.size(); ++i)
		m_vecIcon[i]->MeshRender()->Deactivate();
}

CWWCScript::~CWWCScript()
{
}

void CWWCScript::begin()
{
	GetOwner()->GetRenderComponent()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"BuildMtrl"));
	GetOwner()->GetRenderComponent()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\buildings\\Atlas1_LQ.dds"));
	GetOwner()->GetRenderComponent()->SetInstancingType(INSTANCING_TYPE::USED);
}

void CWWCScript::tick()
{
	__super::tick();
}

void CWWCScript::finaltick()
{

	m_fDt += DT;
	m_fDt2 += DT;

	if (0 > m_fHP)
	{
		//CEffectWoodPrefab
		CGameObject* pObj = CResMgr::GetInst()->FindRes<CPrefab>(L"CEffectExplosionPrefab")->Instantiate();
		Instantiate(pObj, Transform()->GetRelativePos(), 3);
		GetOwner()->Destroy();
	}

	if (m_eBuildState == BUILD_STATE::BUILD)
	{
		m_fHP += DT * 10.f;

		if (m_fHP > m_fFullHp)
		{
			GetOwner()->GetRenderComponent()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ObjectMtrl"));
			GetOwner()->GetRenderComponent()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\buildings\\Atlas1_LQ.dds"));
			GetOwner()->GetRenderComponent()->SetInstancingType(INSTANCING_TYPE::USED);
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

#include <Engine\CEngine.h>
#include <Engine\CFontMgr.h>
#include "CButtonScript.h"

void CWWCScript::PhaseEventOn()
{
	__super::PhaseEventOn();

	lstrcpy(CEngine::g_szFullName, L"Woodwall");

	wchar_t sz[200];

	lstrcpy(sz, to_wstring(m_fHP).c_str());
	lstrcat(sz, L"/");
	lstrcat(sz, to_wstring(m_fFullHp).c_str());

	lstrcpy(CEngine::g_szHp, sz);

	m_pPortrait->MeshRender()->Activate();

	CEngine::g_IconText.clear();

	SetIconUI(m_iArmor, 0);

	const vector<CGameObject*> vec = CInterfaceMgr::GetInst()->GetTapButtons();
	for (size_t i{}; i < 6; ++i)
		vec[i]->GetScript<CButtonScript>()->SetColumn((UINT)TAP_CATEGORY_UPGRADE);

	//GetOwner()->GetChilds()[0]->GetRenderComponent()->Activate();
}

void CWWCScript::PhaseEventOff()
{
	__super::PhaseEventOff();

	m_pPortrait->MeshRender()->Deactivate();

	for (size_t i{}; i < m_vecIcon.size(); ++i)
		m_vecIcon[i]->MeshRender()->Deactivate();

	//GetOwner()->GetChilds()[0]->GetRenderComponent()->Deactivate();
}















































































