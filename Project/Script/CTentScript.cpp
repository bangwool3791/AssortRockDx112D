#include "pch.h"
#include "CTentScript.h"

#include <Engine\CDevice.h>
#include <Engine\CLevel.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CTransform.h>

#include <Engine\CInterfaceMgr.h>
#include <Script\CMouseScript.h>

CTentScript::CTentScript()
	:CScript{ SCRIPT_TYPE::TENTSCRIPT }
	, m_vMousePos{}
	, m_pTileObject{}
	, m_eBuildState{ BUILD_STATE::READY }
{
	SetName(L"CTentScript");
}

CTentScript::~CTentScript()
{
}


void CTentScript::begin()
{
	m_pLevelMouseObject = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"MouseObject");
	m_pTileObject = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"LevelTile");

	m_pTileObject->TileMap()->On();
}

void CTentScript::tick()
{

}

void CTentScript::finaltick()
{
	static float dt = 0;

	if (BUILD_STATE::READY == m_eBuildState)
	{
		if (++dt > 60)
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
				RefreshTile(m_iIndex);
			}

			SetTile(tTile.iIndex);

			tTile.vPos.z += TILECZ * 0.5f;
			m_iIndex = tTile.iIndex;
			GetOwner()->Transform()->SetRelativePos(tTile.vPos);
			dt -= 60;

			if (KEY_PRESSED(KEY::LBTN) && IsBlocked(m_iIndex))
			{
				SetTile(tTile.iIndex, 4);
				m_pTileObject->TileMap()->Off();
				m_eBuildState = BUILD_STATE::BUILD;
			}
		}
		//if (!Picking(vPos))
		//{
		//	tTile = GetOwner()->TileMap()->GetInfo(vPos);
		//}
		//static queue<UINT> que;
		//static queue<UINT> result;

		//if (-1 != m_iIndex)
		//{
		//	result.push(m_iIndex);
		//	SetTileInfo(que, result, 0);
		//	SetTileInfo(que, result, 0);
		//	SetTileInfo(que, result, 0);
		//	SetTileInfo(que, result, 0);
		//	SetTileInfo(que, result, 0);
		//	SetTileInfo(que, result, 0);
		//	SetTileInfo(que, result, 0);
		//	SetTileInfo(que, result, 0);
		//	SetTileInfo(que, result, 0);
		//	SetTileInfo(que, result, 0);
		//}

		//while (!result.empty())
		//	result.pop();

		//for (size_t i{}; i < 40000; ++i)
		//	m_bCheck[i] = false;

		//tTile ttile = m_pTileObject->TileMap()->GetInfo(m_pLevelMouseObject->Transform()->GetRelativePos());

		//result.push(ttile.iIndex);
		//SetTileInfo(que, result, 0);
		//SetTileInfo(que, result, 0);
		//SetTileInfo(que, result, 0);
		//SetTileInfo(que, result, 0);
		//SetTileInfo(que, result, 0);
		//SetTileInfo(que, result, 0);
		//SetTileInfo(que, result, 0);
		//SetTileInfo(que, result, 0);
		//SetTileInfo(que, result, 0);
		//SetTileInfo(que, result, 0);

		//while (!result.empty())
		//	result.pop();

		//for (size_t i{}; i < 40000; ++i)
		//	m_bCheck[i] = false;

		//m_iIndex = ttile.iIndex;

		//GetOwner()->Transform()->SetRelativePos(ttile.vPos);
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

	if (1 == tTile.iInfo)
	{
		m_pTileObject->TileMap()->SetInfo(_iTile, 2);
	}
	else if (2 == tTile.iInfo)
	{
		m_pTileObject->TileMap()->SetInfo(_iTile, 1);
	}
	else if (4 == tTile.iInfo)
	{
		m_pTileObject->TileMap()->SetInfo(_iTile, 3);
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

	if (2 == tTile.iInfo)
	{
		m_pTileObject->TileMap()->SetInfo(_iTile, 1);
	}
	else if (3 == tTile.iInfo)
	{
		m_pTileObject->TileMap()->SetInfo(_iTile, 4);
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
		if (4 == vec[i].iInfo)
			return false;
	}

	return true;
}