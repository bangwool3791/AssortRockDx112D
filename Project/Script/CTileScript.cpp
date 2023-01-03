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

	//�������� ���콺 ������Ʈ�� Set���� �Է� ���� ��
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
		////���콺�� ��ǥ�� Tile ��ü �ȿ� �ְ� LBTN�̸�
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
		//		//cout << "���콺 ��ǥ " << m_vMousePos.x << " " << m_vMousePos.y << endl;
		//			//cout << "��ŷ " << tiles[i].vPos.x << " " << tiles[i].vPos.y << " " << tiles[i].iIndex << endl;
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
* ������ ��� �غ�
*/
bool CTileScript::Picking(const Vec3& vPos, UINT& iIndex)
{
	//const vector<tTile>&  tiles = m_pTileMap->GetTiles();

	///*
	//�ֺ� 4���� Ÿ�� ����
	//*/
	//Vec3	vPoint[4] = {

	//	Vec3(tiles[iIndex].vPos.x,					tiles[iIndex].vPos.y + (TILECZ * m_vTileSize.y  / 2.f), 0.f),	// 12
	//	Vec3(tiles[iIndex].vPos.x + (TILECX * m_vTileSize.x / 2.f),	0.f, tiles[iIndex].vPos.z),						// 3
	//	Vec3(tiles[iIndex].vPos.x,					tiles[iIndex].vPos.y - (TILECZ * m_vTileSize.y  / 2.f), 0.f),	// 6
	//	Vec3(tiles[iIndex].vPos.x - (TILECX * m_vTileSize.x / 2.f),	tiles[iIndex].vPos.y, tiles[iIndex].vPos.z),						// 9

	//};

	//// �ð�������� ���⺤�͸� ������

	//Vec3		vDir[4] = {

	//	vPoint[1] - vPoint[0],
	//	vPoint[2] - vPoint[1],
	//	vPoint[3] - vPoint[2],
	//	vPoint[0] - vPoint[3]

	//};

	//// �� ���⺤���� ������ ����

	//Vec3		vNormal[4] = {

	//	Vec3(-vDir[0].y, vDir[0].x, 0.f),
	//	Vec3(-vDir[1].y, vDir[1].x, 0.f),
	//	Vec3(-vDir[2].y, vDir[2].x, 0.f),
	//	Vec3(-vDir[3].y, vDir[3].x, 0.f)
	//};

	//// ���� ���� ���͵��� ����ȭ(��������) �����ش�.
	//for (int i = 0; i < 4; ++i)
	//	vNormal[i].Normalize();

	//// �� �������κ��� ���콺 ��ġ�� ���ϴ� ���⺤�͸� ���Ѵ�

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
	//	// ������ ���Դ�
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