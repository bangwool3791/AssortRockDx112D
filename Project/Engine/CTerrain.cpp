#include "pch.h"
#include "CTerrain.h"

#include "CLevel.h"
#include "CLevelMgr.h"
#include "CResMgr.h"

#include "CTransform.h"
#include "CCamera.h"
#include "CStructuredBuffer.h"

#include "Base.h"

CTerrain::CTerrain()
	: CRenderComponent(COMPONENT_TYPE::TERRAIN)
{
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"TerrainMesh"));
	SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TileMapMtrl"));
	m_TileBuffer = new CStructuredBuffer;
}

CTerrain::~CTerrain()
{
	if (nullptr != m_TileBuffer)
		delete m_TileBuffer;
}

void CTerrain::begin()
{
	auto spCollision = std::make_shared<JPSCollision>();
	if (!spCollision)
		throw std::bad_alloc();

	//====================================================
// CREATE MAP 
//====================================================
	Int32 GridWidth = TERRAINX, GridHeight = TERRAINZ;
	spCollision->Create(GridWidth, GridHeight);

	srand(32);

	// Start, End Position (시작점, 도착점)
	Int32 Sx = 0, Sy = 0;
	Int32 Ex = GridWidth - 1, Ey = GridHeight - 1;

	// Make a Maze (충돌 미로를 만든다.)
	for (Int32 y = 0; y < GridHeight; ++y)
	{
		for (Int32 x = 0; x < GridWidth; ++x)
		{
			// Don't Mark Collision at Start, End Position (시작,도착점은 충돌처리 하지 않는다.)
			if (x == Sx && y == Sy)	continue;
			if (x == Ex && y == Ey) continue;

			// Randomly Mark Collision at Position (랜덤하게 충돌처리를 한다.)
			if (rand() % 50 == 0) spCollision->SetAt(x, y);
		}
	}

	//====================================================
	// Find PATH
	//====================================================
	std::list<JPSCoord> ResultNodes;	// Result for JPS

	JPSPath	jps;

	jps.Init(spCollision, GetMesh());

	jps.Search(Sx, Sy, Ex, Ey, ResultNodes);

	std::string results(GridHeight * (GridWidth + 1) + 1, ' ');

	vector<Vec3> vec;

	for (Int32 y = 0; y < GridHeight; y++)
	{
		for (Int32 x = 0; x < GridWidth; x++)
		{
			results[(GridHeight - 1 - y) * (GridWidth + 1) + x] = !spCollision->IsCollision(x, y) ? ' ' : '@';
		}
		results[(GridHeight - 1 - y) * (GridWidth + 1) + GridWidth] = '\n';
	}

	if (ResultNodes.size() > 0)
	{
		auto iterS = ResultNodes.begin();
		auto iterE = ResultNodes.end();
		auto iterP = iterS;	++iterS;
		for (; iterS != iterE; iterP = iterS, ++iterS)
		{
			auto& PreCoord = (*iterP);
			auto& CurCoord = (*iterS);

			Int32 x = PreCoord.m_x;
			Int32 y = PreCoord.m_y;
			Int32 dx = core::clamp<Int32>(CurCoord.m_x - PreCoord.m_x, -1, 1);
			Int32 dy = core::clamp<Int32>(CurCoord.m_y - PreCoord.m_y, -1, 1);

			for (Int32 v = y, u = x; ; v += dy, u += dx)
			{
				results[(GridHeight - 1 - v) * (GridWidth + 1) + u] = '#';
				vec.push_back(jps.GetCoord(u, v));

				if (u == CurCoord.m_x && v == CurCoord.m_y)
					break;

				Int32 deltax = core::clamp<Int32>(CurCoord.m_x - u, -1, 1);
				Int32 deltay = core::clamp<Int32>(CurCoord.m_y - v, -1, 1);
				if (deltax != dx || deltay != dy)
				{
					std::cout << "INVALID NODE\n";
					break;
				}
			}
			results[(GridHeight - 1 - CurCoord.m_y) * (GridWidth + 1) + CurCoord.m_x] = '#';
			vec.push_back(jps.GetCoord(CurCoord.m_x, CurCoord.m_y));
		}

		// Mark Start & End Position ('S', 'E' 로 시작점 도착점을 표시합니다.)
		auto	iterStart = ResultNodes.begin();
		auto	iterEnd = ResultNodes.rbegin();
		auto& startCoord = (*iterStart);
		auto& endCoord = (*iterEnd);
		results[(GridHeight - 1 - startCoord.m_y) * (GridWidth + 1) + startCoord.m_x] = 'S';
		results[(GridHeight - 1 - endCoord.m_y) * (GridWidth + 1) + endCoord.m_x] = 'E';
	}

	//====================================================
	// SAVE FILE
	//====================================================
	if (results.size() > 0)
	{
		FILE* pFile = nullptr;
		fopen_s(&pFile, "result_jps(b).txt", "wt");

		if (!pFile)
			return;

		if (pFile != NULL)
		{
			fwrite(results.c_str(), sizeof(char), results.size(), pFile);
			fclose(pFile);
		}
	}

	for (size_t i{}; i < vec.size(); ++i)
	{
		cout << "[x] " << vec[i].x << " " << "[z] " <<vec[i].z << endl;
	}
}

void CTerrain::finaltick()
{
	static bool bCheck = false;
	if (!bCheck)
	{
		bCheck = true;
		m_AtlasTex[32] = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Mask\\TileMask.png");
	}
	/*
	* 타일 Edit 모드이면 타일 벡터 순회 사이즈 조절
	*/
}

void CTerrain::render()
{
	Transform()->UpdateData();

	if (m_pCamera)
		m_vCameraPos = m_pCamera->Transform()->GetRelativePos();


	//GetCurMaterial()->SetScalarParam(VEC4_0, m_vTileSize);
	//GetCurMaterial()->SetScalarParam(VEC4_0, &m_vCameraPos);
	
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

	GetMesh()->render_particle(1);

	CMaterial::Clear();
}



void CTerrain::SaveToFile(FILE* _File)
{
	CRenderComponent::SaveToFile(_File);

	size_t size = m_AtlasTex.size();

	fwrite(&size, sizeof(size_t), 1, _File);
	
	for (size_t i{}; i < size; ++i)
	{
		SaveResourceRef(m_AtlasTex[i], _File);
	}
}

void CTerrain::LoadFromFile(FILE* _File)
{
	CRenderComponent::LoadFromFile(_File);

	size_t size = 0;

	fread(&size, sizeof(size_t), 1, _File);

	m_AtlasTex.resize(size);

	for (size_t i{}; i < size; ++i)
	{
		LoadResourceRef(m_AtlasTex[i], _File);
	}
}

void CTerrain::SetTextureID(Ray _ray, UINT i)
{
	GetMesh()->SetTextureID(_ray, i);
}