#include "pch.h"
#include "CWWallScript.h"

#include <Engine\CDevice.h>
#include <Engine\CLevel.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CTransform.h>

#include <Engine\CJpsMgr.h>
#include <Engine\CInterfaceMgr.h>
#include <Script\CMouseScript.h>

array<tWoodWall, 200 * 200> CWWallScript::m_arr{};

CWWallScript::CWWallScript()
	:CScript{ SCRIPT_TYPE::WWALLSCRIPT}
	, m_vMousePos{}
	, m_pTileObject{}
	, m_eBuildState{ BUILD_STATE::READY }
{

	SetName(L"CWWallScript");

	for (size_t i{}; i < 200 * 200; ++i)
	{
		m_arr[i].bChecked = false;

		for (size_t j{}; j < 8; ++j)
		{
			m_arr[i].arr[j] = false;
		}
	}

	m_iGoldOut = 10;
	m_iWoodOut = 3;
	m_iIronOut = 0;

	m_fFullHp = 400.f;

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

CWWallScript::~CWWallScript()
{
}

void CWWallScript::begin()
{
	__super::begin();

	m_pLevelMouseObject = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"MouseObject");
	m_pTileObject = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"LevelTile");

	GetOwner()->GetRenderComponent()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"WWallMesh"));
	GetOwner()->GetRenderComponent()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"BuildMtrl"));
	GetOwner()->GetRenderComponent()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\buildings\\Atlas1_LQ.dds"));
	GetOwner()->GetRenderComponent()->SetInstancingType(INSTANCING_TYPE::USED);

	GetOwner()->GetChilds()[0]->GetRenderComponent()->Deactivate();
}

void CWWallScript::tick()
{
	__super::tick();
}

void CWWallScript::finaltick()
{
	if (0 > m_fHP)
	{
		//CEffectWoodPrefab
		CGameObject* pObj = CResMgr::GetInst()->FindRes<CPrefab>(L"CEffectExplosionPrefab")->Instantiate();
		Instantiate(pObj, Transform()->GetRelativePos(), 3);
		GetOwner()->Destroy();

		for (size_t i{}; i < m_vecBlock.size(); ++i)
			CJpsMgr::GetInst()->ClearCollision(m_vecBlock[i].x, m_vecBlock[i].z);

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

			SetTileInfo(tTile.iIndex);

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

		if (KEY_PRESSED(KEY::LBTN) && !IsBlocked(m_iIndex))
		{

			Int32 x = m_iIndex % TILEX;
			Int32 z = m_iIndex / TILEZ;

			m_vecBlock.push_back(tBlock{ x, z });
			CJpsMgr::GetInst()->SetCollision(x, z);

			m_pTileObject->TileMap()->SetInfo(m_iIndex, (UINT)TILE_TYPE::USED);
			m_eBuildState = BUILD_STATE::BUILD;
			m_fDt = 0.f;
			m_fDt2 = 0.f;

			m_arr[m_iIndex].bChecked = true;

			Create(L"WoodWallPrefab", m_vMousePos);

			ChildWallProcess();
		}
	}
	else if (m_eBuildState == BUILD_STATE::BUILD)
	{
		m_fHP += DT * 30.f;

		if (m_fHP > m_fFullHp)
		{
			GetOwner()->GetRenderComponent()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ObjectMtrl"));
			GetOwner()->GetRenderComponent()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\buildings\\Atlas1_LQ.dds"));
			GetOwner()->GetRenderComponent()->SetInstancingType(INSTANCING_TYPE::USED);
			m_eBuildState = BUILD_STATE::COMPLETE;
			m_fDt = 0.f;
			
			//ChildWallProcess();
		}
	}
	else if (m_eBuildState == BUILD_STATE::COMPLETE)
	{
		//ChildWallProcess();
	}
}

void CWWallScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CWWallScript::Overlap(CCollider2D* _pOther)
{
}

void CWWallScript::EndOverlap(CCollider2D* _pOther)
{
}
void CWWallScript::SaveToFile(FILE* _File)
{
	CScript::SaveToFile(_File);
}

void CWWallScript::LoadFromFile(FILE* _File)
{
	CScript::LoadFromFile(_File);
}

void CWWallScript::SetTileInfo(UINT _iTile)
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
	else if ((UINT)TILE_TYPE::COLLISION == tTile.iInfo)
	{
		m_pTileObject->TileMap()->SetInfo(_iTile, (UINT)TILE_TYPE::USED);
	}
}

bool  CWWallScript::IsBlocked(UINT _iTile)
{
	static vector<tTile> vec(1, tTile{});

	vec[0] = m_pTileObject->TileMap()->GetInfo(_iTile);

	for (size_t i{}; i < vec.size(); ++i)
	{
		if ((UINT)TILE_TYPE::BUILD != vec[i].iInfo)
			return true;
	}
	return false;
}

bool  CWWallScript::Node(UINT _iTile, UINT _iIndex)
{
	const static pair<UINT, UINT> index[8] = 
	{
	 pair<UINT, UINT> {7,3}
	,pair<UINT, UINT> {1,5}
	,pair<UINT, UINT> {3,7}
	,pair<UINT, UINT> {5,1}
	,pair<UINT, UINT> {0,4}
	,pair<UINT, UINT> {2,6} 
	,pair<UINT, UINT> {4,0}
	,pair<UINT, UINT> {6,2}
	};

	UINT i = _iIndex - 1;
	if (m_arr[m_iIndex].bChecked && m_arr[_iTile].bChecked && !m_arr[_iTile].arr[index[i].first] && !m_arr[m_iIndex].arr[index[i].second])
	{
		m_arr[_iTile].arr[index[i].first] = true;
		m_arr[m_iIndex].arr[index[i].second] = true;
		return true;
	}
	return false;
}

void CWWallScript::ChildWallProcess()
{

	//even
	if ((m_iIndex / TILEX) % 2 == 0)
	{
		if (0 <= m_iIndex + TILEX - 1 && m_iIndex + TILEX - 1 < 100 * 100)
		{
			if (Node(m_iIndex + TILEX - 1, 1))
			{
				Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"WoodWallLeftSidePrefab");
				CGameObject* pObj{};
				Instantiate(pObj = pUIPrefab->Instantiate(), 1);

				tTile tTile1 = m_pTileObject->TileMap()->GetInfo(m_iIndex);
				tTile tTile2 = m_pTileObject->TileMap()->GetInfo(m_iIndex + TILEX - 1);
				Vec3 vPos = (tTile1.vPos + tTile2.vPos) * 0.5f;
				Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();
				pObj->Transform()->SetRelativePos(vPos);
				pObj->Transform()->SetRelativeScale(vScale);
				pObj->begin();
			}
		}
		if (0 <= m_iIndex + TILEX && m_iIndex + TILEX < 100 * 100)
		{
			if (Node(m_iIndex + TILEX, 2))
			{
				Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"WoodWallRightSidePrefab");
				CGameObject* pObj{};
				Instantiate(pObj = pUIPrefab->Instantiate(), 1);

				tTile tTile1 = m_pTileObject->TileMap()->GetInfo(m_iIndex);
				tTile tTile2 = m_pTileObject->TileMap()->GetInfo(m_iIndex + TILEX);
				Vec3 vPos = (tTile1.vPos + tTile2.vPos) * 0.5f;
				Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();
				pObj->Transform()->SetRelativePos(vPos);
				pObj->Transform()->SetRelativeScale(vScale);
				pObj->begin();
			}
		}
		if (0 <= m_iIndex - TILEX && m_iIndex - TILEX < 100 * 100)
		{
			if (Node(m_iIndex - TILEX, 3))
			{
				Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"WoodWallLeftSidePrefab");
				CGameObject* pObj{};
				Instantiate(pObj = pUIPrefab->Instantiate(), 1);

				tTile tTile1 = m_pTileObject->TileMap()->GetInfo(m_iIndex);
				tTile tTile2 = m_pTileObject->TileMap()->GetInfo(m_iIndex - TILEX);
				Vec3 vPos = (tTile1.vPos + tTile2.vPos) * 0.5f;
				Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();
				pObj->Transform()->SetRelativePos(vPos);
				pObj->Transform()->SetRelativeScale(vScale);
				pObj->begin();
			}
		}
		if (0 <= m_iIndex - TILEX - 1 && m_iIndex - TILEX - 1 < 100 * 100)
		{
			if (Node(m_iIndex - TILEX - 1, 4))
			{
				Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"WoodWallRightSidePrefab");
				CGameObject* pObj{};
				Instantiate(pObj = pUIPrefab->Instantiate(), 1);

				tTile tTile1 = m_pTileObject->TileMap()->GetInfo(m_iIndex);
				tTile tTile2 = m_pTileObject->TileMap()->GetInfo(m_iIndex - TILEX - 1);
				Vec3 vPos = (tTile1.vPos + tTile2.vPos) * 0.5f;
				Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();
				pObj->Transform()->SetRelativePos(vPos);
				pObj->Transform()->SetRelativeScale(vScale);
				pObj->begin();
			}
		}

		if (0 <= m_iIndex + TILEX * 2 && m_iIndex + TILEX * 2 < 100 * 100)
		{
			if (Node(m_iIndex + TILEX * 2, 5))
			{
				Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"WoodWallHightPrefab");
				CGameObject* pObj{};
				Instantiate(pObj = pUIPrefab->Instantiate(), 1);

				tTile tTile1 = m_pTileObject->TileMap()->GetInfo(m_iIndex);
				tTile tTile2 = m_pTileObject->TileMap()->GetInfo(m_iIndex + TILEX * 2);
				Vec3 vPos = (tTile1.vPos + tTile2.vPos) * 0.5f;
				Vec3 vScale = tTile1.vPos - tTile2.vPos;
				vScale.x = GetOwner()->Transform()->GetRelativeScale().x * 0.5f;
				vScale.y = abs(vScale.y);
				vScale.z = abs(vScale.z);
				pObj->Transform()->SetRelativePos(vPos);
				pObj->Transform()->SetRelativeScale(vScale);
				pObj->begin();
			}
		}
		if (0 <= m_iIndex + 1 && m_iIndex + 1 < 100 * 100)
		{
			if (Node(m_iIndex + 1, 6))
			{
				Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"WoodWallWidthPrefab");
				CGameObject* pObj{};
				Instantiate(pObj = pUIPrefab->Instantiate(), 1);

				tTile tTile1 = m_pTileObject->TileMap()->GetInfo(m_iIndex);
				tTile tTile2 = m_pTileObject->TileMap()->GetInfo(m_iIndex + 1);
				Vec3 vPos = (tTile1.vPos + tTile2.vPos) * 0.5f;
				vPos.z -= Transform()->GetRelativeScale().z * 0.25f;
				Vec3 vScale = tTile1.vPos - tTile2.vPos;
				vScale.x = abs(vScale.x) - GetOwner()->Transform()->GetWorldScale().x;
				vScale.y = GetOwner()->Transform()->GetRelativeScale().y;
				vScale.z = GetOwner()->Transform()->GetRelativeScale().z * 0.5f;

				pObj->Transform()->SetRelativePos(vPos);
				pObj->Transform()->SetRelativeScale(vScale);
				pObj->begin();
			}
		}
		if (0 <= m_iIndex - TILEX * 2 && m_iIndex - TILEX * 2 < 100 * 100)
		{
			if (Node(m_iIndex - TILEX * 2, 7))
			{
				Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"WoodWallHightPrefab");
				CGameObject* pObj{};
				Instantiate(pObj = pUIPrefab->Instantiate(), 1);

				tTile tTile1 = m_pTileObject->TileMap()->GetInfo(m_iIndex);
				tTile tTile2 = m_pTileObject->TileMap()->GetInfo(m_iIndex - TILEX * 2);
				Vec3 vPos = (tTile1.vPos + tTile2.vPos) * 0.5f;
				Vec3 vScale = tTile1.vPos - tTile2.vPos;
				vScale.x = GetOwner()->Transform()->GetRelativeScale().x * 0.5f;
				vScale.y = abs(vScale.y);
				vScale.z = abs(vScale.z);
				pObj->Transform()->SetRelativePos(vPos);
				pObj->Transform()->SetRelativeScale(vScale);
				pObj->begin();
			}
		}
		if (0 <= m_iIndex - 1 && m_iIndex - 1 < 100 * 100)
		{
			if (Node(m_iIndex - 1, 8))
			{
				Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"WoodWallWidthPrefab");
				CGameObject* pObj{};
				Instantiate(pObj = pUIPrefab->Instantiate(), 1);

				tTile tTile1 = m_pTileObject->TileMap()->GetInfo(m_iIndex);
				tTile tTile2 = m_pTileObject->TileMap()->GetInfo(m_iIndex - 1);
				Vec3 vPos = (tTile1.vPos + tTile2.vPos) * 0.5f;
				vPos.z -= Transform()->GetRelativeScale().z * 0.25f;
				Vec3 vScale = tTile1.vPos - tTile2.vPos;
				vScale.x = abs(vScale.x) - GetOwner()->Transform()->GetWorldScale().x;
				vScale.y = GetOwner()->Transform()->GetRelativeScale().y;
				vScale.z = GetOwner()->Transform()->GetRelativeScale().z * 0.5f;
				pObj->Transform()->SetRelativePos(vPos);
				pObj->Transform()->SetRelativeScale(vScale);
				pObj->begin();
			}
		}
	}
	//odd
	else if ((m_iIndex / TILEX) % 2 == 1)
	{
		if (0 <= m_iIndex + TILEX && m_iIndex + TILEX < 100 * 100)
		{
			if (Node(m_iIndex + TILEX, 1))
			{
				Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"WoodWallLeftSidePrefab");
				CGameObject* pObj{};
				Instantiate(pObj = pUIPrefab->Instantiate(), 1);

				tTile tTile1 = m_pTileObject->TileMap()->GetInfo(m_iIndex);
				tTile tTile2 = m_pTileObject->TileMap()->GetInfo(m_iIndex + TILEX);
				Vec3 vPos = (tTile1.vPos + tTile2.vPos) * 0.5f;
				Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();
				pObj->Transform()->SetRelativePos(vPos);
				pObj->Transform()->SetRelativeScale(vScale);
				pObj->begin();
			}
		}
		if (0 <= m_iIndex + TILEX + 1 && m_iIndex + TILEX + 1 < 100 * 100)
		{
			if (Node(m_iIndex + TILEX + 1, 2))
			{
				Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"WoodWallRightSidePrefab");
				CGameObject* pObj{};
				Instantiate(pObj = pUIPrefab->Instantiate(), 1);

				tTile tTile1 = m_pTileObject->TileMap()->GetInfo(m_iIndex);
				tTile tTile2 = m_pTileObject->TileMap()->GetInfo(m_iIndex + TILEX + 1);
				Vec3 vPos = (tTile1.vPos + tTile2.vPos) * 0.5f;
				Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();
				pObj->Transform()->SetRelativePos(vPos);
				pObj->Transform()->SetRelativeScale(vScale);
				pObj->begin();
			}
		}
		if (0 <= m_iIndex - TILEX + 1 && m_iIndex - TILEX + 1 < 100 * 100)
		{
			if (Node(m_iIndex - TILEX + 1, 3))
			{
				Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"WoodWallLeftSidePrefab");
				CGameObject* pObj{};
				Instantiate(pObj = pUIPrefab->Instantiate(), 1);

				tTile tTile1 = m_pTileObject->TileMap()->GetInfo(m_iIndex);
				tTile tTile2 = m_pTileObject->TileMap()->GetInfo(m_iIndex - TILEX + 1);
				Vec3 vPos = (tTile1.vPos + tTile2.vPos) * 0.5f;
				Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();
				pObj->Transform()->SetRelativePos(vPos);
				pObj->Transform()->SetRelativeScale(vScale);
				pObj->begin();
			}
		}
		if (0 <= m_iIndex - TILEX && m_iIndex - TILEX < 100 * 100)
		{
			if (Node(m_iIndex - TILEX, 4))
			{
				Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"WoodWallRightSidePrefab");
				CGameObject* pObj{};
				Instantiate(pObj = pUIPrefab->Instantiate(), 1);

				tTile tTile1 = m_pTileObject->TileMap()->GetInfo(m_iIndex);
				tTile tTile2 = m_pTileObject->TileMap()->GetInfo(m_iIndex - TILEX);
				Vec3 vPos = (tTile1.vPos + tTile2.vPos) * 0.5f;
				Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();
				pObj->Transform()->SetRelativePos(vPos);
				pObj->Transform()->SetRelativeScale(vScale);
				pObj->begin();
			}
		}

		if (0 <= m_iIndex + TILEX * 2 && m_iIndex + TILEX * 2 < 100 * 100)
		{
			if (Node(m_iIndex + TILEX * 2, 5))
			{
				Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"WoodWallHightPrefab");
				CGameObject* pObj{};
				Instantiate(pObj = pUIPrefab->Instantiate(), 1);

				tTile tTile1 = m_pTileObject->TileMap()->GetInfo(m_iIndex);
				tTile tTile2 = m_pTileObject->TileMap()->GetInfo(m_iIndex + TILEX * 2);
				Vec3 vPos = (tTile1.vPos + tTile2.vPos) * 0.5f;
				Vec3 vScale = tTile1.vPos - tTile2.vPos;
				vScale.x = GetOwner()->Transform()->GetRelativeScale().x * 0.5f;
				vScale.y = abs(vScale.y);
				vScale.z = abs(vScale.z);
				pObj->Transform()->SetRelativePos(vPos);
				pObj->Transform()->SetRelativeScale(vScale);
				pObj->begin();
			}
		}

		if (0 <= m_iIndex + 1 && m_iIndex + 1 < 100 * 100)
		{
			if (Node(m_iIndex + 1, 6))
			{
				Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"WoodWallWidthPrefab");
				CGameObject* pObj{};
				Instantiate(pObj = pUIPrefab->Instantiate(), 1);

				tTile tTile1 = m_pTileObject->TileMap()->GetInfo(m_iIndex);
				tTile tTile2 = m_pTileObject->TileMap()->GetInfo(m_iIndex + 1);
				Vec3 vPos = (tTile1.vPos + tTile2.vPos) * 0.5f;
				vPos.z -= Transform()->GetRelativeScale().z * 0.25f;
				Vec3 vScale = tTile1.vPos - tTile2.vPos;
				vScale.x = abs(vScale.x) - GetOwner()->Transform()->GetWorldScale().x;
				vScale.y = GetOwner()->Transform()->GetRelativeScale().y;
				vScale.z = GetOwner()->Transform()->GetRelativeScale().z * 0.5f;
				pObj->Transform()->SetRelativePos(vPos);
				pObj->Transform()->SetRelativeScale(vScale);
				pObj->begin();
			}
		}
		if (0 <= m_iIndex - TILEX * 2 && m_iIndex - TILEX * 2 < 100 * 100)
		{
			if (Node(m_iIndex - TILEX * 2, 7))
			{
				Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"WoodWallHightPrefab");
				CGameObject* pObj{};
				Instantiate(pObj = pUIPrefab->Instantiate(), 1);

				tTile tTile1 = m_pTileObject->TileMap()->GetInfo(m_iIndex);
				tTile tTile2 = m_pTileObject->TileMap()->GetInfo(m_iIndex - TILEX * 2);
				Vec3 vPos = (tTile1.vPos + tTile2.vPos) * 0.5f;
				Vec3 vScale = tTile1.vPos - tTile2.vPos;
				vScale.x = GetOwner()->Transform()->GetRelativeScale().x * 0.5f;
				vScale.y = abs(vScale.y);
				vScale.z = abs(vScale.z);
				pObj->Transform()->SetRelativePos(vPos);
				pObj->Transform()->SetRelativeScale(vScale);
				pObj->begin();
			}
		}
		if (0 <= m_iIndex - 1 && m_iIndex - 1 < 100 * 100)
		{
			if (Node(m_iIndex - 1, 8))
			{
				Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"WoodWallWidthPrefab");
				CGameObject* pObj{};
				Instantiate(pObj = pUIPrefab->Instantiate(), 1);

				tTile tTile1 = m_pTileObject->TileMap()->GetInfo(m_iIndex);
				tTile tTile2 = m_pTileObject->TileMap()->GetInfo(m_iIndex - 1);
				Vec3 vPos = (tTile1.vPos + tTile2.vPos) * 0.5f;
				vPos.z -= Transform()->GetRelativeScale().z * 0.25f;
				Vec3 vScale = tTile1.vPos - tTile2.vPos;
				vScale.x = abs(vScale.x) - GetOwner()->Transform()->GetWorldScale().x;
				vScale.y = GetOwner()->Transform()->GetRelativeScale().y;
				vScale.z = GetOwner()->Transform()->GetRelativeScale().z * 0.5f;
				pObj->Transform()->SetRelativePos(vPos);
				pObj->Transform()->SetRelativeScale(vScale);
				pObj->begin();
			}
		}
	}
}

void CWWallScript::clear()
{
	if (-1 != m_iIndex)
	{
		SetTileInfo(m_iIndex);
	}
}


#include <Engine\CEngine.h>
#include <Engine\CFontMgr.h>
#include "CButtonScript.h"

void CWWallScript::PhaseEventOn()
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

	GetOwner()->GetChilds()[0]->GetRenderComponent()->Activate();
}

void CWWallScript::PhaseEventOff()
{
	__super::PhaseEventOff();

	m_pPortrait->MeshRender()->Deactivate();

	for (size_t i{}; i < m_vecIcon.size(); ++i)
		m_vecIcon[i]->MeshRender()->Deactivate();

	GetOwner()->GetChilds()[0]->GetRenderComponent()->Deactivate();
}
