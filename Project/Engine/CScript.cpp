#include "pch.h"
#include "CScript.h"

#include "CEngine.h"
#include "CDevice.h"
#include "CInterfaceMgr.h"

CScript::CScript(int _iScriptType)
	:CComponent(COMPONENT_TYPE::SCRIPT)
	, m_iScriptType{_iScriptType}
	, m_bActive{false}
{
}

CScript::~CScript()
{

}

void CScript::begin()
{
	m_pPortrait->GetRenderComponent()->Deactivate();
}

void CScript::tick()
{
	if (m_bDesc)
	{
		//lstrcpy(g_szHp, to_wstring(m_fHP));
		wchar_t sz[200];

		lstrcpy(sz, std::to_wstring(m_fHP).c_str());
		lstrcat(sz, L"/");
		lstrcat(sz, std::to_wstring(m_fFullHp).c_str());

		lstrcpy(CEngine::g_szHp, sz);
	}
}

void CScript::SetIconUI(int iValue, UINT idx)
{

	static wchar_t sz1[50];

	memset(sz1, 0, sizeof(wchar_t) * 50);

	if (0 <= iValue)
		lstrcpy(sz1, L"+");
	else
		lstrcpy(sz1, L"-");

	lstrcat(sz1, std::to_wstring(iValue).c_str());

	m_vecIcon[idx]->MeshRender()->Activate();
	Vec3 vPos = m_vecIcon[idx]->Transform()->GetRelativePos();
	pair<wstring, Vec2> p{};
	p.first = sz1;
	Vec2 vRenderResolution = CDevice::GetInst()->GetRenderResolution();

	if(4 > idx)
		p.second = Vec2(vRenderResolution.x * 0.5f + vPos.x - 15, vRenderResolution.y * 0.5f - vPos.z * 0.5f + 120);
	else
		p.second = Vec2(vRenderResolution.x * 0.5f + vPos.x - 15, vRenderResolution.y * 0.5f - vPos.z * 0.5f + 135);
	CEngine::g_IconText.push_back(p);
}

void CScript::ActiveIcon()
{
	GetOwner()->GetChilds()[0]->GetRenderComponent()->Activate();
}

void CScript::DeactiveIcon()
{
	GetOwner()->GetChilds()[0]->GetRenderComponent()->Deactivate();
}

void CScript::PhaseEventOn()
{
	m_bDesc = true;
}

void CScript::PhaseEventOff()
{
	m_bDesc = false;
}


bool CScript::Create(const wstring& _str)
{
	Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(_str);
	CGameObject* pObj = pUIPrefab->Instantiate();

	int _iGold = pObj->GetScripts()[0]->GetGoldOut();
	int _iWood = pObj->GetScripts()[0]->GetWoodOut();
	int _iIron = pObj->GetScripts()[0]->GetIronOut();

	int _iGoldInc = pObj->GetScripts()[0]->GetGold();

	int _iWorker = pObj->GetScripts()[0]->GetWorker();
	int _iFood = pObj->GetScripts()[0]->GetFood();
	int _iColony = pObj->GetScripts()[0]->GetColony();

	if (g_iGold >= _iGold && g_iWood >= _iWood && g_iIron >= _iIron)
	{
		if (0 > g_iGoldInc && 0 >= g_iGoldInc - _iGoldInc)
		{
			delete pObj;
			return false;
		}
		else if (0 > g_iFood && 0 >= g_iFood - _iFood)
		{
			delete pObj;
			return false;
		}
		else if (0 > g_iColony && 0 >= g_iColony - _iColony)
		{
			delete pObj;
			return false;
		}
		else
		{
			CInterfaceMgr::GetInst()->SetBuildObj(pObj);
			Instantiate(pObj, 5);
			return true;
		}
	}
	else
	{
		delete pObj;
		return false;
	}
}

bool CScript::Create(const wstring& _str, Vec3 vPos)
{
	CInterfaceMgr::GetInst()->SetBuildObj(nullptr);

	Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(_str);
	CGameObject* pObj = pUIPrefab->Instantiate();

	int _iGold = pObj->GetScripts()[0]->GetGoldOut();
	int _iWood = pObj->GetScripts()[0]->GetWoodOut();
	int _iIron = pObj->GetScripts()[0]->GetIronOut();

	int _iGoldInc = pObj->GetScripts()[0]->GetGold();

	int _iWorker = pObj->GetScripts()[0]->GetWorker();
	int _iFood = pObj->GetScripts()[0]->GetFood();
	int _iColony = pObj->GetScripts()[0]->GetColony();

	if (g_iGold >= _iGold && g_iWood >= _iWood && g_iIron >= _iIron)
	{
		if (0 > g_iGoldInc && 0 >= g_iGoldInc - _iGoldInc)
		{
			delete pObj;
			return false;
		}
		else if (0 > g_iFood && 0 >= g_iFood - _iFood)
		{
			delete pObj;
			return false;
		}
		else if (0 > g_iColony && 0 >= g_iColony - _iColony)
		{
			delete pObj;
			return false;
		}
		else
		{
			g_iGoldInc -= _iGoldInc;
			g_iFood += _iFood;
			g_iColony += _iColony;
			g_iGold -= _iGold;
			g_iWood -= _iWood;
			g_iIron -= _iIron;
			CInterfaceMgr::GetInst()->SetBuildObj(pObj);
			Instantiate(pObj, vPos, 5);
			return true;
		}
	}
	else
	{
		delete pObj;
		return false;
	}
}