#include "pch.h"
#include "CTileMap.h"

#include "CResMgr.h"
#include "CTransform.h"
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

			// �»�� 0 - 20 - 41 - 61
			// �� ���� �ƴ� ���  &&	�� ���� ���� �ƴ� ���
			if ((0 != i) && (0 != iIndex % (TILEX * 2)))
			{
				// Ȧ������ ¦�� �ٷ� �̵��� �� 20 ����
				if ((0 != i % 2) && (!m_vecTile[iIndex - TILEX].ibyOption))
				{
					m_vecAdjacency[iIndex].push_back(m_vecTile[iIndex - TILEX]);
				}
				// ¦������ Ȧ�� �ٷ� �̵��� �� 21 ����
				else if ((0 == i % 2) && (!m_vecTile[iIndex - (TILEX + 1)].ibyOption))
				{
					m_vecAdjacency[iIndex].push_back(m_vecTile[iIndex - (TILEX + 1)]);
				}
			}

			// �� ���
			// �� ���� �ƴ� ���  &&	�� ������ ���� �ƴ� ���
			if ((0 != i) && ((TILEX * 2 - 1) != iIndex % (TILEX * 2)))
			{
				// Ȧ������ ¦�� �ٷ� �̵��� �� 19 ����
				if ((0 != i % 2) && (!m_vecTile[iIndex - (TILEX - 1)].ibyOption))
				{
					m_vecAdjacency[iIndex].push_back(m_vecTile[iIndex - (TILEX - 1)]);
				}
				// ¦������ Ȧ�� �ٷ� �̵��� �� 20 ����
				else if ((0 == i % 2) && (!m_vecTile[iIndex - TILEX].ibyOption))
				{
					m_vecAdjacency[iIndex].push_back(m_vecTile[iIndex - TILEX]);
				}
			}

			// �� �ϴ� 
			// �� �Ʒ� �� �ƴ� ���    &&	�� ���� ���� �ƴ� ���
			if ((TILEY - 1 != i) && (0 != iIndex % (TILEX * 2)))
			{
				// Ȧ������ ¦�� �ٷ� �̵��� �� 20 ����
				if ((0 != i % 2) && (!m_vecTile[iIndex + TILEX].ibyOption))
				{
					m_vecAdjacency[iIndex].push_back(m_vecTile[iIndex + TILEX]);
				}
				// ¦������ Ȧ�� �ٷ� �̵��� �� 19 ����
				else if ((0 == i % 2) && (!m_vecTile[iIndex + (TILEX - 1)].ibyOption))
				{
					m_vecAdjacency[iIndex].push_back(m_vecTile[iIndex + (TILEX - 1)]);
				}
			}

			// �� �ϴ� 
			// �� �Ʒ� �� �ƴ� ���    &&	�� ������ ���� �ƴ� ���
			if ((TILEY - 1 != i) && ((TILEX * 2 - 1) != iIndex % (TILEX * 2)))
			{
				// Ȧ������ ¦�� �ٷ� �̵��� �� 21 ����
				if ((0 != i % 2) && (!m_vecTile[iIndex + (TILEX + 1)].ibyOption))
				{
					m_vecAdjacency[iIndex].push_back(m_vecTile[iIndex + (TILEX + 1)]);
				}
				// ¦������ Ȧ�� �ٷ� �̵��� �� 20 ����
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

void CTileMap::finaltick()
{

}

void CTileMap::render()
{
	Transform()->UpdateData();


	float fWidth = m_AtlasTex->GetWidth();
	float fHeight = m_AtlasTex->GetHeight();

	Vec2 vLeftTop = Vec2(64.f, 64.f);
	Vec2 vSlice = Vec2(64.f, 64.f);

	vLeftTop /= Vec2(fWidth, fHeight);
	vSlice /= Vec2(fWidth, fHeight);

	tTile t{};
	//for (size_t i = 0; i < m_vecTile.size(); ++i)
	//{
	//	m_vecTile[i].vLeftTop = vLeftTop;
	//	m_vecTile[i].vSlice = vSlice;
	//}
	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			float	fX = (TILECX * j) + ((i % 2) * (TILECX / 2.f));
			float	fY = (TILECY / 2.f) * i;

			t.vLeftTop		= vLeftTop;
			t.vSlice		= vSlice;
			t.vPos			= { fX, fY, 0.f };
			t.iIndex		= i * TILEX + j;
			t.iParentIndex  = 0;

			m_vecTile[i * TILEX + j] = t;
		}
	}

	//m_vecTile[0].vLeftTop = Vec2(0.f, 0.f);

	m_TileBuffer->SetData(m_vecTile.data(), (UINT)(m_vTileCount.x * m_vTileCount.y));
	m_TileBuffer->UpdateData(18, PIPELINE_STAGE::PS);



	GetCurMaterial()->SetScalarParam(VEC2_0, &vLeftTop);
	GetCurMaterial()->SetScalarParam(VEC2_1, &vSlice);
	GetCurMaterial()->SetScalarParam(VEC2_2, &m_vTileCount);

	GetCurMaterial()->SetTexParam(TEX_0, m_AtlasTex);

	GetCurMaterial()->UpdateData();

	GetMesh()->render();
}