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
	SetName(L"CQuarryScript");
}

CQuarryScript::~CQuarryScript()
{
}

void CQuarryScript::begin()
{
	m_pTileObject = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"LevelTile");

	GetOwner()->GetRenderComponent()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"BuildMtrl"));
	GetOwner()->GetRenderComponent()->SetInstancingType(INSTANCING_TYPE::NONE);


	m_pTileObject->TileMap()->On();
}

void CQuarryScript::tick()
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

			if (m_iIndex != tTile.iIndex)
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

				m_pTileObject->TileMap()->Off();
				m_eBuildState = BUILD_STATE::BUILD;
				m_fDt = 0.f;
				m_fDt2 = 0.f;
			}
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
			m_pTileObject->TileMap()->SetInfo(data, _value);
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
