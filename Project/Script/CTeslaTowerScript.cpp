#include "pch.h"
#include "CTeslaTowerScript.h"

#include <Engine\CDevice.h>
#include <Engine\CLevel.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CTransform.h>

#include <Engine\CJpsMgr.h>

#include <Engine\CInterfaceMgr.h>
#include <Script\CMouseScript.h>

CTeslaTowerScript::CTeslaTowerScript()
	:CScript{ SCRIPT_TYPE::TESLATOWERSCRIPT }
	, m_vMousePos{}
	, m_pTileObject{}
	, m_eBuildState{ BUILD_STATE::READY }
{
	m_fFullHp = 75;

	m_iArmor = 10;

	m_iGoldOut = 200;
	m_iWoodOut = 10;
	m_iIronOut = 0;

	Ptr<CPrefab> prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"CImageTeslaTowerPrefab");

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

	SetName(L"CTeslaTowerScript");
}

CTeslaTowerScript::~CTeslaTowerScript()
{
}

void CTeslaTowerScript::begin()
{
	__super::begin();

	m_pLevelMouseObject = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"MouseObject");
	m_pTileObject = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"LevelTile");

	GetOwner()->GetRenderComponent()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"BuildMtrl"));
	GetOwner()->GetRenderComponent()->SetInstancingType(INSTANCING_TYPE::USED);

	GetOwner()->GetChilds()[2]->GetRenderComponent()->Deactivate();
}

void CTeslaTowerScript::tick()
{
	__super::tick();
}

void CTeslaTowerScript::finaltick()
{
	m_fDt += DT;
	m_fDt2 += DT;

	if (0 > m_fHP)
	{
		//CEffectWoodPrefab
		CGameObject* pObj = CResMgr::GetInst()->FindRes<CPrefab>(L"CEffectExplosionPrefab")->Instantiate();
		Instantiate(pObj, Transform()->GetRelativePos(), 3);
		GetOwner()->Destroy();

		for (size_t i{}; i < m_vecBlock.size(); ++i)
			CJpsMgr::GetInst()->ClearCollision(m_vecBlock[i].x, m_vecBlock[i].z);
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

				m_result.push_back(tTile.iIndex);
				SetTileInfo(m_vec, m_result, (UINT)TILE_TYPE::USED);
				SetTileInfo(m_vec, m_result, (UINT)TILE_TYPE::BUILD);
				SetTileInfo(m_vec, m_result, (UINT)TILE_TYPE::BUILD);
				SetTileInfo(m_vec, m_result, (UINT)TILE_TYPE::BUILD);
				SetTileInfo(m_vec, m_result, (UINT)TILE_TYPE::BUILD);
				SetTileInfo(m_vec, m_result, (UINT)TILE_TYPE::BUILD);

				m_result.clear();

				for (size_t i{}; i < 40000; ++i)
					m_bCheck[i] = false;

				m_iIndex = tTile.iIndex;

				GetOwner()->Transform()->SetRelativePos(tTile.vPos);

				int a = 0;

				if (IsBlocked(m_iIndex))
					GetOwner()->GetRenderComponent()->GetDynamicMaterial()->SetScalarParam(INT_0, &a);
				else
				{
					a = 1;
					GetOwner()->GetRenderComponent()->GetDynamicMaterial()->SetScalarParam(INT_0, &a);
				}

				m_fDt = 0.f;
			}
		}

		if (m_fDt2 > 0.5f)
		{
			if (KEY_PRESSED(KEY::LBTN) && IsBlocked(m_iIndex))
			{
				m_result.push_back(m_iIndex);
				SetTileInfo(m_vec, m_result, (UINT)TILE_TYPE::NOTUSED);
				SetTileInfo(m_vec, m_result, (UINT)TILE_TYPE::EMPTY);
				SetTileInfo(m_vec, m_result, (UINT)TILE_TYPE::EMPTY);
				SetTileInfo(m_vec, m_result, (UINT)TILE_TYPE::EMPTY);
				SetTileInfo(m_vec, m_result, (UINT)TILE_TYPE::EMPTY);
				SetTileInfo(m_vec, m_result, (UINT)TILE_TYPE::EMPTY);

				m_result.clear();

				for (size_t i{}; i < 40000; ++i)
					m_bCheck[i] = false;

				m_eBuildState = BUILD_STATE::BUILD;
				m_fDt = 0.f;
				m_fDt2 = 0.f;

				Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"CTeslaTowerPrefab");
				CGameObject* pObj = pUIPrefab->Instantiate();
				CInterfaceMgr::GetInst()->SetBuildObj(pObj);
				m_vMousePos.z += Transform()->GetRelativeScale().z * 0.5f;
				Instantiate(pObj, m_vMousePos, 1);
			}
		}
	}
	else if (m_eBuildState == BUILD_STATE::BUILD)
	{
		m_fHP += DT * 10.f;

		if(m_fHP > m_fFullHp)
		{
			m_fHP = m_fFullHp;
			GetOwner()->GetRenderComponent()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ObjectMtrl"));
			GetOwner()->GetRenderComponent()->SetInstancingType(INSTANCING_TYPE::USED);
			m_eBuildState = BUILD_STATE::COMPLETE;
		}
	}
	else if (m_eBuildState == BUILD_STATE::COMPLETE)
	{
	}
}

void CTeslaTowerScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CTeslaTowerScript::Overlap(CCollider2D* _pOther)
{
}

void CTeslaTowerScript::EndOverlap(CCollider2D* _pOther)
{
}
void CTeslaTowerScript::SaveToFile(FILE* _File)
{
	CScript::SaveToFile(_File);
}

void CTeslaTowerScript::LoadFromFile(FILE* _File)
{
	CScript::LoadFromFile(_File);
}

bool  CTeslaTowerScript::IsBlocked(UINT _iTile)
{
	tTile tTile{};
	tTile = m_pTileObject->TileMap()->GetInfo(_iTile);

	if ((UINT)TILE_TYPE::NOTUSED == tTile.iInfo)
		return false;

	if ((UINT)TILE_TYPE::COLLISION == tTile.iInfo)
		return false;

	if ((UINT)TILE_TYPE::USED == tTile.iInfo)
		return true;

	return false;
}

void CTeslaTowerScript::SetTileInfo(vector<UINT>& que, vector<UINT>& result, UINT _value)
{
	que = result;

	result.clear();

	for (auto iter{ que.begin() }; iter != que.end(); )
	{
		UINT data = (*iter);

		tTile tTile = m_pTileObject->TileMap()->GetInfo(data);

		if ((UINT)TILE_TYPE::EMPTY == tTile.iInfo && (UINT)TILE_TYPE::USED == _value)
			m_pTileObject->TileMap()->SetInfo(data, _value);
		else if ((UINT)TILE_TYPE::USED == tTile.iInfo && (UINT)TILE_TYPE::EMPTY == _value)
			m_pTileObject->TileMap()->SetInfo(data, _value);
		else if ((UINT)TILE_TYPE::NOTUSED == tTile.iInfo && (UINT)TILE_TYPE::BUILD == _value)
			m_pTileObject->TileMap()->SetInfo(data, _value);
		else if ((UINT)TILE_TYPE::BUILD == tTile.iInfo && (UINT)TILE_TYPE::NOTUSED == _value)
			m_pTileObject->TileMap()->SetInfo(data, _value);
		else if ((UINT)TILE_TYPE::BUILD == tTile.iInfo && (UINT)TILE_TYPE::EMPTY == _value)
			m_pTileObject->TileMap()->SetInfo(data, _value);
		else if ((UINT)TILE_TYPE::USED == tTile.iInfo && (UINT)TILE_TYPE::NOTUSED == _value)
		{
			Int32 x = data % TILEX;
			Int32 z = data / TILEZ;
			m_vecBlock.push_back(tBlock{ x, z });
			CJpsMgr::GetInst()->SetCollision(x, z);

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
		iter = que.erase(iter);
	}
}

void CTeslaTowerScript::clear()
{
	if (-1 != m_iIndex)
	{

		m_result.push_back(m_iIndex);
		SetTileInfo(m_vec, m_result, (UINT)TILE_TYPE::EMPTY);
		SetTileInfo(m_vec, m_result, (UINT)TILE_TYPE::NOTUSED);
		SetTileInfo(m_vec, m_result, (UINT)TILE_TYPE::NOTUSED);
		SetTileInfo(m_vec, m_result, (UINT)TILE_TYPE::NOTUSED);
		SetTileInfo(m_vec, m_result, (UINT)TILE_TYPE::NOTUSED);
		SetTileInfo(m_vec, m_result, (UINT)TILE_TYPE::NOTUSED);

		m_result.clear();

		for (size_t i{}; i < 40000; ++i)
			m_bCheck[i] = false;
	}
}

#include <Engine\CEngine.h>
#include <Engine\CFontMgr.h>
#include "CButtonScript.h"

void CTeslaTowerScript::PhaseEventOn()
{
	__super::PhaseEventOn();

	lstrcpy(CEngine::g_szFullName, L"TeslaTower");

	//lstrcpy(g_szHp, to_wstring(m_fHp));
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
		vec[i]->GetScript<CButtonScript>()->SetColumn((UINT)TAP_CATEGORY_COMPLETE);

	GetOwner()->GetChilds()[2]->GetRenderComponent()->Activate();
}

void CTeslaTowerScript::PhaseEventOff()
{
	__super::PhaseEventOff();

	m_pPortrait->MeshRender()->Deactivate();

	for (size_t i{}; i < m_vecIcon.size(); ++i)
		m_vecIcon[i]->MeshRender()->Deactivate();

	GetOwner()->GetChilds()[2]->GetRenderComponent()->Deactivate();
}
