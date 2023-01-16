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
}

void CQuarryScript::tick()
{
	m_fDt += DT;
	m_fDt2 += DT;

	if (BUILD_STATE::READY == m_eBuildState)
	{
		if (m_fDt > 0.25f)
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
						m_result.push(m_iIndex);
						m_result.push(m_iIndex + TILEX);
						m_result.push(m_iIndex + TILEX - 1);
						m_result.push(m_iIndex + TILEX * 2);
					}
					else if ((m_iIndex / TILEX) % 2 == 1)
					{
						m_result.push(m_iIndex);
						m_result.push(m_iIndex + TILEX);
						m_result.push(m_iIndex + TILEX + 1);
						m_result.push(m_iIndex + TILEX * 2);
					}

					SetTileInfo(m_queue, m_result, (UINT)TILE_TYPE::CRYSTAL);
					SetTileInfo(m_queue, m_result, (UINT)TILE_TYPE::CRYSTAL);

					while (!m_result.empty())
						m_result.pop();

					for (size_t i{}; i < 40000; ++i)
						m_bCheck[i] = false;
				}

				if ((tTile.iIndex / TILEX) % 2 == 0)
				{
					m_result.push(tTile.iIndex);
					m_result.push(tTile.iIndex + TILEX);
					m_result.push(tTile.iIndex + TILEX - 1);
					m_result.push(tTile.iIndex + TILEX * 2);
				}
				else if ((tTile.iIndex / TILEX) % 2 == 1)
				{
					m_result.push(tTile.iIndex);
					m_result.push(tTile.iIndex + TILEX);
					m_result.push(tTile.iIndex + TILEX + 1);
					m_result.push(tTile.iIndex + TILEX * 2);
				}

				SetTileInfo(m_queue, m_result, (UINT)TILE_TYPE::BEFORE_CRYSTAL);
				SetTileInfo(m_queue, m_result, (UINT)TILE_TYPE::BEFORE_CRYSTAL);

				while (!m_result.empty())
					m_result.pop();

				for (size_t i{}; i < 40000; ++i)
					m_bCheck[i] = false;

				m_iIndex = tTile.iIndex;

				GetOwner()->Transform()->SetRelativePos(tTile.vPos);
				m_fDt = 0.f;
			}
		}

		if (m_fDt2 > 0.5f)
		{
			if (KEY_PRESSED(KEY::LBTN) && IsBlocked(m_iIndex))
			{
				m_pTileObject->TileMap()->Off();
				m_eBuildState = BUILD_STATE::BUILD;
				m_fDt2 = 0.f;
			}
		}
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

void CQuarryScript::SetTileInfo(queue<UINT>& que, queue<UINT>& result, UINT _value)
{
	que = result;

	while (!result.empty())
		result.pop();

	while (!que.empty())
	{
		UINT data = que.front();
		que.pop();

		tTile tTile = m_pTileObject->TileMap()->GetInfo(data);

		if ((UINT)TILE_TYPE::CRYSTAL == tTile.iInfo && (UINT)TILE_TYPE::BEFORE_CRYSTAL == _value)
			m_pTileObject->TileMap()->SetInfo(data, _value);
		else if ((UINT)TILE_TYPE::BEFORE_CRYSTAL == tTile.iInfo && (UINT)TILE_TYPE::CRYSTAL == _value)
			m_pTileObject->TileMap()->SetInfo(data, _value);

		m_bCheck[data] = true;

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

bool  CQuarryScript::IsBlocked(UINT _iTile)
{
	tTile tTile{};
	tTile = m_pTileObject->TileMap()->GetInfo(_iTile);

	if ((UINT)TILE_TYPE::NOTUSED == tTile.iInfo)
		return false;

	if ((UINT)TILE_TYPE::COLLISION == tTile.iInfo)
		return false;

	return true;
}
