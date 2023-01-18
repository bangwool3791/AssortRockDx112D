#include "pch.h"
#include "CWWallScript.h"

#include <Engine\CDevice.h>
#include <Engine\CLevel.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CTransform.h>

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
}

CWWallScript::~CWWallScript()
{
}

void CWWallScript::begin()
{
	m_pLevelMouseObject = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"MouseObject");
	m_pTileObject = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"LevelTile");

	GetOwner()->GetRenderComponent()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"WWallMesh"));
	GetOwner()->GetRenderComponent()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"BuildMtrl"));
	GetOwner()->GetRenderComponent()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\buildings\\Atlas1_LQ.dds"));
	GetOwner()->GetRenderComponent()->SetInstancingType(INSTANCING_TYPE::NONE);

	m_pTileObject->TileMap()->On();
}

void CWWallScript::tick()
{

}

void CWWallScript::finaltick()
{

	m_fDt += DT;
	m_fDt2 += DT;

	if (BUILD_STATE::READY == m_eBuildState)
	{
		if (m_fDt > 0.15f)
		{
			Vec2 p = CKeyMgr::GetInst()->GetMousePos();
			Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();

			p.x = (2.0f * p.x) / vResolution.x - 1.0f;
			p.y = 1.0f - (2.0f * p.y) / vResolution.y;

			XMVECTOR det; //Determinant, needed for matrix inverse function call
			Vector3 origin = Vector3(p.x, p.y, 0);
			Vector3 faraway = Vector3(p.x, p.y, 1);

			XMMATRIX invViewProj = XMMatrixInverse(&det, g_transform.matView * g_transform.matProj);
			Vector3 rayorigin = XMVector3Transform(origin, invViewProj);
			Vector3 rayend = XMVector3Transform(faraway, invViewProj);
			Vector3 raydirection = rayend - rayorigin;
			raydirection.Normalize();
			Ray ray;
			ray.position = rayorigin;
			ray.direction = raydirection;

			Vec3 vPos = m_pTileObject->GetRenderComponent()->GetMesh()->GetPosition(ray);

			tTile tTile = m_pTileObject->TileMap()->GetInfo(vPos);

			if (-1 != m_iIndex)
			{
				SetTileInfo(m_iIndex);
			}

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

		if (m_fDt2 > 0.5f)
		{
			if (KEY_PRESSED(KEY::LBTN) && !IsBlocked(m_iIndex))
			{
				Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"WoodWallPrefab");
				Instantiate(pUIPrefab->Instantiate(), 1);
				m_pTileObject->TileMap()->SetInfo(m_iIndex, (UINT)TILE_TYPE::USED);
				m_pTileObject->TileMap()->Off();
				m_eBuildState = BUILD_STATE::BUILD;
				m_fDt = 0.f;
				m_fDt2 = 0.f;

				cout << "m_iIndex " << m_iIndex << endl;
				m_arr[m_iIndex].bChecked = true;

				ChildWallProcess();
			}
			m_fDt2 = 0.5f;
		}
	}
	else if (m_eBuildState == BUILD_STATE::BUILD)
	{
		if (m_fDt > 5.f)
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