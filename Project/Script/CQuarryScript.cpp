#include "pch.h"
#include "CQuarryScript.h"

#include <Engine\CDevice.h>
#include <Engine\CLevel.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CTransform.h>

#include <Engine\CInterfaceMgr.h>
#include <Script\CMouseScript.h>

CQuarryScript::CQuarryScript()
	:CScript{ SCRIPT_TYPE::QUARRYSCRIPT }
	, m_vMousePos{}
	, m_pTileObject{}
	, m_eBuildState{ BUILD_STATE::READY }
{

	m_fFullHp = 125;

	SetName(L"CQuarryScript");

	Ptr<CPrefab> prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"CImageQuarryPrefab");

	m_pPortrait = prefab->Instantiate();
	m_pPortrait->Transform()->SetRelativePos(-220.f, 0.f, -550.f);
	m_pPortrait->MeshRender()->Deactivate();
	Instantiate(m_pPortrait, 31);

	prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"CDescGoldPrefab");
	m_vecIcon.push_back(prefab->Instantiate());

	m_vecIcon[0]->Transform()->SetRelativePos(50.f, 0.f, -500.f);

	for (size_t i{}; i < m_vecIcon.size(); ++i)
		Instantiate(m_vecIcon[i], 31);

	for (size_t i{}; i < m_vecIcon.size(); ++i)
		m_vecIcon[i]->MeshRender()->Deactivate();

}

CQuarryScript::~CQuarryScript()
{
}

void CQuarryScript::begin()
{
	m_pTileObject = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"LevelTile");

	GetOwner()->GetRenderComponent()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"BuildMtrl"));
	GetOwner()->GetRenderComponent()->SetInstancingType(INSTANCING_TYPE::USED);

	GetOwner()->GetChilds()[0]->GetRenderComponent()->Deactivate();
}

void CQuarryScript::tick()
{
	__super::tick();

	m_fDt += DT;
	m_fDt2 += DT;

	if (0 > m_fHP)
	{
		//CEffectWoodPrefab
		CGameObject* pObj = CResMgr::GetInst()->FindRes<CPrefab>(L"CEffectWoodPrefab")->Instantiate();
		Instantiate(pObj, Transform()->GetRelativePos(), 3);
		GetOwner()->Destroy();
	}

	if (BUILD_STATE::READY == m_eBuildState)
	{
		if (m_fDt > 0.15f)
		{
			const Ray& ray = GetRay();

			m_vMousePos = m_pTileObject->GetRenderComponent()->GetMesh()->GetPosition(ray);

			tTile tTile = m_pTileObject->TileMap()->GetInfo(m_vMousePos);

			if (m_iIndex != tTile.iIndex)
			{
				clear();
			}

			if ((tTile.iIndex / TILEX) % 2 == 0)
			{
				m_result.push_back(tTile.iIndex);
				m_result.push_back(tTile.iIndex + TILEX);
				m_result.push_back(tTile.iIndex + TILEX - 1);
				m_result.push_back(tTile.iIndex + TILEX * 2);
			}
			else if ((tTile.iIndex / TILEX) % 2 == 1)
			{
				m_result.push_back(tTile.iIndex);
				m_result.push_back(tTile.iIndex + TILEX);
				m_result.push_back(tTile.iIndex + TILEX + 1);
				m_result.push_back(tTile.iIndex + TILEX * 2);
			}

			SetTileInfo(m_queue, m_result, (UINT)TILE_TYPE::BUILD);
			SetTileInfo(m_queue, m_result, (UINT)TILE_TYPE::BEFORE_CRYSTAL);
			SetTileInfo(m_queue, m_result, (UINT)TILE_TYPE::BEFORE_CRYSTAL);

			m_result.clear();

			for (size_t i{}; i < 40000; ++i)
				m_bCheck[i] = false;

			m_iIndex = tTile.iIndex;

			tTile.vPos.z += GetOwner()->Transform()->GetRelativeScale().z * 0.5f;

			GetOwner()->Transform()->SetRelativePos(tTile.vPos);

			int a = 0;

			if (IsBlocked(m_iIndex))
			{
				a = 1;
				GetOwner()->GetRenderComponent()->GetDynamicMaterial()->SetScalarParam(INT_0, &a);
			}
			else
				GetOwner()->GetRenderComponent()->GetDynamicMaterial()->SetScalarParam(INT_0, &a);

			m_fDt -= 0.15f;
		}

		if (m_fDt2 > 0.5f)
		{
			if (KEY_PRESSED(KEY::LBTN) && !IsBlocked(m_iIndex))
			{
				if ((m_iIndex / TILEX) % 2 == 0)
				{
					m_result.push_back(m_iIndex);
					m_result.push_back(m_iIndex + TILEX);
					m_result.push_back(m_iIndex + TILEX - 1);
					m_result.push_back(m_iIndex + TILEX * 2);
				}
				else if ((m_iIndex / TILEX) % 2 == 1)
				{
					m_result.push_back(m_iIndex);
					m_result.push_back(m_iIndex + TILEX);
					m_result.push_back(m_iIndex + TILEX + 1);
					m_result.push_back(m_iIndex + TILEX * 2);
				}

				SetTileInfo(m_queue, m_result, (UINT)TILE_TYPE::USED);
				SetTileInfo(m_queue, m_result, (UINT)TILE_TYPE::HARVEST);
				SetTileInfo(m_queue, m_result, (UINT)TILE_TYPE::HARVEST);

				m_result.clear();

				for (size_t i{}; i < 40000; ++i)
					m_bCheck[i] = false;

				m_eBuildState = BUILD_STATE::BUILD;
				m_fDt = 0.f;
				m_fDt2 = 0.f;

				Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"QuarryPrefab");
				CGameObject* pObj = pUIPrefab->Instantiate();
				CInterfaceMgr::GetInst()->SetBuildObj(pObj);
				Instantiate(pObj, m_vMousePos, 1);
			}
		}
	}
	else if (m_eBuildState == BUILD_STATE::BUILD)
	{
		m_fHP += DT * 10.f;

		if (m_fHP > m_fFullHp)
		{
			m_fHP = m_fFullHp;
			GetOwner()->GetRenderComponent()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ObjectMtrl"));
			GetOwner()->GetRenderComponent()->SetInstancingType(INSTANCING_TYPE::USED);
			m_eBuildState = BUILD_STATE::COMPLETE;
			m_fDt = 0.f;
		}
	}
	else if (m_eBuildState == BUILD_STATE::COMPLETE)
	{
	}
}

void CQuarryScript::finaltick()
{
}

void CQuarryScript::SetTileInfo(UINT _iTile, UINT _iValue)
{
	m_pTileObject->TileMap()->SetInfo(_iTile, _iValue);
}

void  CQuarryScript::SetTile(UINT _iTile, UINT _iValue)
{
	SetTileInfo(_iTile, _iValue);
}

void CQuarryScript::SetTileInfo(vector<UINT>& que, vector<UINT>& result, UINT _value)
{
	que = result;

	result.clear();

	for(auto iter{que.begin()}; iter != que.end();)
	{
		UINT data = (*iter);
		iter = que.erase(iter);

		tTile tTile = m_pTileObject->TileMap()->GetInfo(data);
		
		m_vecMask.push_back(tTile);

		if ((UINT)TILE_TYPE::CRYSTAL == tTile.iInfo && (UINT)TILE_TYPE::BEFORE_CRYSTAL == _value)
			m_pTileObject->TileMap()->SetInfo(data, _value);
		else if ((UINT)TILE_TYPE::BEFORE_CRYSTAL == tTile.iInfo && (UINT)TILE_TYPE::CRYSTAL == _value)
			m_pTileObject->TileMap()->SetInfo(data, _value);
		else if ((UINT)TILE_TYPE::BEFORE_CRYSTAL == tTile.iInfo && (UINT)TILE_TYPE::HARVEST == _value)
		{
			++m_iIron;
			m_pTileObject->TileMap()->SetInfo(data, _value);
		}
		else if ((UINT)TILE_TYPE::EMPTY == tTile.iInfo && (UINT)TILE_TYPE::BUILD == _value)
			m_pTileObject->TileMap()->SetInfo(data, _value);
		else if ((UINT)TILE_TYPE::BUILD == tTile.iInfo && (UINT)TILE_TYPE::EMPTY == _value)
			m_pTileObject->TileMap()->SetInfo(data, _value);
		else if ((UINT)TILE_TYPE::BUILD == tTile.iInfo && (UINT)TILE_TYPE::USED == _value)
		{
			Int32 x = data % TILEX;
			Int32 z = data / TILEZ;
			m_vecBlock.push_back(tBlock{ x,z });
			m_pTileObject->TileMap()->SetInfo(data, _value);
		}

		m_bCheck[data] = true;

		if ((data / TILEX) % 2 == 0)
		{
			if (0 <= data - 1 && data - 1 < 40000)
				if (!m_bCheck[data - 1])
				{
					result.push_back(data - 1);
					m_bCheck[data - 1] = true;
				}

			if (0 <= data + 1 && data + 1 < 40000)
				if (!m_bCheck[data + 1])
				{
					result.push_back(data + 1);
					m_bCheck[data + 1] = true;
				}

			if (0 <= data + TILEX && data + TILEX < 40000)
				if (!m_bCheck[data + TILEX])
				{
					result.push_back(data + TILEX);
					m_bCheck[data + TILEX] = true;
				}

			if (0 <= data + TILEX - 1 && data + TILEX - 1 < 40000)
				if (!m_bCheck[data + TILEX - 1])
				{
					result.push_back(data + TILEX - 1);
					m_bCheck[data + TILEX - 1] = true;
				}

			if (0 <= data + TILEX * 2 && data + TILEX * 2 < 40000)
				if (!m_bCheck[data + TILEX * 2])
				{
					result.push_back(data + TILEX * 2);
					m_bCheck[data + TILEX * 2] = true;
				}

			if (0 <= data - TILEX && data - TILEX < 40000)
				if (!m_bCheck[data - TILEX])
				{
					result.push_back(data - TILEX);
					m_bCheck[data - TILEX] = true;
				}

			if (0 <= data - TILEX - 1 && data - TILEX - 1 < 40000)
				if (!m_bCheck[data - TILEX - 1])
				{
					result.push_back(data - TILEX - 1);
					m_bCheck[data - TILEX - 1] = true;
				}

			if (0 <= data - TILEX * 2 && data - TILEX * 2 < 40000)
				if (!m_bCheck[data - TILEX * 2])
				{
					result.push_back(data - TILEX * 2);
					m_bCheck[data - TILEX * 2] = true;
				}
		}
		else if ((data / TILEX) % 2 == 1)
		{
			if (0 <= data - 1 && data - 1 < 40000)
				if (!m_bCheck[data - 1])
				{
					result.push_back(data - 1);
					m_bCheck[data - 1] = true;
				}

			if (0 <= data + 1 && data + 1 < 40000)
				if (!m_bCheck[data + 1])
				{
					result.push_back(data + 1);
					m_bCheck[data + 1] = true;
				}

			if (0 <= data + TILEX && data + TILEX < 40000)
				if (!m_bCheck[data + TILEX])
				{
					result.push_back(data + TILEX);
					m_bCheck[data + TILEX] = true;
				}

			if (0 <= data + TILEX + 1 && data + TILEX + 1 < 40000)
				if (!m_bCheck[data + TILEX + 1])
				{
					result.push_back(data + TILEX + 1);
					m_bCheck[data + TILEX + 1] = true;
				}

			if (0 <= data + TILEX * 2 && data + TILEX * 2 < 40000)
				if (!m_bCheck[data + TILEX * 2])
				{
					result.push_back(data + TILEX * 2);
					m_bCheck[data + TILEX * 2] = true;
				}

			if (0 <= data - TILEX && data - TILEX < 40000)
				if (!m_bCheck[data - TILEX])
				{
					result.push_back(data - TILEX);
					m_bCheck[data - TILEX] = true;
				}

			if (0 <= data - TILEX + 1 && data - TILEX + 1 < 40000)
				if (!m_bCheck[data - TILEX + 1])
				{
					result.push_back(data - TILEX + 1);
					m_bCheck[data - TILEX + 1] = true;
				}

			if (0 <= data - TILEX * 2 && data - TILEX * 2 < 40000)
				if (!m_bCheck[data - TILEX * 2])
				{
					result.push_back(data - TILEX * 2);
					m_bCheck[data - TILEX * 2] = true;
				}
		}
	}
}

bool  CQuarryScript::IsBlocked(UINT _iTile)
{
	static vector<tTile> vec{ 4, tTile{} };

	if ((_iTile / TILEX) % 2 == 0)
	{
		vec[0] = m_pTileObject->TileMap()->GetInfo(_iTile);
		vec[1] = m_pTileObject->TileMap()->GetInfo(_iTile + TILEX);
		vec[2] = m_pTileObject->TileMap()->GetInfo(_iTile + TILEX - 1);
		vec[3] = m_pTileObject->TileMap()->GetInfo(_iTile + TILEX * 2);
	}
	else if ((_iTile / TILEX) % 2 == 1)
	{
		vec[0] = m_pTileObject->TileMap()->GetInfo(_iTile);
		vec[1] = m_pTileObject->TileMap()->GetInfo(_iTile + TILEX);
		vec[2] = m_pTileObject->TileMap()->GetInfo(_iTile + TILEX + 1);
		vec[3] = m_pTileObject->TileMap()->GetInfo(_iTile + TILEX * 2);
	}

	for (auto iter{ vec.begin() }; iter != vec.end(); ++iter)
	{
		if ((UINT)TILE_TYPE::BUILD != iter->iInfo)
			return true;
	}

	for (auto iter{ m_vecMask.begin() }; iter != m_vecMask.end(); ++iter)
	{
		if ((UINT)TILE_TYPE::BEFORE_CRYSTAL == iter->iInfo)
			return false;
	}

	return true;
}

void CQuarryScript::clear()
{
	if (-1 != m_iIndex)
	{
		if ((m_iIndex / TILEX) % 2 == 0)
		{
			m_result.push_back(m_iIndex);
			m_result.push_back(m_iIndex + TILEX);
			m_result.push_back(m_iIndex + TILEX - 1);
			m_result.push_back(m_iIndex + TILEX * 2);
		}
		else if ((m_iIndex / TILEX) % 2 == 1)
		{
			m_result.push_back(m_iIndex);
			m_result.push_back(m_iIndex + TILEX);
			m_result.push_back(m_iIndex + TILEX + 1);
			m_result.push_back(m_iIndex + TILEX * 2);
		}

		SetTileInfo(m_queue, m_result, (UINT)TILE_TYPE::EMPTY);
		SetTileInfo(m_queue, m_result, (UINT)TILE_TYPE::CRYSTAL);
		SetTileInfo(m_queue, m_result, (UINT)TILE_TYPE::CRYSTAL);

		m_result.clear();

		for (size_t i{}; i < 40000; ++i)
			m_bCheck[i] = false;

		m_vecMask.clear();
	}
}

#include <Engine\CEngine.h>
#include <Engine\CFontMgr.h>
#include "CButtonScript.h"

void CQuarryScript::PhaseEventOn()
{
	__super::PhaseEventOn();

	lstrcpy(CEngine::g_szFullName, L"Quarry");

	wchar_t sz[200];

	lstrcpy(sz, to_wstring(m_fHP).c_str());
	lstrcat(sz, L"/");
	lstrcat(sz, to_wstring(m_fFullHp).c_str());

	lstrcpy(CEngine::g_szHp, sz);

	m_pPortrait->MeshRender()->Activate();

	CEngine::g_IconText.clear();

	SetIconUI(m_iIron, 0);

	const vector<CGameObject*> vec = CInterfaceMgr::GetInst()->GetTapButtons();
	for (size_t i{}; i < 6; ++i)
		vec[i]->GetScript<CButtonScript>()->SetColumn((UINT)TAP_CATEGORY_UPGRADE);

	GetOwner()->GetChilds()[0]->GetRenderComponent()->Activate();
}

void CQuarryScript::PhaseEventOff()
{
	__super::PhaseEventOff();

	m_pPortrait->MeshRender()->Deactivate();

	for (size_t i{}; i < m_vecIcon.size(); ++i)
		m_vecIcon[i]->MeshRender()->Deactivate();

	GetOwner()->GetChilds()[0]->GetRenderComponent()->Deactivate();
}
