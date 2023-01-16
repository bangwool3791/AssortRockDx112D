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

	GetOwner()->GetRenderComponent()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"BuildMtrl"));
	GetOwner()->GetRenderComponent()->SetInstancingType(INSTANCING_TYPE::NONE);
	//GetOwner()->GetRenderComponent()->GetCurMaterial()->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Mask\\buildmask.png"));

	int random = rand();

	if (random % 4 == 0)
		GetOwner()->Animator2D()->Play(L"Right1", false);
	else if (random % 4 == 1)
		GetOwner()->Animator2D()->Play(L"Right2", false);
	else if (random % 4 == 2)
		GetOwner()->Animator2D()->Play(L"Left1", false);
	else if (random % 4 == 3)
		GetOwner()->Animator2D()->Play(L"Left2", false);

	m_pTileObject->TileMap()->On();
}

void CTentScript::tick()
{

}

void CTentScript::finaltick()
{
	static float dt = DT;
	static float dt2 = DT;

	dt += DT;
	dt2 += DT;

	if (BUILD_STATE::READY == m_eBuildState)
	{
		if (dt > 0.25f)
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
			dt -= 0.25f;
		}

		if (dt2 > 0.5f)
		{
			if (KEY_PRESSED(KEY::LBTN) && IsBlocked(m_iIndex))
			{
				SetTile(m_iIndex, 4);
				m_pTileObject->TileMap()->Off();
				m_eBuildState = BUILD_STATE::BUILD;
			}
			dt2 = 0.5f;
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
		if ((UINT)TILE_TYPE::NOTUSED == vec[i].iInfo)
			return false;
	}

	for (size_t i{}; i < vec.size(); ++i)
	{
		if ((UINT)TILE_TYPE::COLLISION == vec[i].iInfo)
			return false;
	}

	return true;
}