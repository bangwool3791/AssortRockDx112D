#include "pch.h"
#include "CSCScript.h"

#include <Engine\CDevice.h>
#include <Engine\CLevel.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CTransform.h>

#include <Engine\CInterfaceMgr.h>
#include <Script\CMouseScript.h>

CSCScript::CSCScript()
	:CScript{ SCRIPT_TYPE::SCSCRIPT }
	, m_vMousePos{}
	, m_pTileObject{}
	, m_eBuildState{ BUILD_STATE::READY }
	, m_pGameObject{}
{
	SetName(L"CSCScript");
}

CSCScript::~CSCScript()
{
}

void CSCScript::begin()
{
	m_pLevelMouseObject = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"MouseObject");
	m_pTileObject = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"LevelTile");

	GetOwner()->GetRenderComponent()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"BuildMtrl"));
	GetOwner()->GetRenderComponent()->SetInstancingType(INSTANCING_TYPE::NONE);
	//GetOwner()->GetRenderComponent()->GetCurMaterial()->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Mask\\buildmask.png"));

	m_pTileObject->TileMap()->On();
}

void CSCScript::tick()
{

}

void CSCScript::finaltick()
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

			m_vMousePos = m_pTileObject->GetRenderComponent()->GetMesh()->GetPosition(ray);

			tTile tTile = m_pTileObject->TileMap()->GetInfo(m_vMousePos);

			if (-1 != m_iIndex)
			{
				RefreshTile(m_iIndex);
			}

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
				m_pTileObject->TileMap()->Off();
				m_eBuildState = BUILD_STATE::BUILD;

				Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"SCPrefab");
				CGameObject* pObj = pUIPrefab->Instantiate();
				CInterfaceMgr::GetInst()->SetBuildObj(pObj);
				Instantiate(pObj, m_vMousePos, 0);
			}
			m_fDt2 = 0.5f;
		}
	}
	else if (m_eBuildState == BUILD_STATE::BUILD)
	{
		if (m_fDt > 5.f)
		{
			GetOwner()->GetRenderComponent()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ObjectMtrl"));
			GetOwner()->GetRenderComponent()->SetInstancingType(INSTANCING_TYPE::USED);
			m_eBuildState = BUILD_STATE::COMPLETE;
			m_fDt = 0.f;
		}
	}
	else if (m_eBuildState == BUILD_STATE::COMPLETE)
	{
		if (KEY_PRESSED(KEY::LBTN))
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
			Vec3 vPos{};

			if (GetOwner()->Transform()->Picking(ray, vPos))
			{
				CInterfaceMgr::GetInst()->SetTarget(GetOwner());
			}
		}
	}
	else if (m_eBuildState == BUILD_STATE::CREATE_UNIT)
	{
		if (m_fDt >= 10.f)
		{
			Ptr<CPrefab> prefab = CResMgr::GetInst()->FindRes<CPrefab>(m_strPrefab);
			CGameObject* pUnit = prefab->Instantiate();
			Vec3 vPos = Transform()->GetRelativePos();
			vPos.z -= (Transform()->GetRelativeScale().z + pUnit->Transform()->GetRelativePos().z) * 0.5f;

			Instantiate(pUnit, vPos, 1);

			m_strPrefab.clear();
			m_strPrefab.shrink_to_fit();

			m_eBuildState = BUILD_STATE::COMPLETE;
			m_fDt = 0.f;

			m_pCircleArrow->Destroy();
			m_pCircleArrow = nullptr;
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
	if (BUILD_STATE::COMPLETE == m_eBuildState && m_strPrefab.empty())
	{
		CGameObject* pButton{};

		if (!lstrcmp(_str.data(), L"CRangerPrefab"))
		{
			pButton = CInterfaceMgr::GetInst()->GetBtn(0);
		}
		else if (!lstrcmp(_str.data(), L"SoldierPrefab"))
		{
			pButton = CInterfaceMgr::GetInst()->GetBtn(1);
		}
		else if (!lstrcmp(_str.data(), L"CSniperPrefab"))
		{
			pButton = CInterfaceMgr::GetInst()->GetBtn(2);
		}
		
		Ptr<CPrefab> pCircleArrowPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"CCircleArrowPrefab");

		CGameObject* pParent = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(31)->FindParent(L"Interface");

		assert(pParent);

		m_pCircleArrow = pCircleArrowPrefab->Instantiate();

		m_pCircleArrow->Transform()->SetRelativePos(pButton->Transform()->GetRelativePos());

		Instantiate(m_pCircleArrow, pParent, 31);

		m_eBuildState = BUILD_STATE::CREATE_UNIT;
		m_strPrefab = _str;
		m_fDt = 0.f;
	}
}