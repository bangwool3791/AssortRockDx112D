#include "pch.h"
#include "CJpsMgr.h"

CJpsMgr::CJpsMgr()
{

}

CJpsMgr::~CJpsMgr()
{

}

void CJpsMgr::Initialize(Ptr<CMesh> _Mesh)
{
	m_spCollision = std::make_shared<JPSCollision>();
	if (!m_spCollision)
		throw std::bad_alloc();


	//====================================================
// CREATE MAP 
//====================================================
	m_spCollision->Create(GridWidth, GridHeight);

	//====================================================
	// Find PATH
	//====================================================
	std::list<JPSCoord> ResultNodes;	// Result for JPS
	//이니셜 라이즈 여기서 
	m_JpsPath.Init(m_spCollision, _Mesh);
}

const vector<Vec3>& CJpsMgr::Update(Int32 x, Int32 z)
{
	// Start, End Position (시작점, 도착점)
	Int32 Sx = 0, Sy = 0;
	Int32 Ex = GridWidth - 1, Ey = GridHeight - 1;

	m_JpsPath.Search(Sx, Sy, Ex, Ey, ResultNodes);

	std::string results(GridHeight * (GridWidth + 1) + 1, ' ');

	vector<Vec3> vec;

	for (Int32 y = 0; y < GridHeight; y++)
	{
		for (Int32 x = 0; x < GridWidth; x++)
		{
			results[(GridHeight - 1 - y) * (GridWidth + 1) + x] = !m_spCollision->IsCollision(x, y) ? ' ' : '@';
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
				vec.push_back(m_JpsPath.GetCoord(u, v));

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
			vec.push_back(m_JpsPath.GetCoord(CurCoord.m_x, CurCoord.m_y));
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
		cout << "[x] " << vec[i].x << " " << "[z] " << vec[i].z << endl;
	}
}

void CJpsMgr::SetCollision(Int32 x, Int32 z)
{
	m_spCollision->SetAt(x, z);
}

void CJpsMgr::ClearCollision(Int32 x, Int32 z)
{
	m_spCollision->ClrAt(x, z);
}