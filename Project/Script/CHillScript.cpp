#include "pch.h"
#include "CHillScript.h"

#include <Engine\CDevice.h>
#include <Engine\CLevel.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CTransform.h>

#include <Engine\CInterfaceMgr.h>
#include <Script\CMouseScript.h>
#include <Engine\CJpsMgr.h>

CHillScript::CHillScript()
	:CScript{ SCRIPT_TYPE::HILLSCRIPT }
	, m_vMousePos{}
	, m_pTileObject{}
{
	SetName(L"CHillScript");
}

CHillScript::~CHillScript()
{
}

void CHillScript::begin()
{
	m_pTileObject = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"LevelTile");

	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();
	float fLength{};
	float fTileSize{};
	if (vScale.x > vScale.z)
	{
		fTileSize = TILECX;
		fLength = vScale.x * 0.5f;
	}
	else
	{
		fTileSize = TILECZ;
		fLength = vScale.z * 0.5f;
	}
	tTile tTile = m_pTileObject->TileMap()->GetInfo(vPos);

	GetOwner()->Transform()->SetRelativePos(tTile.vPos);

	queue<UINT> que;
	queue<UINT> result;

	result.push(tTile.iIndex);

	for (float i = 0; i <= fLength; i += fTileSize)
	{
		SetTileInfo(que, result, (UINT)TILE_TYPE::USED);
	}
}

void CHillScript::tick()
{

}

void CHillScript::finaltick()
{
}

void CHillScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CHillScript::Overlap(CCollider2D* _pOther)
{
	//Vec2 vRelativePos = GetOwner()->Collider2D()->GetFinalPos() - _pOther->GetFinalPos();
	//Vec2 vScale = (GetOwner()->Collider2D()->GetScale() + _pOther->GetScale()) * 0.5f;
	//Vec3 vDiff{};
	//vDiff.x = fabsf(vScale.x - fabsf(vRelativePos.x));
	//vDiff.z = fabsf(vScale.y - fabsf(vRelativePos.y));
	//Vec3 vDir = { vRelativePos.x , 0.f, vRelativePos.y };
	//vDir = vDir.Normalize();

	//Vec3 vPos = Transform()->GetRelativePos();

	//Transform()->SetRelativePos(vPos + vDiff * vDir * DT * 8);
}

void CHillScript::EndOverlap(CCollider2D* _pOther)
{
}

void CHillScript::SetTileInfo(queue<UINT>& que, queue<UINT>& result, UINT value)
{
	que = result;

	while (!result.empty())
		result.pop();

	while (!que.empty())
	{
		UINT data = que.front();
		que.pop();

		tTile tTile = m_pTileObject->TileMap()->GetInfo(data);

		if ((UINT)TILE_TYPE::NOTUSED == tTile.iInfo)
			m_pTileObject->TileMap()->SetInfo(data, value);

		m_bCheck[data] = true;

		if (((UINT)TILE_TYPE::USED) == value)
		{
			Int32 x = data % TILEX;
			Int32 z = data / TILEZ;
			CJpsMgr::GetInst()->SetCollision(x, z);
			CJpsMgr::GetInst()->SetMonsterCollision(x, z);
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
