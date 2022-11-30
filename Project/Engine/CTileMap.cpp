#include "pch.h"
#include "CTileMap.h"

#include "CLevel.h"
#include "CLevelMgr.h"
#include "CResMgr.h"

#include "CTransform.h"
#include "CCamera.h"

#include "CStructuredBuffer.h"

CTileMap::CTileMap()
	: CRenderComponent(COMPONENT_TYPE::TILEMAP)
{
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"Tile"));
	SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TileMapMtrl"));

	m_TileBuffer = new CStructuredBuffer;
}

CTileMap::~CTileMap()
{
	if (nullptr != m_TileBuffer)
		delete m_TileBuffer;
}

void CTileMap::Ready_Adjacency()
{
	m_vecAdjacency.resize(m_vecTile.size());

	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			int		iIndex = i * TILEX + j;

			// 좌상단 0 - 20 - 41 - 61
			// 맨 윗줄 아닌 경우  &&	맨 왼쪽 줄이 아닌 경우
			if ((0 != i) && (0 != iIndex % (TILEX * 2)))
			{
				// 홀수에서 짝수 줄로 이동할 때 20 감소
				if ((0 != i % 2) && (!m_vecTile[iIndex - TILEX].ibyOption))
				{
					m_vecAdjacency[iIndex].push_back(m_vecTile[iIndex - TILEX]);
				}
				// 짝수에서 홀수 줄로 이동할 때 21 감소
				else if ((0 == i % 2) && (!m_vecTile[iIndex - (TILEX + 1)].ibyOption))
				{
					m_vecAdjacency[iIndex].push_back(m_vecTile[iIndex - (TILEX + 1)]);
				}
			}

			// 우 상단
			// 맨 윗줄 아닌 경우  &&	맨 오른쪽 줄이 아닌 경우
			if ((0 != i) && ((TILEX * 2 - 1) != iIndex % (TILEX * 2)))
			{
				// 홀수에서 짝수 줄로 이동할 때 19 감소
				if ((0 != i % 2) && (!m_vecTile[iIndex - (TILEX - 1)].ibyOption))
				{
					m_vecAdjacency[iIndex].push_back(m_vecTile[iIndex - (TILEX - 1)]);
				}
				// 짝수에서 홀수 줄로 이동할 때 20 감소
				else if ((0 == i % 2) && (!m_vecTile[iIndex - TILEX].ibyOption))
				{
					m_vecAdjacency[iIndex].push_back(m_vecTile[iIndex - TILEX]);
				}
			}

			// 좌 하단 
			// 맨 아랫 줄 아닌 경우    &&	맨 왼쪽 줄이 아닌 경우
			if ((TILEY - 1 != i) && (0 != iIndex % (TILEX * 2)))
			{
				// 홀수에서 짝수 줄로 이동할 때 20 증가
				if ((0 != i % 2) && (!m_vecTile[iIndex + TILEX].ibyOption))
				{
					m_vecAdjacency[iIndex].push_back(m_vecTile[iIndex + TILEX]);
				}
				// 짝수에서 홀수 줄로 이동할 때 19 증가
				else if ((0 == i % 2) && (!m_vecTile[iIndex + (TILEX - 1)].ibyOption))
				{
					m_vecAdjacency[iIndex].push_back(m_vecTile[iIndex + (TILEX - 1)]);
				}
			}

			// 우 하단 
			// 맨 아랫 줄 아닌 경우    &&	맨 오른쪽 줄이 아닌 경우
			if ((TILEY - 1 != i) && ((TILEX * 2 - 1) != iIndex % (TILEX * 2)))
			{
				// 홀수에서 짝수 줄로 이동할 때 21 증가
				if ((0 != i % 2) && (!m_vecTile[iIndex + (TILEX + 1)].ibyOption))
				{
					m_vecAdjacency[iIndex].push_back(m_vecTile[iIndex + (TILEX + 1)]);
				}
				// 짝수에서 홀수 줄로 이동할 때 20 증가
				else if ((0 == i % 2) && (!m_vecTile[iIndex + TILEX].ibyOption))
				{
					m_vecAdjacency[iIndex].push_back(m_vecTile[iIndex + TILEX]);
				}
			}

		}
	}
}

void CTileMap::SetTileCount(UINT _iWidth, UINT _iHeight)
{
	m_vTileCount = Vec2((float)_iWidth, (float)_iHeight);

	m_vecTile.clear();
	m_vecTile.resize(_iWidth * _iHeight);

	if (m_TileBuffer->GetElementsCount() < _iWidth * _iHeight)
	{
		m_TileBuffer->Create(sizeof(tTile), _iWidth * _iHeight, SB_TYPE::SRV_ONLY, nullptr, true);
	}
}

void CTileMap::begin()
{
	//float fWidth = m_AtlasTex->GetWidth();
	//float fHeight = m_AtlasTex->GetHeight();

	m_vLeftTop = Vec2(64.f, 64.f);
	m_vSlice = Vec2(64.f, 64.f);

	//m_vLeftTop /= Vec2(fWidth, fHeight);
	//m_vSlice /= Vec2(fWidth, fHeight);

	tTile t{};
	//for (size_t i = 0; i < m_vecTile.size(); ++i)
	//{
	//	m_vecTile[i].vLeftTop = vLeftTop;
	//	m_vecTile[i].vSlice = vSlice;
	//}

	m_pCamera = CLevelMgr::GetInst()->GetCurLevel()->FindParentObjectByName(L"MainCamera");

	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			float	fX = (TILECX * j) + ((i % 2) * (TILECX / 2.f));
			float	fY = (TILECY / 2.f) * i;

			t.vLeftTop = m_vLeftTop;
			t.vSlice = m_vSlice;
			t.vPos = { fX, fY, 1.f };
			t.vSize = Vec3{ (float)TILECX, (float)TILECY, 0.f };
			t.iIndex = i * TILEX + j;
			t.iParentIndex = 0;
			t.ibyOption = 0;
			m_vecTile[i * TILEX + j] = t;
		}
	}

	m_vecTile[0].ibyOption = 1;
}

void CTileMap::finaltick()
{

}

void CTileMap::render()
{
	Transform()->UpdateData();

	//m_vecTile[0].vLeftTop = Vec2(0.f, 0.f);

	m_TileBuffer->SetData(m_vecTile.data(), (UINT)(m_vTileCount.x * m_vTileCount.y));
	m_TileBuffer->UpdateData(18, PIPELINE_STAGE::VS | PIPELINE_STAGE::PS);


	if (m_pCamera)
		m_vCameraPos = m_pCamera->Transform()->GetRelativePos();

	GetCurMaterial()->SetScalarParam(VEC2_0, &m_vLeftTop);
	GetCurMaterial()->SetScalarParam(VEC2_1, &m_vSlice);
	GetCurMaterial()->SetScalarParam(VEC2_2, &m_vTileCount);

	GetCurMaterial()->SetScalarParam(VEC4_0, &m_vCameraPos);

	GetCurMaterial()->SetTexParam(TEX_0, m_AtlasTex[0]);
	GetCurMaterial()->SetTexParam(TEX_1, m_AtlasTex[1]);
	GetCurMaterial()->SetTexParam(TEX_2, m_AtlasTex[2]);
	GetCurMaterial()->SetTexParam(TEX_3, m_AtlasTex[3]);
	GetCurMaterial()->SetTexParam(TEX_4, m_AtlasTex[4]);
	GetCurMaterial()->SetTexParam(TEX_5, m_AtlasTex[5]);
	GetCurMaterial()->SetTexParam(TEX_6, m_AtlasTex[6]);
	GetCurMaterial()->SetTexParam(TEX_7, m_AtlasTex[7]);
	GetCurMaterial()->SetTexParam(TEX_8, m_AtlasTex[8]);
	GetCurMaterial()->SetTexParam(TEX_9, m_AtlasTex[9]);
	GetCurMaterial()->SetTexParam(TEX_10, m_AtlasTex[10]);
	GetCurMaterial()->SetTexParam(TEX_11, m_AtlasTex[11]);
	GetCurMaterial()->SetTexParam(TEX_12, m_AtlasTex[12]);
	GetCurMaterial()->SetTexParam(TEX_13, m_AtlasTex[13]);
	GetCurMaterial()->SetTexParam(TEX_14, m_AtlasTex[14]);
	GetCurMaterial()->SetTexParam(TEX_15, m_AtlasTex[15]);
	GetCurMaterial()->SetTexParam(TEX_16, m_AtlasTex[16]);
	GetCurMaterial()->SetTexParam(TEX_17, m_AtlasTex[17]);
	GetCurMaterial()->SetTexParam(TEX_18, m_AtlasTex[18]);
	GetCurMaterial()->SetTexParam(TEX_19, m_AtlasTex[19]);
	GetCurMaterial()->SetTexParam(TEX_20, m_AtlasTex[20]);
	GetCurMaterial()->SetTexParam(TEX_21, m_AtlasTex[21]);
	GetCurMaterial()->SetTexParam(TEX_22, m_AtlasTex[22]);
	GetCurMaterial()->SetTexParam(TEX_23, m_AtlasTex[23]);
	GetCurMaterial()->SetTexParam(TEX_24, m_AtlasTex[24]);
	GetCurMaterial()->SetTexParam(TEX_25, m_AtlasTex[25]);
	GetCurMaterial()->SetTexParam(TEX_26, m_AtlasTex[26]);
	GetCurMaterial()->SetTexParam(TEX_27, m_AtlasTex[27]);
	GetCurMaterial()->SetTexParam(TEX_28, m_AtlasTex[28]);
	GetCurMaterial()->SetTexParam(TEX_29, m_AtlasTex[29]);
	GetCurMaterial()->SetTexParam(TEX_30, m_AtlasTex[30]);
	GetCurMaterial()->SetTexParam(TEX_31, m_AtlasTex[31]);
	GetCurMaterial()->SetTexParam(TEX_32, m_AtlasTex[32]);

	GetCurMaterial()->UpdateData();

	GetMesh()->render_particle(m_vecTile.size());

	CMaterial::Clear();
}