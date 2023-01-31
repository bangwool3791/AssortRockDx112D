#include "pch.h"
#include "CSCScript.h"

#include <Engine\CEngine.h>
#include <Engine\CDevice.h>
#include <Engine\CLevel.h>
#include <Engine\CJpsMgr.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CTransform.h>

#include <Engine\CInterfaceMgr.h>

#include "CButtonScript.h"

CSCScript::CSCScript()
	:CScript{ SCRIPT_TYPE::SCSCRIPT }
	, m_vMousePos{}
	, m_pTileObject{}
	, m_eBuildState{ BUILD_STATE::READY }
	, m_pGameObject{}
{
	m_fFullHp = 500;

	m_iGold = -14;
	m_iWorker = -8;

	SetName(L"CSCScript");

	 Ptr<CPrefab> prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"CImageSCPrefab");

	m_pPortrait = prefab->Instantiate();
	m_pPortrait->Transform()->SetRelativePos(-220.f, 0.f, -550.f);
	m_pPortrait->MeshRender()->Deactivate();
	Instantiate(m_pPortrait, 31);

	prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"CDescGoldPrefab");
	m_vecIcon.push_back(prefab->Instantiate());
	prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"CDescWorkerPrefab");
	m_vecIcon.push_back(prefab->Instantiate());

	m_vecIcon[0]->Transform()->SetRelativePos(50.f, 0.f, -500.f);
	m_vecIcon[1]->Transform()->SetRelativePos(120.f, 0.f, -500.f);

	for (size_t i{}; i < m_vecIcon.size(); ++i)
		Instantiate(m_vecIcon[i], 31);

	for (size_t i{}; i < m_vecIcon.size(); ++i)
		m_vecIcon[i]->MeshRender()->Deactivate();
}

CSCScript::~CSCScript()
{
}

void CSCScript::begin()
{
	m_pLevelMouseObject = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"MouseObject");
	m_pTileObject = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"LevelTile");

	GetOwner()->GetRenderComponent()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"BuildMtrl"));
	GetOwner()->GetRenderComponent()->SetInstancingType(INSTANCING_TYPE::USED);

	Ptr<CPrefab> prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"CDescProgressFramePrefab");
	m_pProgressBar = prefab->Instantiate();
	m_pProgressBar->Transform()->SetRelativePos(150.f, 0.f, -500.f);
	m_pProgressBar->GetChilds()[0]->Transform()->SetRelativePos(-150.f, 0.f, 0.f);
	m_pProgressBar->MeshRender()->Deactivate();
	m_pProgressBar->GetChilds()[0]->MeshRender()->Deactivate();

	Instantiate(m_pProgressBar, 31);

	prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"CRallypointPrefab");

	m_pRallyPoint = prefab->Instantiate();
	m_pRallyPoint->GetRenderComponent()->Deactivate();

	Instantiate(m_pRallyPoint, 0);

	GetOwner()->GetChilds()[2]->GetRenderComponent()->Deactivate();
}

void CSCScript::tick()
{
	__super::tick();
}

void CSCScript::finaltick()
{
	if (0 > m_fHP)
	{
		//CEffectWoodPrefab
		CGameObject* pObj = CResMgr::GetInst()->FindRes<CPrefab>(L"CEffectExplosionPrefab")->Instantiate();
		Instantiate(pObj, Transform()->GetRelativePos(), 3);
		GetOwner()->Destroy();
	}
	m_fDt += DT;
	m_fDt2 += DT;

	if (m_bRallyPoint)
	{
		const Ray& ray = GetRay();

		Vec3 vPos = m_pTileObject->GetRenderComponent()->GetMesh()->GetPosition(ray);

		tTile tTile = m_pTileObject->TileMap()->GetInfo(vPos);

		m_pRallyPoint->Transform()->SetRelativePos(tTile.vPos);

		if (KEY_PRESSED(KEY::RBTN))
		{
			m_bRallyPoint = false;
			m_IsRallyPoint = true;
		}
	}

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

			m_fDt -= 0.15f;
		}

		if (m_fDt2 > 0.5f)
		{
			if (KEY_PRESSED(KEY::LBTN) && !IsBlocked(m_iIndex))
			{
				SetTile(m_iIndex, (UINT)TILE_TYPE::HARVEST);
				m_eBuildState = BUILD_STATE::BUILD;

				Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"SCPrefab");
				m_pBuildObj = pUIPrefab->Instantiate();
				CInterfaceMgr::GetInst()->SetBuildObj(m_pBuildObj);
				Instantiate(m_pBuildObj, m_vMousePos, 1);
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
	else if (m_eBuildState == BUILD_STATE::CREATE_UNIT)
	{
		CGameObject* pObj = m_pProgressBar->GetChilds()[0];
		Vec3 vScale = pObj->Transform()->GetRelativeScale();

		float x = m_fDt / 10.f * m_pProgressBar->Transform()->GetRelativeScale().x;

		vScale.x = x;

		float fCX = m_pProgressBar->Transform()->GetRelativeScale().x;

		pObj->Transform()->SetRelativePos(Vec3(-0.5f * fCX + x * 0.5f, 0.f, 0.f));
		pObj->Transform()->SetRelativeScale(vScale);

		if (m_fDt >= 10.f)
		{
			CGameObject* pObj = m_queUnit.front();
			const wstring& str = pObj->GetName();

			if (str == L"CImageRanger")
			{
				m_strPrefab = L"CRangerPrefab";
			}
			else if (str == L"CImageSolider")
			{
				m_strPrefab = L"SoldierPrefab";
			}
			else if (str ==  L"CImageSniper")
			{
				m_strPrefab = L"CSniperPrefab";
			}

			Ptr<CPrefab> prefab = CResMgr::GetInst()->FindRes<CPrefab>(m_strPrefab);
			CGameObject* pUnit = prefab->Instantiate();
			Vec3 vPos{};

			vPos = Transform()->GetRelativePos();
			vPos.z -= (Transform()->GetRelativeScale().z + pUnit->Transform()->GetRelativeScale().z) * 0.5f;
			Instantiate(pUnit, vPos, 1);

			if (m_IsRallyPoint)
			{
				Vec3 vPos = m_pRallyPoint->Transform()->GetRelativePos();
				pUnit->GetScripts()[0]->begin();
				tTile tTile = CJpsMgr::GetInst()->GetTileObj()->TileMap()->GetInfo(vPos);
				Int32 x1 = tTile.iIndex % TILEX;
				Int32 z1 = tTile.iIndex / TILEX;
				pUnit->GetScripts()[0]->Move(x1, z1);
			}


			m_strPrefab.clear();

			pObj->Destroy();

			m_queUnit.pop();
			
			if (this->GetOwner() == CInterfaceMgr::GetInst()->GetTarget())
			{
				for (size_t i{ 0 }; i < m_pCreateUnit->GetChilds().size(); ++i)
					m_pCreateUnit->GetChilds()[i]->MeshRender()->Activate();

				for (size_t i{}; i < m_queUnit.size(); ++i)
				{
					CGameObject* pObj = m_queUnit.front();
					m_queUnit.pop();

					Vec3 vScale = m_pCreateUnit->GetChilds()[i]->Transform()->GetRelativeScale();
					pObj->Transform()->SetRelativeScale(vScale);
					pObj->Transform()->SetRelativePos(60 * i, 0.f, -600.f);
					m_pCreateUnit->GetChilds()[i]->MeshRender()->Deactivate();

					m_queUnit.push(pObj);
				}
			}
			CreateEnd();
			m_fDt = 0.f;
		}
	}
}

void CSCScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CSCScript::Overlap(CCollider2D* _pOther)
{
}

void CSCScript::EndOverlap(CCollider2D* _pOther)
{
}
void CSCScript::SaveToFile(FILE* _File)
{
	CScript::SaveToFile(_File);
}

void CSCScript::LoadFromFile(FILE* _File)
{
	CScript::LoadFromFile(_File);
}

void CSCScript::SetTileInfo(UINT _iTile, UINT _iValue)
{
	if ((UINT)TILE_TYPE::HARVEST == _iValue)
	{
		Int32 x = _iTile % TILEX;
		Int32 z = _iTile / TILEZ;
		m_vecBlock.push_back(tBlock{x, z});
		CJpsMgr::GetInst()->SetCollision(x, z);
	}
	m_pTileObject->TileMap()->SetInfo(_iTile, _iValue);
}

void  CSCScript::SetTile(UINT _iTile, UINT _iValue)
{
	if ((_iTile / TILEX) % 2 == 0)
	{
		SetTileInfo(_iTile, _iValue);
		SetTileInfo(_iTile - 1, _iValue);
		SetTileInfo(_iTile + 1, _iValue);
		SetTileInfo(_iTile + TILEX, _iValue);
		SetTileInfo(_iTile + TILEX - 1, _iValue);
		SetTileInfo(_iTile + TILEX * 2, _iValue);
		SetTileInfo(_iTile - TILEX, _iValue);
		SetTileInfo(_iTile - TILEX - 1, _iValue);
		SetTileInfo(_iTile - TILEX * 2, _iValue);
	}
	else if ((_iTile / TILEX) % 2 == 1)
	{
		SetTileInfo(_iTile, _iValue);
		SetTileInfo(_iTile - 1, _iValue);
		SetTileInfo(_iTile + 1, _iValue);
		SetTileInfo(_iTile + TILEX, _iValue);
		SetTileInfo(_iTile + TILEX + 1, _iValue);
		SetTileInfo(_iTile + TILEX * 2, _iValue);
		SetTileInfo(_iTile - TILEX, _iValue);
		SetTileInfo(_iTile - TILEX + 1, _iValue);
		SetTileInfo(_iTile - TILEX * 2, _iValue);
	}
}

void CSCScript::SetTileInfo(UINT _iTile)
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


void  CSCScript::SetTile(UINT _iTile)
{
	if ((_iTile / TILEX) % 2 == 0)
	{
		SetTileInfo(_iTile);
		SetTileInfo(_iTile - 1);
		SetTileInfo(_iTile + 1);
		SetTileInfo(_iTile + TILEX);
		SetTileInfo(_iTile + TILEX - 1);
		SetTileInfo(_iTile + TILEX * 2);
		SetTileInfo(_iTile - TILEX);
		SetTileInfo(_iTile - TILEX - 1);
		SetTileInfo(_iTile - TILEX * 2);
	}
	else if ((_iTile / TILEX) % 2 == 1)
	{
		SetTileInfo(_iTile);
		SetTileInfo(_iTile - 1);
		SetTileInfo(_iTile + 1);
		SetTileInfo(_iTile + TILEX);
		SetTileInfo(_iTile + TILEX + 1);
		SetTileInfo(_iTile + TILEX * 2);
		SetTileInfo(_iTile - TILEX);
		SetTileInfo(_iTile - TILEX + 1);
		SetTileInfo(_iTile - TILEX * 2);
	}
}


void  CSCScript::RefreshTile(UINT _iTile)
{
	if ((_iTile / TILEX) % 2 == 0)
	{
		RefreshTileInfo(_iTile);
		RefreshTileInfo(_iTile - 1);
		RefreshTileInfo(_iTile + 1);
		RefreshTileInfo(_iTile + TILEX);
		RefreshTileInfo(_iTile + TILEX - 1);
		RefreshTileInfo(_iTile + TILEX * 2);
		RefreshTileInfo(_iTile - TILEX);
		RefreshTileInfo(_iTile - TILEX - 1);
		RefreshTileInfo(_iTile - TILEX * 2);
	}
	else if ((_iTile / TILEX) % 2 == 1)
	{
		RefreshTileInfo(_iTile);
		RefreshTileInfo(_iTile - 1);
		RefreshTileInfo(_iTile + 1);
		RefreshTileInfo(_iTile + TILEX);
		RefreshTileInfo(_iTile + TILEX + 1);
		RefreshTileInfo(_iTile + TILEX * 2);
		RefreshTileInfo(_iTile - TILEX);
		RefreshTileInfo(_iTile - TILEX + 1);
		RefreshTileInfo(_iTile - TILEX * 2);
	}
}

void CSCScript::RefreshTileInfo(UINT _iTile)
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

bool  CSCScript::IsBlocked(int _iTile)
{
	static vector<tTile> vec(9, tTile{});
	vec.resize(9);
	if ((_iTile / TILEX) % 2 == 0)
	{
		vec[0] = m_pTileObject->TileMap()->GetInfo(_iTile);
		vec[1] = m_pTileObject->TileMap()->GetInfo(_iTile - 1);
		vec[2] = m_pTileObject->TileMap()->GetInfo(_iTile + 1);
		vec[3] = m_pTileObject->TileMap()->GetInfo(_iTile + TILEX);
		vec[4] = m_pTileObject->TileMap()->GetInfo(_iTile + TILEX - 1);
		vec[5] = m_pTileObject->TileMap()->GetInfo(_iTile + TILEX * 2);
		vec[6] = m_pTileObject->TileMap()->GetInfo(_iTile - TILEX);
		vec[7] = m_pTileObject->TileMap()->GetInfo(_iTile - TILEX - 1);
		vec[8] = m_pTileObject->TileMap()->GetInfo(_iTile + TILEX * 2);
	}
	else if ((_iTile / TILEX) % 2 == 1)
	{
		vec[0] = m_pTileObject->TileMap()->GetInfo(_iTile);
		vec[1] = m_pTileObject->TileMap()->GetInfo(_iTile - 1);
		vec[2] = m_pTileObject->TileMap()->GetInfo(_iTile + 1);
		vec[3] = m_pTileObject->TileMap()->GetInfo(_iTile + TILEX);
		vec[4] = m_pTileObject->TileMap()->GetInfo(_iTile + TILEX + 1);
		vec[5] = m_pTileObject->TileMap()->GetInfo(_iTile + TILEX * 2);
		vec[6] = m_pTileObject->TileMap()->GetInfo(_iTile - TILEX);
		vec[7] = m_pTileObject->TileMap()->GetInfo(_iTile - TILEX + 1);
		vec[8] = m_pTileObject->TileMap()->GetInfo(_iTile - TILEX * 2);
	}

	for (size_t i{}; i < vec.size(); ++i)
	{
		if ((UINT)TILE_TYPE::BUILD != vec[i].iInfo)
			return true;
	}
	return false;
}

void CSCScript::CreateUnit(const wstring& _str)
{
	if ((BUILD_STATE::COMPLETE == m_eBuildState || BUILD_STATE::CREATE_UNIT == m_eBuildState) && 5 > m_queUnit.size())
	{
		m_pProgressBar->MeshRender()->Activate();
		m_pProgressBar->GetChilds()[0]->MeshRender()->Activate();

		CGameObject* pButton{};
		Ptr<CPrefab> prefab{};

		if (m_queUnit.empty())
		{
			m_fDt = 0.f;

			CEngine::g_IconText.clear();

			for (size_t i{}; i < m_vecIcon.size(); ++i)
				m_vecIcon[i]->MeshRender()->Deactivate();

			Ptr<CPrefab> prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"CDescCreateUnitPrefab");
			m_pCreateUnit = prefab->Instantiate();
			m_pCreateUnit->Transform()->SetRelativePos(0.f, 0.f, -600.f);
			Instantiate(m_pCreateUnit, 31);
		}

		if (!lstrcmp(_str.data(), L"CRangerPrefab"))
		{
			prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"CImageRangerPrefab");
		}
		else if (!lstrcmp(_str.data(), L"SoldierPrefab"))
		{
			prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"CImageSoliderPrefab");
		}
		else if (!lstrcmp(_str.data(), L"CSniperPrefab"))
		{
			prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"CImageSniperPrefab");
		}
		CGameObject* pObj = prefab->Instantiate();
		m_queUnit.push(pObj);
		Instantiate(pObj, 31);

		m_eBuildState = BUILD_STATE::CREATE_UNIT;
		m_strPrefab = _str;

		for (size_t i{}; i < m_queUnit.size(); ++i)
		{
			CGameObject* pObj = m_queUnit.front();
			m_queUnit.pop();
			Vec3 vScale = m_pCreateUnit->GetChilds()[i]->Transform()->GetRelativeScale();
			pObj->Transform()->SetRelativeScale(vScale);
			pObj->Transform()->SetRelativePos(60 * i, 0.f, -600.f);
			m_pCreateUnit->GetChilds()[i]->MeshRender()->Deactivate();
			m_queUnit.push(pObj);
		}

		size_t iSize = m_pCreateUnit->GetChilds().size() - m_queUnit.size();

		for (size_t i{ iSize }; i < m_pCreateUnit->GetChilds().size(); ++i)
			m_pCreateUnit->GetChilds()[i]->MeshRender()->Activate();
	}
}

void CSCScript::clear()
{
	if (-1 != m_iIndex)
	{
		RefreshTile(m_iIndex);
	}
}

void CSCScript::PhaseEventOn()
{
	__super::PhaseEventOn();

	CEngine::g_IconText.clear();

	lstrcpy(CEngine::g_szFullName, L"SoilderCenter");

	m_pPortrait->MeshRender()->Activate();

	//lstrcpy(g_szHp, to_wstring(m_fHP));

	if (BUILD_STATE::CREATE_UNIT == m_eBuildState)
	{
		m_pProgressBar->MeshRender()->Activate();
		m_pProgressBar->GetChilds()[0]->MeshRender()->Activate();

		for (size_t i{ 0 }; i < m_pCreateUnit->GetChilds().size(); ++i)
			m_pCreateUnit->GetChilds()[i]->MeshRender()->Activate();

		for (size_t i{}; i < m_queUnit.size(); ++i)
		{
			CGameObject* pObj = m_queUnit.front();
			m_queUnit.pop();
			Vec3 vScale = m_pCreateUnit->GetChilds()[i]->Transform()->GetRelativeScale();
			pObj->Transform()->SetRelativeScale(vScale);
			pObj->Transform()->SetRelativePos(60 * i, 0.f, -600.f);
			pObj->MeshRender()->Activate();
			m_pCreateUnit->GetChilds()[i]->MeshRender()->Deactivate();
			m_queUnit.push(pObj);
		}

	}
	else
	{
		wchar_t sz[200];

		for (size_t i{}; i < m_vecIcon.size(); ++i)
			m_vecIcon[i]->MeshRender()->Activate();

		lstrcpy(sz, to_wstring(m_fHP).c_str());
		lstrcat(sz, L"/");
		lstrcat(sz, to_wstring(m_fFullHp).c_str());

		lstrcpy(CEngine::g_szHp, sz);

		SetIconUI(m_iGold, 0);
		SetIconUI(m_iWorker, 1);
	}

	const vector<CGameObject*> vec = CInterfaceMgr::GetInst()->GetTapButtons();
	for (size_t i{}; i < 6; ++i)
		vec[i]->GetScript<CButtonScript>()->SetColumn((UINT)SOLDIER_CMD);

	GetOwner()->GetChilds()[2]->GetRenderComponent()->Activate();
}

void CSCScript::PhaseEventOff()
{
	__super::PhaseEventOff();

	CEngine::g_IconText.clear();

	if (BUILD_STATE::CREATE_UNIT == m_eBuildState)
	{
		m_pProgressBar->MeshRender()->Deactivate();
		m_pProgressBar->GetChilds()[0]->MeshRender()->Deactivate();

		for (size_t i{}; i < m_queUnit.size(); ++i)
		{
			CGameObject* pObj = m_queUnit.front();
			m_queUnit.pop();
			pObj->MeshRender()->Deactivate();
			m_queUnit.push(pObj);
		}

		for (size_t i{ 0 }; i < m_pCreateUnit->GetChilds().size(); ++i)
			m_pCreateUnit->GetChilds()[i]->MeshRender()->Deactivate();
	}

	m_pPortrait->MeshRender()->Deactivate();

	for (size_t i{}; i < m_vecIcon.size(); ++i)
		m_vecIcon[i]->MeshRender()->Deactivate();

	GetOwner()->GetChilds()[2]->GetRenderComponent()->Deactivate();
}

void CSCScript::CreateEnd()
{
	if (m_queUnit.empty())
	{
		for (size_t i{ 0 }; i < m_pCreateUnit->GetChilds().size(); ++i)
			m_pCreateUnit->GetChilds()[i]->MeshRender()->Deactivate();

		//if (this->GetOwner() == CInterfaceMgr::GetInst()->GetTarget())
		//{
		//	for (size_t i{}; i < m_vecIcon.size(); ++i)
		//		m_vecIcon[i]->MeshRender()->Activate();

		//	CEngine::g_IconText.clear();
		//	SetIconUI(m_iGold, 0);
		//	SetIconUI(m_iWorker, 1);
		//}

		m_pCreateUnit->Destroy();

		m_pProgressBar->MeshRender()->Deactivate();
		m_pProgressBar->GetChilds()[0]->MeshRender()->Deactivate();

		m_eBuildState = BUILD_STATE::COMPLETE;
	}
	else
	{
		m_eBuildState = BUILD_STATE::CREATE_UNIT;
	}

}

void CSCScript::RallyEvent()
{
	m_bRallyPoint = true;

	m_pRallyPoint->MeshRender()->Activate();
}