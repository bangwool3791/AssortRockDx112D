#include "pch.h"
#include "CCommandScript.h"

#include <Engine\CDevice.h>
#include <Engine\CLevel.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CTransform.h>

#include <Engine\CInterfaceMgr.h>
#include <Script\CMouseScript.h>

#include <Engine\CJpsMgr.h>

CCommandScript::CCommandScript()
	:CScript{ SCRIPT_TYPE::COMMANDSCRIPT }
	, m_vMousePos{}
	, m_pTileObject{}
	, m_eBuildState{BUILD_STATE::COMPLETE}
{
	SetName(L"CCommandScript");
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
}

void CCommandScript::tick()
{
	if (0 >= m_iHp)
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

