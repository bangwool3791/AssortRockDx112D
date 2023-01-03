#include "pch.h"
#include "CTileScript.h"

#include <Engine\CDevice.h>
#include <Engine\CLevel.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CTransform.h>
#include <Engine\CTileMap.h>


CTileScript::CTileScript()
	:CScript{ TILESCRIPT}
	,m_tTileInfo{}
{
	SetName(L"CTileScript");
}

CTileScript::~CTileScript()
{
}


void CTileScript::begin()
{
}

void CTileScript::tick()
{
	static Vec3 vMouse;

	//에디터의 마우스 오브젝트를 Set으로 입력 받은 뒤
	CTransform* pTransform = GetOwner()->Transform();
	Vec3 vPos   = pTransform->GetRelativePos();
	Vec3 vScale = pTransform->GetRelativeScale();

	Vec3 vLeftBottom = vPos - vScale;
	Vec3 vRightTop   = vPos + vScale;
	if (KEY_PRESSED(KEY::LBTN))
	{
		//m_vMousePos = m_pMouseObject->Transform()->GetRelativePos();

		////if (vLeftBottom <= m_vMousePos && m_vMousePos <= vRightTop)
		////{
		////마우스의 좌표가 Tile 객체 안에 있고 LBTN이면
		//	if (m_vTileSize != m_pTileMap->GetTileSize())
		//		m_vTileSize = m_pTileMap->GetTileSize();

		//	vector<tTile>& tiles = m_pTileMap->GetTiles();

		//	UINT startX = (UINT)(m_vMousePos.x / (TILECX * m_vTileSize.x));
		//	UINT startZ = (UINT)(m_vMousePos.y / (TILECZ * m_vTileSize.y * 0.5f));
		//	UINT StartIndex = startX + startZ * TILEX;

		//	UINT endX = (UINT)(m_vRenderResolution.x / (TILECX * m_vTileSize.x));
		//	UINT endY = (UINT)(m_vRenderResolution.y / (TILECZ * m_vTileSize.y * 0.5f));
		//	UINT EndIndex = endX + endY * endX;

		//	for (UINT i{ StartIndex }; i < StartIndex + EndIndex; ++i)
		//	{
		//		vMouse.x = m_vMousePos.x;
		//		vMouse.y = m_vMousePos.y;

		//		if (Picking(vMouse, i))
		//		{
		//		//cout << "마우스 좌표 " << m_vMousePos.x << " " << m_vMousePos.y << endl;
		//			//cout << "피킹 " << tiles[i].vPos.x << " " << tiles[i].vPos.y << " " << tiles[i].iIndex << endl;
		//			tiles[i].ibyOption = m_tTileInfo.ibyOption;
		//			break;
		//		}
		//	}
		////}
	}
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

/*
* 쓰레드 사용 준비
*/
bool CTileScript::Picking(const Vec3& vPos, UINT& iIndex)
{
	//const vector<tTile>&  tiles = m_pTileMap->GetTiles();

	///*
	//주변 4개의 타일 벡터
	//*/
	//Vec3	vPoint[4] = {

	//	Vec3(tiles[iIndex].vPos.x,					tiles[iIndex].vPos.y + (TILECZ * m_vTileSize.y  / 2.f), 0.f),	// 12
	//	Vec3(tiles[iIndex].vPos.x + (TILECX * m_vTileSize.x / 2.f),	0.f, tiles[iIndex].vPos.z),						// 3
	//	Vec3(tiles[iIndex].vPos.x,					tiles[iIndex].vPos.y - (TILECZ * m_vTileSize.y  / 2.f), 0.f),	// 6
	//	Vec3(tiles[iIndex].vPos.x - (TILECX * m_vTileSize.x / 2.f),	tiles[iIndex].vPos.y, tiles[iIndex].vPos.z),						// 9

	//};

	//// 시계방향으로 방향벡터를 만들자

	//Vec3		vDir[4] = {

	//	vPoint[1] - vPoint[0],
	//	vPoint[2] - vPoint[1],
	//	vPoint[3] - vPoint[2],
	//	vPoint[0] - vPoint[3]

	//};

	//// 각 방향벡터의 법선을 구함

	//Vec3		vNormal[4] = {

	//	Vec3(-vDir[0].y, vDir[0].x, 0.f),
	//	Vec3(-vDir[1].y, vDir[1].x, 0.f),
	//	Vec3(-vDir[2].y, vDir[2].x, 0.f),
	//	Vec3(-vDir[3].y, vDir[3].x, 0.f)
	//};

	//// 구한 법선 벡터들을 정규화(단위벡터) 시켜준다.
	//for (int i = 0; i < 4; ++i)
	//	vNormal[i].Normalize();

	//// 각 정점으로부터 마우스 위치를 향하는 방향벡터를 구한다

	//Vec3	vMouseDir[4] = {

	//	vPos - vPoint[0],
	//	vPos - vPoint[1],
	//	vPos - vPoint[2],
	//	vPos - vPoint[3]
	//};

	//for (int i = 0; i < 4; ++i)
	//	vNormal[i].Normalize();

	//for (int i = 0; i < 4; ++i)
	//{
	//	// 예각이 나왔다
	//	if (0.f < vNormal[i].Dot(vMouseDir[i]))
	//		return false;
	//}

	//return true;
	return false;
}

void CTileScript::Initialize(void* _pAddr)
{
	CGameObject** pGameAddr = (CGameObject**)_pAddr;

	m_pCameraObject = pGameAddr[0];
	m_pMouseObject =  pGameAddr[1];

	m_pTileMap = static_cast<CTileMap*>(GetOwner()->GetRenderComponent());
	m_vRenderResolution = CDevice::GetInst()->GetRenderResolution();
}

void CTileScript::SetTileInfo(UINT _ibyOption)
{
	m_tTileInfo.ibyOption = _ibyOption; 

	//vector<tTile>& tiles = m_pTileMap->GetTiles();

	//tiles[0].ibyOption = _ibyOption;
}