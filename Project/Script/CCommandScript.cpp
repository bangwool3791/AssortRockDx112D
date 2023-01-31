#include "pch.h"
#include "CCommandScript.h"

#include <Engine\CEngine.h>
#include <Engine\CDevice.h>
#include <Engine\CLevel.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CTransform.h>

#include <Engine\CInterfaceMgr.h>
#include <Script\CMouseScript.h>

#include <Engine\CJpsMgr.h>

#include "CButtonScript.h"

CCommandScript::CCommandScript()
	:CScript{ SCRIPT_TYPE::COMMANDSCRIPT }
	, m_vMousePos{}
	, m_pTileObject{}
{
	m_eBuildState = BUILD_STATE::COMPLETE;
	m_fHP = 5000;
	m_fFullHp = 5000;

	m_iGold = 200;
	m_iWorker = 10;
	m_iFood = 20;
	m_iStorage = 50;

	SetName(L"CCommandScript");

	Ptr<CPrefab> prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"CImageCommandCenterPrefab");

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
	prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"CDescStoragePrefab");
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

CCommandScript::~CCommandScript()
{
}


void CCommandScript::begin()
{
	m_pLevelMouseObject = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"MouseObject");
	m_pTileObject = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"LevelTile");

	Vec3 vCenter = GetOwner()->Transform()->GetRelativePos();
	Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();

	Vec3 vLeftTop = Vec3{ vCenter.x - vScale.x * 0.5f , 1.f ,vCenter.z + vScale.z * 0.5f };
	Vec3 vRightBottom = Vec3{ vCenter.x + vScale.x * 0.5f , 1.f ,vCenter.z - vScale.z * 0.5f };

	tTile ttile = m_pTileObject->TileMap()->GetInfo(vCenter);

	queue<UINT> que;
	queue<UINT> result;

	result.push(ttile.iIndex);
	SetTileInfo(que, result,(UINT)TILE_TYPE::EMPTY);
	SetTileInfo(que, result,(UINT)TILE_TYPE::EMPTY);
	SetTileInfo(que, result,(UINT)TILE_TYPE::EMPTY);
	SetTileInfo(que, result,(UINT)TILE_TYPE::EMPTY);
	SetTileInfo(que, result,(UINT)TILE_TYPE::EMPTY);
	SetTileInfo(que, result,(UINT)TILE_TYPE::EMPTY);
	SetTileInfo(que, result,(UINT)TILE_TYPE::EMPTY);
	SetTileInfo(que, result,(UINT)TILE_TYPE::EMPTY);
	SetTileInfo(que, result,(UINT)TILE_TYPE::EMPTY);
	SetTileInfo(que, result,(UINT)TILE_TYPE::EMPTY);

	while (!result.empty())
		result.pop();

	for (size_t i{}; i < 40000; ++i)
		m_bCheck[i] = false;

	result.push(ttile.iIndex);
	SetTileInfo(que, result,(UINT)TILE_TYPE::NOTUSED);
	SetTileInfo(que, result,(UINT)TILE_TYPE::NOTUSED);
	SetTileInfo(que, result,(UINT)TILE_TYPE::NOTUSED);

	GetOwner()->Transform()->SetRelativePos(ttile.vPos);

	GetOwner()->GetChilds()[2]->GetRenderComponent()->Deactivate();
}

void CCommandScript::tick()
{
	__super::tick();

	if (0 > m_fHP)
		GetOwner()->Destroy();
}

void CCommandScript::finaltick()
{
	//if (KEY_PRESSED(KEY::LBTN))
	//{

	//	const Ray& ray = GetRay();
	//	Vec3 vPos{};

	//	if (GetOwner()->Transform()->Picking(ray, vPos))
	//	{
	//		m_pTileObject->TileMap()->On();
	//		CInterfaceMgr::GetInst()->SetTarget(GetOwner());
	//	}
	//}
}

void CCommandScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CCommandScript::Overlap(CCollider2D* _pOther)
{
}

void CCommandScript::EndOverlap(CCollider2D* _pOther)
{
}
void CCommandScript::SaveToFile(FILE* _File)
{
	CScript::SaveToFile(_File);
}

void CCommandScript::LoadFromFile(FILE* _File)
{
	CScript::LoadFromFile(_File);
}

void CCommandScript::SetTileInfo(queue<UINT>& que, queue<UINT>& result, UINT value)
{
	que = result;
	
	while (!result.empty())
		result.pop();

	while (!que.empty())
	{
		UINT data = que.front();
		que.pop();

		tTile tTile = m_pTileObject->TileMap()->GetInfo(data);

		if ((UINT)TILE_TYPE::NOTUSED == tTile.iInfo)
			m_pTileObject->TileMap()->SetInfo(data, value);
		else if ((UINT)TILE_TYPE::EMPTY == tTile.iInfo)
			m_pTileObject->TileMap()->SetInfo(data, value);

		m_bCheck[data] = true;

		if (((UINT)TILE_TYPE::NOTUSED) == value)
		{
			Int32 x = data % TILEX;
			Int32 z = data / TILEZ;
			m_vecBlock.push_back(tBlock{ x, z });
			CJpsMgr::GetInst()->SetCollision(x, z);
		}

		if ((data / TILEX) % 2 == 0)
		{
			if (0 <= data - 1 && data - 1 < 40000)
				if (!m_bCheck[data - 1])
				{
					result.push(data - 1);
					m_bCheck[data - 1] = true;
				}

			if (0 <= data + 1 && data + 1 < 40000)
				if (!m_bCheck[data + 1])
				{
					result.push(data + 1);
					m_bCheck[data + 1] = true;
				}

			if (0 <= data + TILEX && data + TILEX < 40000)
				if (!m_bCheck[data + TILEX])
				{
					result.push(data + TILEX);
					m_bCheck[data + TILEX] = true;
				}

			if (0 <= data + TILEX - 1 && data + TILEX - 1 < 40000)
				if (!m_bCheck[data + TILEX - 1])
				{
					result.push(data + TILEX - 1);
					m_bCheck[data + TILEX - 1] = true;
				}

			if (0 <= data + TILEX * 2 && data + TILEX * 2 < 40000)
				if (!m_bCheck[data + TILEX * 2])
				{
					result.push(data + TILEX * 2);
					m_bCheck[data + TILEX * 2] = true;
				}

			if (0 <= data - TILEX && data - TILEX < 40000)
				if (!m_bCheck[data - TILEX])
				{
					result.push(data - TILEX);
					m_bCheck[data - TILEX] = true;
				}

			if (0 <= data - TILEX - 1 && data - TILEX - 1 < 40000)
				if (!m_bCheck[data - TILEX - 1])
				{
					result.push(data - TILEX - 1);
					m_bCheck[data - TILEX - 1] = true;
				}

			if (0 <= data - TILEX * 2 && data - TILEX * 2 < 40000)
				if (!m_bCheck[data - TILEX * 2])
				{
					result.push(data - TILEX * 2);
					m_bCheck[data - TILEX * 2] = true;
				}
		}
		else if ((data / TILEX) % 2 == 1)
		{
			if (0 <= data - 1 && data - 1 < 40000)
				if (!m_bCheck[data - 1])
				{
					result.push(data - 1);
					m_bCheck[data - 1] = true;
				}

			if (0 <= data + 1 && data + 1 < 40000)
				if (!m_bCheck[data + 1])
				{
					result.push(data + 1);
					m_bCheck[data + 1] = true;
				}

			if (0 <= data + TILEX && data + TILEX < 40000)
				if (!m_bCheck[data + TILEX])
				{
					result.push(data + TILEX);
					m_bCheck[data + TILEX] = true;
				}

			if (0 <= data + TILEX + 1 && data + TILEX + 1 < 40000)
				if (!m_bCheck[data + TILEX + 1])
				{
					result.push(data + TILEX + 1);
					m_bCheck[data + TILEX + 1] = true;
				}

			if (0 <= data + TILEX * 2 && data + TILEX * 2 < 40000)
				if (!m_bCheck[data + TILEX * 2])
				{
					result.push(data + TILEX * 2);
					m_bCheck[data + TILEX * 2] = true;
				}

			if (0 <= data - TILEX && data - TILEX < 40000)
				if (!m_bCheck[data - TILEX])
				{
					result.push(data - TILEX);
					m_bCheck[data - TILEX] = true;
				}

			if (0 <= data - TILEX + 1 && data - TILEX + 1 < 40000)
				if (!m_bCheck[data - TILEX + 1])
				{
					result.push(data - TILEX + 1);
					m_bCheck[data - TILEX + 1] = true;
				}

			if (0 <= data - TILEX * 2 && data - TILEX * 2 < 40000)
				if (!m_bCheck[data - TILEX * 2])
				{
					result.push(data - TILEX * 2);
					m_bCheck[data - TILEX * 2] = true;
				}
		}
	}
}

#include <Engine\CEngine.h>
#include <Engine\CFontMgr.h>

void CCommandScript::PhaseEventOn()
{
	__super::PhaseEventOn();

	lstrcpy(CEngine::g_szFullName, L"CommandCenter");
	
	//lstrcpy(g_szHp, to_wstring(m_fHP));
	wchar_t sz[200];

	lstrcpy(sz, to_wstring(m_fHP).c_str());
	lstrcat(sz, L"/");
	lstrcat(sz, to_wstring(m_fFullHp).c_str());

	lstrcpy(CEngine::g_szHp, sz);

	m_pTileObject->TileMap()->On();
	
	m_pPortrait->MeshRender()->Activate();

	CEngine::g_IconText.clear();

	SetIconUI(m_iGold, 0);
	SetIconUI(m_iWorker, 1);
	SetIconUI(m_iFood, 2);
	SetIconUI(m_iStorage, 3);

	const vector<CGameObject*> vec = CInterfaceMgr::GetInst()->GetTapButtons();
	for (size_t i{}; i < 6; ++i)
		vec[i]->GetScript<CButtonScript>()->SetColumn((UINT)COMMAND_CENTER);

	GetOwner()->GetChilds()[2]->GetRenderComponent()->Activate();
}

void CCommandScript::PhaseEventOff()
{
	__super::PhaseEventOff();

	m_pTileObject->TileMap()->Off();

	m_pPortrait->MeshRender()->Deactivate();

	for (size_t i{}; i < m_vecIcon.size(); ++i)
		m_vecIcon[i]->MeshRender()->Deactivate();

	GetOwner()->GetChilds()[2]->GetRenderComponent()->Deactivate();
}

