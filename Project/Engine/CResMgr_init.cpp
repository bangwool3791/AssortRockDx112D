#include "pch.h"
#include "CResMgr.h"

#include "CTransform.h"

#include "CTexture.h"
#include "CMeshRender.h"
#include "CPaintShader.h"

#include "CSound.h"

void CResMgr::init()
{
	initSound();

	CreateDefaultMesh();

	CreateDefaultTexture();

	CreateDefaultGraphicsShader();

	CreateDefaultComputeShader();

	CreateDefaultMaterial();

}

Ptr<CTexture> CResMgr::CreateTexture(const wstring& _strKey, UINT _iWidth, UINT _iHeight, DXGI_FORMAT _eFormat, UINT _iBindFlag)
{
	Ptr<CTexture> pTex = FindRes<CTexture>(_strKey);
	assert(!pTex.Get());

	pTex = new CTexture(true);

	pTex->Create(_iWidth, _iHeight, _eFormat, _iBindFlag);

	AddRes<CTexture>(_strKey, pTex.Get());
	return pTex;
}

Ptr<CTexture> CResMgr::CreateTexture(const wstring& _strKey, ComPtr<ID3D11Texture2D> _Tex2D)
{
	Ptr<CTexture> pTex = FindRes<CTexture>(_strKey);
	assert(!pTex.Get());

	pTex = new CTexture(true);

	pTex->Create(_Tex2D);

	AddRes<CTexture>(_strKey, pTex.Get());
	return pTex;
}

void CResMgr::CreateDefaultMesh()
{
	vector<Vtx> vecVtx;
	vector<UINT> vecIdx;

	back_insert_iterator iterVtx{ vecVtx };
	back_insert_iterator iterIdx{ vecIdx };
	
	Vtx v;

	CMesh* pMesh = nullptr;

	// 사각형 메쉬 만들기
	v.vPos = Vec3(-0.5f, 0.f, 0.5f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);
	*iterVtx = v;

	v.vPos = Vec3(0.5f, 0.f, 0.5f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(1.f, 0.f);
	*iterVtx = v;

	v.vPos = Vec3(0.5f, 0.f, -0.5f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(1.f, 1.f);
	*iterVtx = v;

	v.vPos = Vec3(-0.5f, 0.f, -0.5f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(0.f, 1.f);
	*iterVtx = v;

	*iterIdx = 0;
	*iterIdx = 2;
	*iterIdx = 3;

	*iterIdx = 0;
	*iterIdx = 1;
	*iterIdx = 2;

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddRes<CMesh>(L"RectMesh", pMesh);
	//vecIdx.clear();

	//*iterIdx = 0;
	//*iterIdx = 1;
	//*iterIdx = 2;
	//*iterIdx = 3;
	//*iterIdx = 0;

	pMesh = new CMesh(true);
	pMesh->SetName(L"RectMesh_Debug");
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddRes<CMesh>(L"RectMesh_Debug", pMesh);
	vecVtx.clear();
	vecIdx.clear();

	v.vPos = Vec3(-0.5f, 0.f, 0.5f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(1090.f / 2048.f, 200.f / 2048.f);
	*iterVtx = v;

	v.vPos = Vec3(0.5f, 0.f, 0.5f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(1190.f / 2048.f, 200.f / 2048.f);
	*iterVtx = v;

	v.vPos = Vec3(0.5f, 0.f, -0.5f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(1190.f / 2048.f, 275.f / 2048.f);
	*iterVtx = v;

	v.vPos = Vec3(-0.5f, 0.f, -0.5f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(1090.f / 2048.f, 275.f / 2048.f);
	*iterVtx = v;

	*iterIdx = 0;
	*iterIdx = 2;
	*iterIdx = 3;

	*iterIdx = 0;
	*iterIdx = 1;
	*iterIdx = 2;

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddRes<CMesh>(L"CrystalMesh", pMesh);
	vecVtx.clear();
	vecIdx.clear();

	v.vPos = Vec3(-0.5f, 0.f, 0.5f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f / 4096.f, 0.f / 4096.f);
	*iterVtx = v;

	v.vPos = Vec3(0.5f, 0.f, 0.5f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(520.f / 4096.f, 0.f / 4096.f);
	*iterVtx = v;

	v.vPos = Vec3(0.5f, 0.f, -0.5f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(520.f / 4096.f, 960.f / 4096.f);
	*iterVtx = v;

	v.vPos = Vec3(-0.5f, 0.f, -0.5f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(0.f / 4096.f, 960.f / 4096.f);
	*iterVtx = v;

	*iterIdx = 0;
	*iterIdx = 2;
	*iterIdx = 3;

	*iterIdx = 0;
	*iterIdx = 1;
	*iterIdx = 2;

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddRes<CMesh>(L"TreeMesh", pMesh);
	vecVtx.clear();
	vecIdx.clear();

	v.vPos = Vec3(-0.5f, 0.f, 0.5f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(640.f / 1024.f, 919.f / 1024.f);
	*iterVtx = v;

	v.vPos = Vec3(0.5f, 0.f, 0.5f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(678.f / 1024.f, 919.f / 1024.f);
	*iterVtx = v;

	v.vPos = Vec3(0.5f, 0.f, -0.5f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(678.f / 1024.f, 1014.f / 1024.f);
	*iterVtx = v;

	v.vPos = Vec3(-0.5f, 0.f, -0.5f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(640.f / 1024.f, 1014.f / 1024.f);
	*iterVtx = v;

	*iterIdx = 0;
	*iterIdx = 2;
	*iterIdx = 3;

	*iterIdx = 0;
	*iterIdx = 1;
	*iterIdx = 2;

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddRes<CMesh>(L"WWallMesh", pMesh);
	vecVtx.clear();
	vecIdx.clear();

	v.vPos = Vec3(-0.5f, 0.f, 0.5f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(2.f / 1024.f, 950.5f / 1024.f);
	*iterVtx = v;

	v.vPos = Vec3(0.5f, 0.f, 0.5f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(76.f / 1024.f, 950.5f / 1024.f);
	*iterVtx = v;

	v.vPos = Vec3(0.5f, 0.f, -0.5f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(76.f / 1024.f, 1014.f / 1024.f);
	*iterVtx = v;

	v.vPos = Vec3(-0.5f, 0.f, -0.5f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(2.f / 1024.f, 1014.f / 1024.f);
	*iterVtx = v;

	*iterIdx = 0;
	*iterIdx = 2;
	*iterIdx = 3;

	*iterIdx = 0;
	*iterIdx = 1;
	*iterIdx = 2;

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddRes<CMesh>(L"WWallWidthMesh", pMesh);
	vecVtx.clear();
	vecIdx.clear();

	v.vPos = Vec3(-0.5f, 0.f, 0.5f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(554.f / 1024.f, 277.f / 1024.f);
	*iterVtx = v;

	v.vPos = Vec3(0.5f, 0.f, 0.5f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(617.f / 1024.f, 277.f / 1024.f);
	*iterVtx = v;

	v.vPos = Vec3(0.5f, 0.f, -0.5f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(617.f / 1024.f, 370.f / 1024.f);
	*iterVtx = v;

	v.vPos = Vec3(-0.5f, 0.f, -0.5f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(554.f / 1024.f, 370.f / 1024.f);
	*iterVtx = v;

	*iterIdx = 0;
	*iterIdx = 2;
	*iterIdx = 3;

	*iterIdx = 0;
	*iterIdx = 1;
	*iterIdx = 2;

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddRes<CMesh>(L"WWallRightSideMesh", pMesh);
	vecVtx.clear();
	vecIdx.clear();

	v.vPos = Vec3(-0.5f, 0.f, 0.5f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(552.f / 1024.f, 667.f / 1024.f);
	*iterVtx = v;

	v.vPos = Vec3(0.5f, 0.f, 0.5f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(605.f / 1024.f, 667.f / 1024.f);
	*iterVtx = v;

	v.vPos = Vec3(0.5f, 0.f, -0.5f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(605.f / 1024.f, 760.f / 1024.f);
	*iterVtx = v;

	v.vPos = Vec3(-0.5f, 0.f, -0.5f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(552.f / 1024.f, 760.f / 1024.f);
	*iterVtx = v;

	*iterIdx = 0;
	*iterIdx = 2;
	*iterIdx = 3;

	*iterIdx = 0;
	*iterIdx = 1;
	*iterIdx = 2;

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddRes<CMesh>(L"WWallLeftSideMesh", pMesh);
	vecVtx.clear();
	vecIdx.clear();

	v.vPos = Vec3(-0.5f, 0.f, 0.5f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(1003.f / 1024.f, 206.f / 1024.f);
	*iterVtx = v;

	v.vPos = Vec3(0.5f, 0.f, 0.5f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(1018.f / 1024.f, 206.f / 1024.f);
	*iterVtx = v;

	v.vPos = Vec3(0.5f, 0.f, -0.5f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(1018.f / 1024.f, 313.f / 1024.f);
	*iterVtx = v;

	v.vPos = Vec3(-0.5f, 0.f, -0.5f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(1003.f / 1024.f, 313.f / 1024.f);
	*iterVtx = v;

	*iterIdx = 0;
	*iterIdx = 2;
	*iterIdx = 3;

	*iterIdx = 0;
	*iterIdx = 1;
	*iterIdx = 2;

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddRes<CMesh>(L"WWallHightMesh", pMesh);
	vecVtx.clear();
	vecIdx.clear();
	// 원형메쉬 만들기
	// 중심점	
	v.vPos = Vec3(0.f, 0.f, 1.f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);

	*iterVtx = v;

	int iSlice = 40;
	float fRadius = 0.5f;
	float fTheta = XM_2PI / (float)iSlice;

	for (int i = 0; i < iSlice; ++i)
	{
		v.vPos = Vec3(fRadius * cosf(fTheta * (float)i), 0.f, fRadius * sinf(fTheta * (float)i));
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		*iterVtx = v;
	}

	for (UINT i = 0; i < (UINT)iSlice; ++i)
	{
		*iterIdx = 0;
		if (i == iSlice - 1)
		{
			*iterIdx = 1;
		}
		else
		{
			*iterIdx = i + 2;
		}
		*iterIdx = i + 1;
	}

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddRes<CMesh>(L"CircleMesh", pMesh);
	vecIdx.clear();

	for (UINT i = 0; i < (UINT)iSlice; ++i)
	{
		*iterIdx = i + 1;
	}
	*iterIdx = 1;
	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddRes<CMesh>(L"CircleMesh_Debug", pMesh);
	vecVtx.clear();
	vecIdx.clear();

	v.vPos = Vec3(-0.5f, 0.f, 0.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 0.5f);
	*iterVtx = v;
	v.vPos = Vec3(0.f, 0.f, 0.5f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(0.5f, 0.f);
	*iterVtx = v;
	v.vPos = Vec3(0.5f, 0.f, 0.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(1.f, 0.5f);
	*iterVtx = v;
	v.vPos = Vec3(0.f, 0.f, -0.5f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(0.5f, 1.f);
	*iterVtx = v;

	*iterIdx = 0;
	*iterIdx = 1;
	*iterIdx = 2;

	*iterIdx = 0;
	*iterIdx = 2;
	*iterIdx = 3;

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddRes<CMesh>(L"Tile", pMesh);
	vecVtx.clear();
	vecIdx.clear();

	/*정점 메쉬
	*/
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);

	UINT idx = 0;

	pMesh = new CMesh(true);
	pMesh->Create(&v, 1, &idx, 1);
	AddRes<CMesh>(L"PointMesh", pMesh);
	vecVtx.clear();
	vecIdx.clear();

	int iOffset = TERRAINX * 64 >> 1;
	for (int i = 0; i < TERRAINX; ++i)
	{
		for (int j = 0; j < TERRAINZ; ++j)
		{
			for (int k = 0; k < 4; ++k)
			{
				//top view
				//float	fX = (64 >> 1) * (j - i) + iOffset;
				//float	fZ = (64 >> 1) * (j + i) + (64 >> 1);
				float	fX = (TERRAINCX * j) + ((i % 2) * (TERRAINCX * 0.5f));
				float	fZ = (TERRAINCZ * 0.5f) * i;

				if (k % 4 == 0)
					fX -= TERRAINCX * 0.5f;
				else if (k % 4 == 1)
					fZ += TERRAINCZ * 0.5f;
				else if (k % 4 == 2)
					fX += TERRAINCX * 0.5f;
				else if (k % 4 == 3)
					fZ -= TERRAINCZ * 0.5f;

				v.vPos = Vec3{ fX , 0.f, fZ };
				//cout << "[x][z] " << v.vPos.x << " " << v.vPos.z << endl;
				v.vColor = Vec4(0.f, 0.f, 0.f, 1.f);
				*iterVtx = v;
			}
		}
	}


	for (int j = 0; j < TERRAINX * TERRAINZ; ++j)
	{
		*iterIdx = 0 + j * 4;
		*iterIdx = 1 + j * 4;
		*iterIdx = 2 + j * 4;
		*iterIdx = 0 + j * 4;
		*iterIdx = 2 + j * 4;
		*iterIdx = 3 + j * 4;

		vecVtx[0 + j * 4].vUV = Vec2(0.f, 0.f);
		vecVtx[1 + j * 4].vUV = Vec2(1.f, 0.f);
		vecVtx[2 + j * 4].vUV = Vec2(1.f, 1.f);
		vecVtx[3 + j * 4].vUV = Vec2(0.f, 1.f);
	}

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddRes<CMesh>(L"TerrainMesh", pMesh);
	vecVtx.clear();
	vecIdx.clear();

	for (int i = 0; i < TILEX; ++i)
	{
		for (int j = 0; j < TILEZ; ++j)
		{
			for (int k = 0; k < 4; ++k)
			{
				float	fX = (TILECX * j) + ((i % 2) * (TILECX * 0.5f));
				float	fZ = (TILECZ * 0.5f) * i;

				if (k % 4 == 0)
					fX -= TILECX * 0.5f;
				else if (k % 4 == 1)
					fZ += TILECZ * 0.5f;
				else if (k % 4 == 2)
					fX += TILECX * 0.5f;
				else if (k % 4 == 3)
					fZ -= TILECZ * 0.5f;

				v.vPos = Vec3{ fX, 0.f, fZ };
				v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
				*iterVtx = v;
			}
		}
	}

	for (int j = 0; j < TILEX * TILEZ; ++j)
	{
		*iterIdx = 0 + j * 4;
		*iterIdx = 1 + j * 4;

		*iterIdx = 1 + j * 4;
		*iterIdx = 2 + j * 4;

		*iterIdx = 2 + j * 4;
		*iterIdx = 3 + j * 4;

		*iterIdx = 3 + j * 4;
		*iterIdx = 0 + j * 4;

		vecVtx[0 + j * 4].vUV = Vec2(0.f, 0.f);
		vecVtx[1 + j * 4].vUV = Vec2(1.f, 0.f);
		vecVtx[2 + j * 4].vUV = Vec2(1.f, 1.f);
		vecVtx[3 + j * 4].vUV = Vec2(0.f, 0.f);

	}

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddRes<CMesh>(L"BorderMesh", pMesh);
	vecVtx.clear();
	vecIdx.clear();

	iOffset = TILEX * 64 >> 1;
	for (int i = 0; i < TILEX; ++i)
	{
		for (int j = 0; j < TILEZ; ++j)
		{
			for (int k = 0; k < 4; ++k)
			{
				//top view
				//float	fX = (64 >> 1) * (j - i) + iOffset;
				//float	fZ = (64 >> 1) * (j + i) + (64 >> 1);
				float	fX = (TILECX * j) + ((i % 2) * (TILECX * 0.5f));
				float	fZ = (TILECZ * 0.5f) * i;

				if (k % 4 == 0)
					fX -= (TILECX * 0.5f - 5.f);
				else if (k % 4 == 1)
					fZ += (TILECZ * 0.5f - 5.f);
				else if (k % 4 == 2)
					fX += (TILECX * 0.5f - 5.f);
				else if (k % 4 == 3)
					fZ -= (TILECZ * 0.5f - 5.f);

				v.vPos = Vec3{ fX, 0.f, fZ };
				v.vColor = Vec4(0.f, 0.f, 0.f, 1.f);
				v.vColor = Vec4(i * TILEZ + j, 0.f, 0.f, 0.f);
				*iterVtx = v;
			}
		}
	}


	for (int j = 0; j < TILEX * TILEZ; ++j)
	{
		*iterIdx = 0 + j * 4;
		*iterIdx = 1 + j * 4;
		*iterIdx = 2 + j * 4;
		*iterIdx = 0 + j * 4;
		*iterIdx = 2 + j * 4;
		*iterIdx = 3 + j * 4;

		vecVtx[0 + j * 4].vUV = Vec2(0.f, 0.f);
		vecVtx[1 + j * 4].vUV = Vec2(1.f, 0.f);
		vecVtx[2 + j * 4].vUV = Vec2(1.f, 1.f);
		vecVtx[3 + j * 4].vUV = Vec2(0.f, 1.f);
	}

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddRes<CMesh>(L"TileMesh", pMesh);
	vecVtx.clear();
	vecIdx.clear();


}

void CResMgr::CreateDefaultTexture()
{
	Load<CTexture>(L"texture\\Player.bmp", L"texture\\Player.bmp");
	Load<CTexture>(L"texture\\smokeparticle.png", L"texture\\smokeparticle.png");
	Load<CTexture>(L"texture\\Character.png", L"texture\\Character.png");
	Load<CTexture>(L"texture\\link.png", L"texture\\link.png");

	Load<CTexture>(L"texture\\beheaded.png", L"texture\\beheaded.png");
	Load<CTexture>(L"texture\\beheaded_n.png", L"texture\\beheaded_n.png");

	Load<CTexture>(L"texture\\particle\\smokeparticle.png", L"texture\\particle\\smokeparticle.png");
	Load<CTexture>(L"texture\\particle\\CartoonSmoke.png", L"texture\\particle\\CartoonSmoke.png");
	Load<CTexture>(L"texture\\particle\\Bubbles50px.png", L"texture\\particle\\Bubbles50px.png");
	// NoiseTexture
	Load<CTexture>(L"texture\\noise\\noise_01.png", L"texture\\noise\\noise_01.png");
	Load<CTexture>(L"texture\\noise\\noise_02.png", L"texture\\noise\\noise_02.png");
	Load<CTexture>(L"texture\\noise\\noise_03.jpg", L"texture\\noise\\noise_03.jpg");

	Load<CTexture>(L"texture\\particle\\Sparks.png", L"texture\\particle\\Sparks.png");
	Load<CTexture>(L"texture\\Mask\\TileMask.png", L"texture\\Mask\\TileMask.png");

	Load<CTexture>(L"texture\\TILE.bmp", L"texture\\TILE.bmp");

	Load<CTexture>(L"texture\\Mask\\buildmask.png", L"texture\\Mask\\buildmask.png");

	for (UINT i{}; i < TEX_32; i++)
	{
		wstring str = L"texture\\Terrain\\Tile\\Tile";
		str += std::to_wstring(i) + L".png";
		Load<CTexture>(str, str);
	}

	/*
	* They are billions
	*/
	Load<CTexture>(L"texture\\HumansA_LQ.png", L"texture\\HumansA_LQ.png");
	Load<CTexture>(L"texture\\Interface\\Atlas1_LQ.dds", L"texture\\Interface\\Atlas1_LQ.dds");
	Load<CTexture>(L"texture\\Interface\\Atlas1_LQ.dds", L"texture\\Interface\\Atlas1_LQ.dds");
	Load<CTexture>(L"texture\\Interface\\Icons.png", L"texture\\Interface\\Icons.png");
	Load<CTexture>(L"texture\\Interface\\Portraits.dds", L"texture\\Interface\\Portraits.dds");
	Load<CTexture>(L"texture\\geology\\Atlas1_LQ.dds", L"texture\\geology\\Atlas1_LQ.dds");
	Load<CTexture>(L"texture\\vegetation\\Atlas1_HQ.dds", L"texture\\vegetation\\Atlas1_HQ.dds");
	Load<CTexture>(L"texture\\buildings\\Atlas1_LQ.dds", L"texture\\buildings\\Atlas1_LQ.dds");

	//sprite
	Load<CTexture>(L"texture\\sprite\\HumansA_LQ.dds", L"texture\\sprite\\HumansA_LQ.dds");
	Load<CTexture>(L"texture\\sprite\\AnimalsA_HQ.dds", L"texture\\sprite\\AnimalsA_HQ.dds");
	Load<CTexture>(L"texture\\sprite\\AnimalsB_HQ.dds", L"texture\\sprite\\AnimalsB_HQ.dds");
	Load<CTexture>(L"texture\\sprite\\BigUnitsA_HQ.dds", L"texture\\sprite\\BigUnitsA_HQ.dds");
	Load<CTexture>(L"texture\\sprite\\BuildingsA_HQ.dds", L"texture\\sprite\\BuildingsA_HQ.dds");
	Load<CTexture>(L"texture\\sprite\\BuildingsB_HQ.dds", L"texture\\sprite\\BuildingsB_HQ.dds");
	Load<CTexture>(L"texture\\sprite\\BuildingsC_HQ.dds", L"texture\\sprite\\BuildingsC_HQ.dds");
	Load<CTexture>(L"texture\\sprite\\Effects_A_HQ.dds", L"texture\\sprite\\Effects_A_HQ.dds");
	Load<CTexture>(L"texture\\sprite\\EnvironmentA_HQ.dds", L"texture\\sprite\\EnvironmentA_HQ.dds");
	Load<CTexture>(L"texture\\sprite\\FortressA_HQ.dds", L"texture\\sprite\\FortressA_HQ.dds");
	Load<CTexture>(L"texture\\sprite\\HA_HQ.dds", L"texture\\sprite\\HA_HQ.dds");
	Load<CTexture>(L"texture\\sprite\\HB_HQ.dds", L"texture\\sprite\\HB_HQ.dds");
	Load<CTexture>(L"texture\\sprite\\HumansA_HQ.dds", L"texture\\sprite\\HumansA_HQ.dds");
	Load<CTexture>(L"texture\\sprite\\HumansB_HQ.dds", L"texture\\sprite\\HumansB_HQ.dds");
	Load<CTexture>(L"texture\\sprite\\HumansC_HQ.dds", L"texture\\sprite\\HumansC_HQ.dds");
	Load<CTexture>(L"texture\\sprite\\InfectedA_HQ.dds", L"texture\\sprite\\InfectedA_HQ.dds");
	Load<CTexture>(L"texture\\sprite\\InfectedB_HQ.dds", L"texture\\sprite\\InfectedB_HQ.dds");
	Load<CTexture>(L"texture\\sprite\\InfectedC_HQ.dds", L"texture\\sprite\\InfectedC_HQ.dds");
	Load<CTexture>(L"texture\\sprite\\InfectedD_HQ.dds", L"texture\\sprite\\InfectedD_HQ.dds");
	Load<CTexture>(L"texture\\sprite\\InfectedGiant_HQ.dds", L"texture\\sprite\\InfectedGiant_HQ.dds");
	Load<CTexture>(L"texture\\sprite\\TrainsA_HQ.dds", L"texture\\sprite\\TrainsA_HQ.dds");
	Load<CTexture>(L"texture\\sprite\\Icons_HQ.dds", L"texture\\sprite\\Icons_HQ.dds");
	Load<CTexture>(L"texture\\sprite\\Interface_HQ.dds", L"texture\\sprite\\Interface_HQ.dds");
	Load<CTexture>(L"texture\\sprite\\Portraits_HQ.dds", L"texture\\sprite\\Portraits_HQ.dds");
	Load<CTexture>(L"texture\\sprite\\Geology_HQ.dds", L"texture\\sprite\\Geology_HQ.dds");
	//sprite png
	Load<CTexture>(L"texture\\sprite\\HumansA_LQ.png", L"texture\\sprite\\HumansA_LQ.png");
	Load<CTexture>(L"texture\\sprite\\AnimalsA_HQ.png", L"texture\\sprite\\AnimalsA_HQ.png");
	Load<CTexture>(L"texture\\sprite\\AnimalsB_HQ.png", L"texture\\sprite\\AnimalsB_HQ.png");
	Load<CTexture>(L"texture\\sprite\\BigUnitsA_HQ.png", L"texture\\sprite\\BigUnitsA_HQ.png");
	Load<CTexture>(L"texture\\sprite\\BuildingsA_HQ.png", L"texture\\sprite\\BuildingsA_HQ.png");
	Load<CTexture>(L"texture\\sprite\\BuildingsB_HQ.png", L"texture\\sprite\\BuildingsB_HQ.png");
	Load<CTexture>(L"texture\\sprite\\BuildingsC_HQ.png", L"texture\\sprite\\BuildingsC_HQ.png");
	Load<CTexture>(L"texture\\sprite\\Effects_A_HQ.png", L"texture\\sprite\\Effects_A_HQ.png");
	Load<CTexture>(L"texture\\sprite\\EnvironmentA_HQ.png", L"texture\\sprite\\EnvironmentA_HQ.png");
	Load<CTexture>(L"texture\\sprite\\FortressA_HQ.png", L"texture\\sprite\\FortressA_HQ.png");
	Load<CTexture>(L"texture\\sprite\\HA_HQ.png", L"texture\\sprite\\HA_HQ.png");
	Load<CTexture>(L"texture\\sprite\\HB_HQ.png", L"texture\\sprite\\HB_HQ.png");
	Load<CTexture>(L"texture\\sprite\\HumansA_HQ.png", L"texture\\sprite\\HumansA_HQ.png");
	Load<CTexture>(L"texture\\sprite\\HumansB_HQ.png", L"texture\\sprite\\HumansB_HQ.png");
	Load<CTexture>(L"texture\\sprite\\HumansC_HQ.png", L"texture\\sprite\\HumansC_HQ.png");
	Load<CTexture>(L"texture\\sprite\\InfectedA_HQ.png", L"texture\\sprite\\InfectedA_HQ.png");
	Load<CTexture>(L"texture\\sprite\\InfectedB_HQ.png", L"texture\\sprite\\InfectedB_HQ.png");
	Load<CTexture>(L"texture\\sprite\\InfectedC_HQ.png", L"texture\\sprite\\InfectedC_HQ.png");
	Load<CTexture>(L"texture\\sprite\\InfectedD_HQ.png", L"texture\\sprite\\InfectedD_HQ.png");
	Load<CTexture>(L"texture\\sprite\\InfectedGiant_HQ.png", L"texture\\sprite\\InfectedGiant_HQ.png");
	Load<CTexture>(L"texture\\sprite\\TrainsA_HQ.png", L"texture\\sprite\\TrainsA_HQ.png");
	Load<CTexture>(L"texture\\sprite\\Proyectiles_LQ.png", L"texture\\sprite\\Proyectiles_LQ.png");
	Load<CTexture>(L"texture\\sprite\\Icons_HQ.png", L"texture\\sprite\\Icons_HQ.png");
	Load<CTexture>(L"texture\\sprite\\Interface_HQ.png", L"texture\\sprite\\Interface_HQ.png");
	Load<CTexture>(L"texture\\sprite\\Portraits_HQ.png", L"texture\\sprite\\Portraits_HQ.png");
	Load<CTexture>(L"texture\\sprite\\Geology_HQ.png", L"texture\\sprite\\Geology_HQ.png");

	CreateTexture(L"UAVTex", 1024, 1024, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE |
		D3D11_BIND_UNORDERED_ACCESS);
}

void CResMgr::CreateDefaultGraphicsShader()
{
	AddInputLayout(DXGI_FORMAT_R32G32B32_FLOAT, "POSITION");
	AddInputLayout(DXGI_FORMAT_R32G32B32A32_FLOAT, "COLOR");
	AddInputLayout(DXGI_FORMAT_R32G32_FLOAT, "TEXCOORD");

	CGraphicsShader* pShader = nullptr;

	pShader = new CGraphicsShader();
	pShader->CreateVertexShader(L"shader\\test.fx", "VS_DeadCell");
	pShader->CreatePixelShader(L"shader\\test.fx", "PS_DeadCell");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);

	AddRes<CGraphicsShader>(L"TestShader", pShader);

	pShader = new CGraphicsShader();
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	pShader->AddScalarParam(INT_0, "Test Int");
	pShader->AddScalarParam(FLOAT_2, "Test Float");
	pShader->AddScalarParam(VEC2_3, "Test Vec2");
	pShader->AddScalarParam(VEC4_2, "Test Vec4");
	pShader->AddTexureParam(TEX_0, "Output Texture 1");
	pShader->AddTexureParam(TEX_1, "Output Texture 2");
	pShader->AddTexureParam(TEX_2, "Output Texture 3");
	/*
	* 앞에는 그려지나, 언제던지 먼 사물체 그려질 수 있다.
	*/
	AddRes<CGraphicsShader>(L"Std2DShader", pShader);

	/*
	* 알파블랜드용 셰이더
	*/
	pShader = new CGraphicsShader();
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D_AlphaBlend");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_WRITE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddRes<CGraphicsShader>(L"Std2DAlphaBlendShader", pShader);

	pShader = new CGraphicsShader();
	pShader->CreateVertexShader(L"shader\\DragEffect.fx", "VS_DragEffect");
	pShader->CreatePixelShader(L"shader\\DragEffect.fx", "PS_DragEffect");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_WRITE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddRes<CGraphicsShader>(L"DragEffectShader", pShader);

	pShader = new CGraphicsShader();
	pShader->CreateVertexShader(L"shader\\editor.fx", "VS_Grid");
	pShader->CreatePixelShader(L"shader\\editor.fx", "PS_Grid");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_WRITE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddRes<CGraphicsShader>(L"EditorShader", pShader);

	pShader = new CGraphicsShader();
	pShader->CreateVertexShader(L"shader\\debugdraw.fx", "VS_DebugDraw");
	pShader->CreatePixelShader(L"shader\\debugdraw.fx", "PS_DebugDraw");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddRes<CGraphicsShader>(L"DebugDrawShader", pShader);

	pShader = new CGraphicsShader();
	pShader->CreateVertexShader(L"shader\\selectedunit.fx", "VS_DebugDraw");
	pShader->CreatePixelShader(L"shader\\selectedunit.fx", "PS_DebugDraw");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);

	AddRes<CGraphicsShader>(L"UnitSelectUIShader", pShader);

	// ParticleRenderShader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\particlerender.fx", "VS_ParticleRender");
	pShader->CreateGeometryShader(L"shader\\particlerender.fx", "GS_ParticleRender");
	pShader->CreatePixelShader(L"shader\\particlerender.fx", "PS_ParticleRender");

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddRes<CGraphicsShader>(L"ParticleRenderShader", pShader);

	// Instancing Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\objectrenderer.fx", "VS_ObjectRender");
	pShader->CreatePixelShader(L"shader\\objectrenderer.fx", "PS_ObjectRender");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	pShader->AddScalarParam(INT_0, "Use Atlas");
	pShader->AddScalarParam(VEC2_0, "UV LeftTop");
	pShader->AddScalarParam(VEC2_1, "UV Slice");
	pShader->AddTexureParam(TEX_0, "Output Texture 1");

	AddRes<CGraphicsShader>(L"ObjectRenderShader", pShader);

	//UI Domain Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\ui.fx", "VS_UI");
	pShader->CreatePixelShader(L"shader\\ui.fx", "PS_UI");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);
	AddRes<CGraphicsShader>(L"UIRenderShader", pShader);

	//Opaque
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\opaque.fx", "VS_Opaque");
	pShader->CreatePixelShader(L"shader\\opaque.fx", "PS_Opaque");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);
	AddRes<CGraphicsShader>(L"OpaqueShader", pShader);

	// Instancing Shadow Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\shadowrenderer.fx", "VS_ShadowRender");
	pShader->CreatePixelShader(L"shader\\shadowrenderer.fx", "PS_ShadowRender");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddRes<CGraphicsShader>(L"ShadowRenderShader", pShader);

	// PostProcess Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_PostProcess");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_PostProcess");

	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POST_PROCESS);

	AddRes<CGraphicsShader>(L"PostProcessShader", pShader);

	// TileMap Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\terrain.fx", "VS_TileMap");
	//pShader->CreateGeometryShader(L"shader\\tilemap.fx", "GS_TileMap");
	pShader->CreatePixelShader(L"shader\\terrain.fx", "PS_TileMap");
	/*
	* 투명한 타일이 있을 경우
	*/
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	/*
	* 알파블랜드는 타일은 생각하기 힘들다
	*/
	AddRes<CGraphicsShader>(L"TileMapShader", pShader);

	//수정 필요
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\ref_animation.fx", "VS_RefAni");
	pShader->CreatePixelShader(L"shader\\ref_animation.fx", "PS_RefAni");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);
	AddRes<CGraphicsShader>(L"RefAniShader", pShader);

	//Tile Map Border Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\bordermap.fx", "VS_Border");
	pShader->CreatePixelShader(L"shader\\bordermap.fx", "PS_Border");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
	AddRes<CGraphicsShader>(L"BorderShader", pShader);

	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\tilemap.fx", "VS_UiTileMap");
	pShader->CreatePixelShader(L"shader\\tilemap.fx", "PS_UiTileMap");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);
	AddRes<CGraphicsShader>(L"UiTileShader", pShader);

	//Building Red, Green
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\building.fx", "VS_BuildRender");
	pShader->CreatePixelShader(L"shader\\building.fx", "PS_BuildRender");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ONE_ONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);
	AddRes<CGraphicsShader>(L"BuildShader", pShader);

	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\crystal.fx", "VS_CrystalRender");
	pShader->CreatePixelShader(L"shader\\crystal.fx", "PS_CrystalRender");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);
	AddRes<CGraphicsShader>(L"CrystalShader", pShader);

	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\tree.fx", "VS_TreeRender");
	pShader->CreatePixelShader(L"shader\\tree.fx", "PS_TreeRender");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);
	AddRes<CGraphicsShader>(L"TreeShader", pShader);
}

#include "CComputeShader.h"
#include "CParticleUpdateShader.h"

void CResMgr::CreateDefaultComputeShader()
{
	CComputeShader* pShader = nullptr;
	pShader = new CPaintShader;
	pShader->CreateComputeShader(L"shader\\compute.fx", "CS_Paint");
	AddRes<CComputeShader>(L"PaintShader", pShader);

	pShader = new CParticleUpdateShader;
	pShader->CreateComputeShader(L"shader\\particleupdate.fx", "CS_ParticleUpdate");
	AddRes<CComputeShader>(L"ParticleUpdateShader", pShader);

	pShader = new CParticleUpdateShader;
	pShader->CreateComputeShader(L"shader\\particleupdatewood.fx", "CS_ParticleUpdate");
	AddRes<CComputeShader>(L"ParticleUpdateWoodShader", pShader);
}

void CResMgr::CreateDefaultMaterial()
{

	CMaterial* pMaterial = new CMaterial(true);
	pMaterial->SetShader(FindRes<CGraphicsShader>(L"TestShader"));
	AddRes(L"TestMtrl", pMaterial);

	pMaterial = new CMaterial(true);
	pMaterial->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	AddRes(L"Std2DMtrl", pMaterial);

	pMaterial = new CMaterial(true);
	pMaterial->SetShader(FindRes<CGraphicsShader>(L"Std2DAlphaBlendShader"));
	AddRes(L"Std2DAlphaBlendMtrl", pMaterial);

	pMaterial = new CMaterial(true);
	pMaterial->SetShader(FindRes<CGraphicsShader>(L"DragEffectShader"));
	AddRes(L"MouseDragMaterial", pMaterial);

	pMaterial = new CMaterial(true);
	pMaterial->SetShader(FindRes<CGraphicsShader>(L"EditorShader"));
	AddRes(L"EditMaterial", pMaterial);

	pMaterial = new CMaterial(true);
	pMaterial->SetShader(FindRes<CGraphicsShader>(L"UnitSelectUIShader"));
	AddRes(L"UnitSelectUIMaterial", pMaterial);

	pMaterial = new CMaterial(true);
	pMaterial->SetShader(FindRes<CGraphicsShader>(L"ParticleRenderShader"));
	AddRes(L"ParticleRenderMtrl", pMaterial);

	pMaterial = new CMaterial(true);
	pMaterial->SetShader(FindRes<CGraphicsShader>(L"ParticleRenderShader"));
	AddRes(L"ParticleRenderWoodMtrl", pMaterial);

	pMaterial = new CMaterial(true);
	pMaterial->SetShader(FindRes<CGraphicsShader>(L"ObjectRenderShader"));
	AddRes(L"ObjectMtrl", pMaterial);

	pMaterial = new CMaterial(true);
	pMaterial->SetShader(FindRes<CGraphicsShader>(L"PostProcessShader"));
	AddRes<CMaterial>(L"PostProcessMtrl", pMaterial);

	pMaterial = new CMaterial(true);
	pMaterial->SetShader(FindRes<CGraphicsShader>(L"DebugDrawShader"));
	AddRes<CMaterial>(L"DebugDrawMtrl", pMaterial);

	pMaterial = new CMaterial(true);
	pMaterial->SetShader(FindRes<CGraphicsShader>(L"ShadowRenderShader"));
	AddRes<CMaterial>(L"ShadowMtrl", pMaterial);

	pMaterial = new CMaterial(true);
	pMaterial->SetShader(FindRes<CGraphicsShader>(L"TileMapShader"));
	AddRes<CMaterial>(L"TileMapMtrl", pMaterial);

	pMaterial = new CMaterial(true);
	pMaterial->SetShader(FindRes<CGraphicsShader>(L"RefAniShader"));
	AddRes<CMaterial>(L"RefAniMtrl", pMaterial);

	pMaterial = new CMaterial(true);
	pMaterial->SetShader(FindRes<CGraphicsShader>(L"BorderShader"));
	AddRes<CMaterial>(L"BorderMtrl", pMaterial);

	pMaterial = new CMaterial(true);
	pMaterial->SetShader(FindRes<CGraphicsShader>(L"UiTileShader"));
	AddRes<CMaterial>(L"UiTileMtrl", pMaterial);

	pMaterial = new CMaterial(true);
	pMaterial->SetShader(FindRes<CGraphicsShader>(L"UIRenderShader"));
	AddRes<CMaterial>(L"InterfaceMtrl", pMaterial);

	pMaterial = new CMaterial(true);
	pMaterial->SetShader(FindRes<CGraphicsShader>(L"OpaqueShader"));
	AddRes<CMaterial>(L"OpaqueMtrl", pMaterial);

	pMaterial = new CMaterial(true);
	pMaterial->SetShader(FindRes<CGraphicsShader>(L"BuildShader"));
	AddRes<CMaterial>(L"BuildMtrl", pMaterial);

	pMaterial = new CMaterial(true);
	pMaterial->SetShader(FindRes<CGraphicsShader>(L"CrystalShader"));
	AddRes<CMaterial>(L"CrystalMtrl", pMaterial);

	pMaterial = new CMaterial(true);
	pMaterial->SetShader(FindRes<CGraphicsShader>(L"TreeShader"));
	AddRes<CMaterial>(L"TreeMtrl", pMaterial);

}

int GetSizeofFormat(DXGI_FORMAT _eFormat)
{
	int iRetByte = 0;
	switch (_eFormat)
	{
	case DXGI_FORMAT_R32G32B32A32_TYPELESS:
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
	case DXGI_FORMAT_R32G32B32A32_UINT:
	case DXGI_FORMAT_R32G32B32A32_SINT:
		iRetByte = 128;
		break;

	case DXGI_FORMAT_R32G32B32_TYPELESS:
	case DXGI_FORMAT_R32G32B32_FLOAT:
	case DXGI_FORMAT_R32G32B32_UINT:
	case DXGI_FORMAT_R32G32B32_SINT:
		iRetByte = 96;
		break;
	case DXGI_FORMAT_R16G16B16A16_TYPELESS:
	case DXGI_FORMAT_R16G16B16A16_FLOAT:
	case DXGI_FORMAT_R16G16B16A16_UNORM:
	case DXGI_FORMAT_R16G16B16A16_UINT:
	case DXGI_FORMAT_R16G16B16A16_SNORM:
	case DXGI_FORMAT_R16G16B16A16_SINT:
	case DXGI_FORMAT_R32G32_TYPELESS:
	case DXGI_FORMAT_R32G32_FLOAT:
	case DXGI_FORMAT_R32G32_UINT:
	case DXGI_FORMAT_R32G32_SINT:
	case DXGI_FORMAT_R32G8X24_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
	case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
	case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
		iRetByte = 64;
		break;
	case DXGI_FORMAT_R10G10B10A2_TYPELESS:
	case DXGI_FORMAT_R10G10B10A2_UNORM:
	case DXGI_FORMAT_R10G10B10A2_UINT:
	case DXGI_FORMAT_R11G11B10_FLOAT:
	case DXGI_FORMAT_R8G8B8A8_TYPELESS:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
	case DXGI_FORMAT_R8G8B8A8_UINT:
	case DXGI_FORMAT_R8G8B8A8_SNORM:
	case DXGI_FORMAT_R8G8B8A8_SINT:
	case DXGI_FORMAT_R16G16_TYPELESS:
	case DXGI_FORMAT_R16G16_FLOAT:
	case DXGI_FORMAT_R16G16_UNORM:
	case DXGI_FORMAT_R16G16_UINT:
	case DXGI_FORMAT_R16G16_SNORM:
	case DXGI_FORMAT_R16G16_SINT:
	case DXGI_FORMAT_R32_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT:
	case DXGI_FORMAT_R32_FLOAT:
	case DXGI_FORMAT_R32_UINT:
	case DXGI_FORMAT_R32_SINT:
	case DXGI_FORMAT_R24G8_TYPELESS:
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
	case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
	case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
	case DXGI_FORMAT_B8G8R8A8_UNORM:
	case DXGI_FORMAT_B8G8R8X8_UNORM:
		iRetByte = 32;
		break;
	case DXGI_FORMAT_R8G8_TYPELESS:
	case DXGI_FORMAT_R8G8_UNORM:
	case DXGI_FORMAT_R8G8_UINT:
	case DXGI_FORMAT_R8G8_SNORM:
	case DXGI_FORMAT_R8G8_SINT:
	case DXGI_FORMAT_R16_TYPELESS:
	case DXGI_FORMAT_R16_FLOAT:
	case DXGI_FORMAT_D16_UNORM:
	case DXGI_FORMAT_R16_UNORM:
	case DXGI_FORMAT_R16_UINT:
	case DXGI_FORMAT_R16_SNORM:
	case DXGI_FORMAT_R16_SINT:
	case DXGI_FORMAT_B5G6R5_UNORM:
	case DXGI_FORMAT_B5G5R5A1_UNORM:
		iRetByte = 16;
		break;
	case DXGI_FORMAT_R8_TYPELESS:
	case DXGI_FORMAT_R8_UNORM:
	case DXGI_FORMAT_R8_UINT:
	case DXGI_FORMAT_R8_SNORM:
	case DXGI_FORMAT_R8_SINT:
	case DXGI_FORMAT_A8_UNORM:
		iRetByte = 8;
		break;
		// Compressed format; http://msdn2.microsoft.com/en-us/library/bb694531(VS.85).aspx
	case DXGI_FORMAT_BC2_TYPELESS:
	case DXGI_FORMAT_BC2_UNORM:
	case DXGI_FORMAT_BC2_UNORM_SRGB:
	case DXGI_FORMAT_BC3_TYPELESS:
	case DXGI_FORMAT_BC3_UNORM:
	case DXGI_FORMAT_BC3_UNORM_SRGB:
	case DXGI_FORMAT_BC5_TYPELESS:
	case DXGI_FORMAT_BC5_UNORM:
	case DXGI_FORMAT_BC5_SNORM:
		iRetByte = 128;
		break;
		// Compressed format; http://msdn2.microsoft.com/en-us/library/bb694531(VS.85).aspx
	case DXGI_FORMAT_R1_UNORM:
	case DXGI_FORMAT_BC1_TYPELESS:
	case DXGI_FORMAT_BC1_UNORM:
	case DXGI_FORMAT_BC1_UNORM_SRGB:
	case DXGI_FORMAT_BC4_TYPELESS:
	case DXGI_FORMAT_BC4_UNORM:
	case DXGI_FORMAT_BC4_SNORM:
		iRetByte = 64;
		break;
		// Compressed format; http://msdn2.microsoft.com/en-us/library/bb694531(VS.85).aspx
	case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
		iRetByte = 32;
		break;
		// These are compressed, but bit-size information is unclear.
	case DXGI_FORMAT_R8G8_B8G8_UNORM:
	case DXGI_FORMAT_G8R8_G8B8_UNORM:
		iRetByte = 32;
		break;
	case DXGI_FORMAT_UNKNOWN:
	default:
		return -1;
	}

	return iRetByte / 8;
}

void CResMgr::AddInputLayout(DXGI_FORMAT _eFormat, const char* _strSemanticName)
{
	D3D11_INPUT_ELEMENT_DESC LayoutDesc = {};
	LayoutDesc.AlignedByteOffset = m_iLayoutOffset;
	LayoutDesc.Format = _eFormat;
	LayoutDesc.InputSlot = 0;
	LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	LayoutDesc.SemanticName = _strSemanticName;
	LayoutDesc.SemanticIndex = 0;
	m_vecLayoutInfo.push_back(LayoutDesc);

	m_iLayoutOffset += GetSizeofFormat(_eFormat);
}

void CResMgr::initSound()
{
	FMOD::System_Create(&CSound::g_pFMOD);

	if (nullptr == CSound::g_pFMOD)
		assert(nullptr);

	CSound::g_pFMOD->init(32, FMOD_DEFAULT, nullptr);
}
