#include "pch.h"
#include "CTileMap.h"

#include "CLevel.h"
#include "CLevelMgr.h"
#include "CResMgr.h"

#include "CTransform.h"
#include "CCamera.h"
#include "CStructuredBuffer.h"

#include "Base.h"

CTileMap::CTileMap()
	: CRenderComponent(COMPONENT_TYPE::TILEMAP)
{
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"TileMesh"));
	SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"UiTileMtrl"));
	m_TileBuffer = new CStructuredBuffer;

	m_AtlasTex = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Mask\\TileArea.png");

	tTile tTile{};
	for (int i = 0; i < TILEX; ++i)
	{
		for (int j = 0; j < TILEZ; ++j)
		{
			float	fX = (TILECX * j) + ((i % 2) * (TILECX * 0.5f));
			float	fZ = (TILECZ * 0.5f) * i;

			tTile.iInfo = 0;
			tTile.vPos = Vec3{ fX, 0.f, fZ };
			tTile.iIndex = i * TILEZ + j;
			m_vecInfo.push_back(tTile);
		}
	}
	m_vecInfo[TILEX * (TILECZ -1)].iInfo = 1;

	m_TileBuffer->Create(sizeof(tTile), TILEX * TILEZ, SB_TYPE::SRV_ONLY, m_vecInfo.data(), true);

	m_TileBuffer->SetData(m_vecInfo.data(), m_vecInfo.size());
}

CTileMap::~CTileMap()
{
	if (nullptr != m_TileBuffer)
		delete m_TileBuffer;
}

void CTileMap::begin()
{
}

void CTileMap::finaltick()
{
}

void CTileMap::render()
{
	Transform()->UpdateData();

	m_TileBuffer->UpdateData(56, PIPELINE_STAGE::VS | PIPELINE_STAGE::PS);

	GetCurMaterial()->SetTexParam(TEX_0, m_AtlasTex);

	GetCurMaterial()->UpdateData();

	GetMesh()->render_particle(1);

	CMaterial::Clear();
}

tTile CTileMap::GetInfo(Vec3 _vPos)
{
	UINT index = 0;
	int i = _vPos.z / (TILECZ * 0.5f);
	//int j = (_vPos.x - ((i % 2) * (TILECX * 0.5f))) / TILECX;
	for (; i < TILEX; ++i)
	{
		for (int j = 0; j < TILEZ; ++j)
		{
			index = i * TILEZ + j;
			if(Picking(_vPos, index))
				return m_vecInfo[index];
		}
	}
	return m_vecInfo[index];
}

void CTileMap::SetInfo(Vec3 _vPos, UINT _iInfo)
{
	UINT index = 0;
	//int i = _vPos.z / (TILECZ * 0.5f);
	//int j = (_vPos.x - ((i % 2) * (TILECX * 0.5f))) / TILECX;
	for (int i = 0; i < TILEZ; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			index = i * TILEX + j;
			if (Picking(_vPos, index))
				m_vecInfo[index].iInfo = _iInfo;
		}
	}
	m_TileBuffer->SetData(m_vecInfo.data(), m_vecInfo.size());
}

void CTileMap::SetInfo(UINT _iIndex, UINT _iInfo)
{
	m_vecInfo[_iIndex].iInfo = _iInfo;
	m_TileBuffer->SetData(m_vecInfo.data(), m_vecInfo.size());
}

bool CTileMap::Picking(Vec3 vPos, UINT& iIndex)
{
	vPos.y = 0.f;

	Vec3	vPoint[4] = {

		Vec3(m_vecInfo[iIndex].vPos.x,					 0.f, m_vecInfo[iIndex].vPos.z + (TILECZ * 0.5f)),	// 12
		Vec3(m_vecInfo[iIndex].vPos.x + (TILECX * 0.5f), 0.f, m_vecInfo[iIndex].vPos.z),					// 3
		Vec3(m_vecInfo[iIndex].vPos.x,					 0.f, m_vecInfo[iIndex].vPos.z - (TILECZ * 0.5f)),	// 6
		Vec3(m_vecInfo[iIndex].vPos.x - (TILECX * 0.5f), 0.f, m_vecInfo[iIndex].vPos.z),					// 9

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

void CTileMap::SaveToFile(FILE* _File)
{
	CRenderComponent::SaveToFile(_File);
}

void CTileMap::LoadFromFile(FILE* _File)
{
	CRenderComponent::LoadFromFile(_File);
}
