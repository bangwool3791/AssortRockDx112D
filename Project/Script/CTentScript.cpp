#include "pch.h"
#include "CTentScript.h"

#include <Engine\CDevice.h>
#include <Engine\CLevel.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CTransform.h>

#include <Engine\CInterfaceMgr.h>
#include <Engine\CJpsMgr.h>
#include <Script\CMouseScript.h>

CTentScript::CTentScript()
	:CScript{ SCRIPT_TYPE::TENTSCRIPT }
	, m_vMousePos{}
	, m_pTileObject{}
	, m_eBuildState{ BUILD_STATE::READY }
{
	m_fFullHp = 60;

	m_iGold = 4;
	m_iWorker = 4;
	m_iFood = -4;
	m_iColony = 4;

	SetName(L"CTentScript");

	Ptr<CPrefab> prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"CImageTenthousePrefab");

	m_pPortrait = prefab->Instantiate();
	m_pPortrait->Transform()->SetRelativePos(-220.f, 0.f, -550.f);
	m_pPortrait->MeshRender()->Deactivate();
	Instantiate(m_pPortrait, 31);

	prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"CDescGoldPrefab");
	m_vecIcon.push_back(prefab->Instantiate());
	prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"CDescWorkerPrefab");
	m_vecIcon.push_back(prefab->Instantiate());
	prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"CDescFoodPrefab");
	m_vecIcon.push_back(prefab->Instantiate());
	prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"CDescColonyPrefab");
	m_vecIcon.push_back(prefab->Instantiate());

	m_vecIcon[0]->Transform()->SetRelativePos(50.f, 0.f, -500.f);
	m_vecIcon[1]->Transform()->SetRelativePos(120.f, 0.f, -500.f);
	m_vecIcon[2]->Transform()->SetRelativePos(190.f, 0.f, -500.f);
	m_vecIcon[3]->Transform()->SetRelativePos(260.f, 0.f, -500.f);

	for (size_t i{}; i < m_vecIcon.size(); ++i)
		Instantiate(m_vecIcon[i], 31);

	for (size_t i{}; i < m_vecIcon.size(); ++i)
		m_vecIcon[i]->MeshRender()->Deactivate();
}

CTentScript::~CTentScript()
{
}

void CTentScript::begin()
{
	GetOwner()->GetRenderComponent()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"BuildMtrl"));
	GetOwner()->GetRenderComponent()->SetInstancingType(INSTANCING_TYPE::USED);
	//GetOwner()->GetRenderComponent()->GetCurMaterial()->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Mask\\buildmask.png"));


	m_pLevelMouseObject = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"MouseObject");
	m_pTileObject = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"LevelTile");

	GetOwner()->GetChilds()[0]->GetRenderComponent()->Deactivate();

	int random = rand();

	if (random % 4 == 0)
		GetOwner()->Animator2D()->Play(L"Right1", false);
	else if (random % 4 == 1)
		GetOwner()->Animator2D()->Play(L"Right2", false);
	else if (random % 4 == 2)
		GetOwner()->Animator2D()->Play(L"Left1", false);
	else if (random % 4 == 3)
		GetOwner()->Animator2D()->Play(L"Left2", false);
}

void CTentScript::tick()
{
	__super::tick();
}

void CTentScript::finaltick()
{
	if (0 > m_fHP)
	{
		//CEffectWoodPrefab
		CGameObject* pObj = CResMgr::GetInst()->FindRes<CPrefab>(L"CEffectWoodPrefab")->Instantiate();
		Instantiate(pObj, Transform()->GetRelativePos(), 3);
		GetOwner()->Destroy();
	}

	m_fDt += DT;
	m_fDt2 += DT;

	if (BUILD_STATE::READY == m_eBuildState)
	{
		if (m_fDt > 0.15f)
		{
			const Ray& ray = GetRay();

			m_vMousePos = m_pTileObject->GetRenderComponent()->GetMesh()->GetPosition(ray);

			tTile tTile = m_pTileObject->TileMap()->GetInfo(m_vMousePos);

			clear();

			SetTile(tTile.iIndex);

			tTile.vPos.z += TILECZ * 0.5f;
			m_iIndex = tTile.iIndex;
			GetOwner()->Transform()->SetRelativePos(tTile.vPos);

			int a = 0;

			if (IsBlocked(m_iIndex))
			{
				a = 1;
				GetOwner()->GetRenderComponent()->GetDynamicMaterial()->SetScalarParam(INT_0, &a);
			}
			else
				GetOwner()->GetRenderComponent()->GetDynamicMaterial()->SetScalarParam(INT_0, &a);

			m_fDt -= 0.25f;
		}

		if (m_fDt2 > 0.5f)
		{
			if (KEY_PRESSED(KEY::LBTN) && !IsBlocked(m_iIndex))
			{
				SetTile(m_iIndex, (UINT)TILE_TYPE::HARVEST);
				m_eBuildState = BUILD_STATE::BUILD;
				m_fDt = 0.f;
				m_fDt2 = 0.f;

				Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"TentPrefab");
				CGameObject* pObj = pUIPrefab->Instantiate();
				CInterfaceMgr::GetInst()->SetBuildObj(pObj);
				Instantiate(pObj, m_vMousePos, 1);

			}
			m_fDt2 = 0.5f;
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
		}
	}
	else if (m_eBuildState == BUILD_STATE::COMPLETE)
	{
	}
}

void CTentScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CTentScript::Overlap(CCollider2D* _pOther)
{
}

void CTentScript::EndOverlap(CCollider2D* _pOther)
{
}
void CTentScript::SaveToFile(FILE* _File)
{
	CScript::SaveToFile(_File);
}

void CTentScript::LoadFromFile(FILE* _File)
{
	CScript::LoadFromFile(_File);
}

void CTentScript::SetTileInfo(UINT _iTile, UINT _iValue)
{
	if ((UINT)TILE_TYPE::HARVEST == _iValue)
	{
		Int32 x = _iTile % TILEX;
		Int32 z = _iTile / TILEZ;
		m_vecBlock.push_back(tBlock{ x, z });
		CJpsMgr::GetInst()->SetCollision(x, z);
	}

	m_pTileObject->TileMap()->SetInfo(_iTile, _iValue);
}

void  CTentScript::SetTile(UINT _iTile, UINT _iValue)
{
	if ((_iTile / TILEX) % 2 == 0)
	{
		SetTileInfo(_iTile, _iValue);
		SetTileInfo(_iTile + TILEX, _iValue);
		SetTileInfo(_iTile + TILEX - 1, _iValue);
		SetTileInfo(_iTile + TILEX * 2, _iValue);
	}
	else if ((_iTile / TILEX) % 2 == 1)
	{
		SetTileInfo(_iTile, _iValue);
		SetTileInfo(_iTile + TILEX, _iValue);
		SetTileInfo(_iTile + TILEX + 1, _iValue);
		SetTileInfo(_iTile + TILEX * 2, _iValue);
	}
}

void CTentScript::SetTileInfo(UINT _iTile)
{
	tTile tTile = m_pTileObject->TileMap()->GetInfo(_iTile);

	if ((UINT)TILE_TYPE::EMPTY == tTile.iInfo)
	{
		m_pTileObject->TileMap()->SetInfo(_iTile, (UINT)TILE_TYPE::BUILD);
	}
	else if ((UINT)TILE_TYPE::BUILD == tTile.iInfo)
	{
		m_pTileObject->TileMap()->SetInfo(_iTile, (UINT)TILE_TYPE::EMPTY);
	}
	else if ((UINT)TILE_TYPE::USED == tTile.iInfo)
	{
		m_pTileObject->TileMap()->SetInfo(_iTile, (UINT)TILE_TYPE::COLLISION);
	}
}


void  CTentScript::SetTile(UINT _iTile)
{
	if ((_iTile / TILEX) % 2 == 0)
	{
		SetTileInfo(_iTile);
		SetTileInfo(_iTile + TILEX);
		SetTileInfo(_iTile + TILEX - 1);
		SetTileInfo(_iTile + TILEX * 2);
	}
	else if ((_iTile / TILEX) % 2 == 1)
	{
		SetTileInfo(_iTile);
		SetTileInfo(_iTile + TILEX);
		SetTileInfo(_iTile + TILEX + 1);
		SetTileInfo(_iTile + TILEX * 2);
	}
}


void  CTentScript::RefreshTile(UINT _iTile)
{
	if ((_iTile / TILEX) % 2 == 0)
	{
		RefreshTileInfo(_iTile);
		RefreshTileInfo(_iTile + TILEX);
		RefreshTileInfo(_iTile + TILEX - 1);
		RefreshTileInfo(_iTile + TILEX * 2);
	}
	else if ((_iTile / TILEX) % 2 == 1)
	{
		RefreshTileInfo(_iTile);
		RefreshTileInfo(_iTile + TILEX);
		RefreshTileInfo(_iTile + TILEX + 1);
		RefreshTileInfo(_iTile + TILEX * 2);
	}
}

void CTentScript::RefreshTileInfo(UINT _iTile)
{
	tTile tTile = m_pTileObject->TileMap()->GetInfo(_iTile);

	if ((UINT)TILE_TYPE::BUILD == tTile.iInfo)
	{
		m_pTileObject->TileMap()->SetInfo(_iTile, (UINT)TILE_TYPE::EMPTY);
	}
	else if ((UINT)TILE_TYPE::COLLISION == tTile.iInfo)
	{
		m_pTileObject->TileMap()->SetInfo(_iTile, (UINT)TILE_TYPE::USED);
	}
}

bool  CTentScript::IsBlocked(UINT _iTile)
{
	static vector<tTile> vec(4, tTile{});

	if ((_iTile / TILEX) % 2 == 0)
	{
		vec[0] = m_pTileObject->TileMap()->GetInfo(_iTile);
		vec[1] = m_pTileObject->TileMap()->GetInfo(_iTile + TILEX);
		vec[2] = m_pTileObject->TileMap()->GetInfo(_iTile + TILEX -1);
		vec[3] = m_pTileObject->TileMap()->GetInfo(_iTile + TILEX * 2);
	}
	else if ((_iTile / TILEX) % 2 == 1)
	{
		vec[0] = m_pTileObject->TileMap()->GetInfo(_iTile);
		vec[1] = m_pTileObject->TileMap()->GetInfo(_iTile + TILEX);
		vec[2] = m_pTileObject->TileMap()->GetInfo(_iTile + TILEX + 1);
		vec[3] = m_pTileObject->TileMap()->GetInfo(_iTile + TILEX * 2);
	}

	for (size_t i{}; i < vec.size(); ++i)
	{
		if ((UINT)TILE_TYPE::BUILD != vec[i].iInfo)
			return true;
	}
	return false;
}

void CTentScript::clear()
{
	if (-1 != m_iIndex)
	{
		RefreshTile(m_iIndex);
	}
}

#include <Engine\CEngine.h>
#include <Engine\CFontMgr.h>
#include "CButtonScript.h"

void CTentScript::PhaseEventOn()
{
	__super::PhaseEventOn();

	lstrcpy(CEngine::g_szFullName, L"TentHose");

	//lstrcpy(g_szHp, to_wstring(m_fHP));
	wchar_t sz[200];

	lstrcpy(sz, to_wstring(m_fHP).c_str());
	lstrcat(sz, L"/");
	lstrcat(sz, to_wstring(m_fFullHp).c_str());

	lstrcpy(CEngine::g_szHp, sz);

	m_pPortrait->MeshRender()->Activate();

	CEngine::g_IconText.clear();

	SetIconUI(m_iGold, 0);
	SetIconUI(m_iWorker, 1);
	SetIconUI(m_iFood, 2);
	SetIconUI(m_iColony, 3);

	const vector<CGameObject*> vec = CInterfaceMgr::GetInst()->GetTapButtons();
	for (size_t i{}; i < 6; ++i)
		vec[i]->GetScript<CButtonScript>()->SetColumn((UINT)TAP_CATEGORY_UPGRADE);

	GetOwner()->GetChilds()[0]->GetRenderComponent()->Activate();
}

void CTentScript::PhaseEventOff()
{
	__super::PhaseEventOff();

	m_pPortrait->MeshRender()->Deactivate();

	for (size_t i{}; i < m_vecIcon.size(); ++i)
		m_vecIcon[i]->GetRenderComponent()->Deactivate();

	GetOwner()->GetChilds()[0]->GetRenderComponent()->Deactivate();
}
