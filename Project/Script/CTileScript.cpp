#include "pch.h"
#include "CTileScript.h"

#include <Engine\CDevice.h>
#include <Engine\CLevel.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CTransform.h>
#include <Engine\CTerrain.h>


CTileScript::CTileScript()
	:CScript{ TILESCRIPT}
{
	SetName(L"CTileScript");
	m_tTileInfo = {};
	m_tTileInfo.iIndex = -1;
}

CTileScript::~CTileScript()
{
}


void CTileScript::begin()
{
}
//레이식 피킹으로 수정
void CTileScript::tick()
{
	static Vec3 vMouse;

	//에디터의 마우스 오브젝트를 Set으로 입력 받은 뒤
	CTransform* pTransform = GetOwner()->Transform();
	Vec3 vPos   = pTransform->GetRelativePos();
	Vec3 vScale = pTransform->GetRelativeScale();

	Vec3 vLeftBottom = vPos - vScale;
	Vec3 vRightTop   = vPos + vScale;

	static bool bCheck = false;

	if (KEY_PRESSED(KEY::NUM_3))
	{
		bCheck = true;
	}

	if (bCheck)
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
		//cout << "마우스 [x] " << rayorigin.x << "[y] " << rayorigin.y << "[z] " << rayorigin.z << endl;

		Vec3 vPos = GetOwner()->TileMap()->GetMesh()->GetPosition(ray);
		tTile tTile;

		if (!Picking(vPos))
		{
			tTile = GetOwner()->TileMap()->GetInfo(vPos);
		
			if (m_tTileInfo.iIndex != tTile.iIndex)
			{
				GetOwner()->TileMap()->SetInfo(tTile.iIndex, 1);
				if (m_tTileInfo.iIndex != -1)
					GetOwner()->TileMap()->SetInfo(m_tTileInfo.iIndex, 0);

				m_tTileInfo.iIndex = tTile.iIndex;
			}
		}
	}
}

bool CTileScript::Picking(Vec3 vPos)
{
	vPos.y = 0.f;

	Vec3	vPoint[4] = {

		Vec3(m_tTileInfo.vPos.x,				   0.f, m_tTileInfo.vPos.z + (TILECZ * 0.5f)),	// 12
		Vec3(m_tTileInfo.vPos.x + (TILECX * 0.5f), 0.f, m_tTileInfo.vPos.z),					// 3
		Vec3(m_tTileInfo.vPos.x,				   0.f, m_tTileInfo.vPos.z - (TILECZ * 0.5f)),	// 6
		Vec3(m_tTileInfo.vPos.x - (TILECX * 0.5f), 0.f, m_tTileInfo.vPos.z),					// 9

	};

	Vec3		vDir[4] = {

		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3]

	};

	Vec3		vNormal[4] = {

		Vec3(-vDir[0].z, 0.f, vDir[0].x),
		Vec3(-vDir[1].z, 0.f, vDir[1].x),
		Vec3(-vDir[2].z, 0.f, vDir[2].x),
		Vec3(-vDir[3].z, 0.f, vDir[3].x)
	};

	for (int i = 0; i < 4; ++i)
		vNormal[i].Normalize();

	Vec3	vMouseDir[4] = {

		vPos - vPoint[0],
		vPos - vPoint[1],
		vPos - vPoint[2],
		vPos - vPoint[3]
	};

	for (int i = 0; i < 4; ++i)
		vNormal[i].Normalize();

	for (int i = 0; i < 4; ++i)
	{
		if (0.f < vNormal[i].Dot(vMouseDir[i]))
			return false;
	}

	return true;
}

void CTileScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CTileScript::Overlap(CCollider2D* _pOther)
{
}

void CTileScript::EndOverlap(CCollider2D* _pOther)
{
}

