#include "pch.h"
#include "CWWSScript.h"

#include <Engine\CDevice.h>
#include <Engine\CLevel.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CTransform.h>

#include <Engine\CInterfaceMgr.h>
#include <Script\CMouseScript.h>

CWWSScript::CWWSScript()
	:CScript{ SCRIPT_TYPE::WWSSCRIPT }
	, m_vMousePos{}
	, m_pTileObject{}
	, m_eBuildState{ BUILD_STATE::READY }
{
	SetName(L"CWWSScript");
}

CWWSScript::~CWWSScript()
{
}

void CWWSScript::begin()
{
	m_pLevelMouseObject = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"MouseObject");
	m_pTileObject = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"LevelTile");

	GetOwner()->GetRenderComponent()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"BuildMtrl"));
	GetOwner()->GetRenderComponent()->SetInstancingType(INSTANCING_TYPE::NONE);
	//GetOwner()->GetRenderComponent()->GetCurMaterial()->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Mask\\buildmask.png"));
	m_pTileObject->TileMap()->On();
}

void CWWSScript::tick()
{

}

void CWWSScript::finaltick()
{
	m_fDt += DT;

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
					m_result.push_back(m_iIndex);
					SetTileInfo(m_vec, m_result, (UINT)TILE_TYPE::EMPTY);
					SetTileInfo(m_vec, m_result, (UINT)TILE_TYPE::EMPTY);

					m_result.clear();

					for (size_t i{}; i < 40000; ++i)
						m_bCheck[i] = false;
				}
			}
			m_result.push_back(tTile.iIndex);
			SetTileInfo(m_vec, m_result, (UINT)TILE_TYPE::BUILD);
			SetTileInfo(m_vec, m_result, (UINT)TILE_TYPE::BUILD);

			m_result.clear();

			for (size_t i{}; i < 40000; ++i)
				m_bCheck[i] = false;

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

			if (KEY_PRESSED(KEY::LBTN) && !IsBlocked(m_iIndex))
			{
				m_result.push_back(tTile.iIndex);
				SetTileInfo(m_vec, m_result, (UINT)TILE_TYPE::USED);
				SetTileInfo(m_vec, m_result, (UINT)TILE_TYPE::USED);

				m_pTileObject->TileMap()->Off();
				m_eBuildState = BUILD_STATE::BUILD;
				m_fDt = 0.f;
			}

			m_vectoken.clear();
			m_fDt -= 0.15f;
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

void CWWSScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CWWSScript::Overlap(CCollider2D* _pOther)
{
}

void CWWSScript::EndOverlap(CCollider2D* _pOther)
{
}
void CWWSScript::SaveToFile(FILE* _File)
{
	CScript::SaveToFile(_File);
}

void CWWSScript::LoadFromFile(FILE* _File)
{
	CScript::LoadFromFile(_File);
}

bool  CWWSScript::IsBlocked(UINT _iTile)
{
	for (size_t i{}; i < m_vectoken.size(); ++i)
	{
		if ((UINT)(TILE_TYPE::NOTUSED) == m_vectoken[i].iInfo)
			return true;
	}
	return false;
}

void CWWSScript::SetTileInfo(vector<UINT>& que, vector<UINT>& result, UINT _value)
{
	que = result;

	result.clear();

	for (auto iter{ que.begin() }; iter != que.end();)
	{
		UINT data = que.front();
		iter = que.erase(iter);

		tTile tTile = m_pTileObject->TileMap()->GetInfo(data);

		if((UINT)TILE_TYPE::BUILD == _value)
			m_vectoken.push_back(tTile);

		if ((UINT)TILE_TYPE::BUILD == tTile.iInfo && (UINT)TILE_TYPE::EMPTY == _value)
			m_pTileObject->TileMap()->SetInfo(data, _value);
		else if ((UINT)TILE_TYPE::EMPTY == tTile.iInfo && (UINT)TILE_TYPE::BUILD == _value)
			m_pTileObject->TileMap()->SetInfo(data, _value);
		else if ((UINT)TILE_TYPE::BUILD == tTile.iInfo && (UINT)TILE_TYPE::USED == _value)
			m_pTileObject->TileMap()->SetInfo(data, _value);

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
