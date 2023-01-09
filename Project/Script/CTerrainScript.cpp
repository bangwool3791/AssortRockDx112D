#include "pch.h"
#include "CTerrainScript.h"

#include <Engine\CDevice.h>
#include <Engine\CLevel.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CTransform.h>
#include <Engine\CTerrain.h>


CTerrainScript::CTerrainScript()
	:CScript{ TERRAINSCRIPT }
	, m_id{}
{
	SetName(L"CTerrainScript");
}

CTerrainScript::~CTerrainScript()
{
}


void CTerrainScript::begin()
{
}
//���̽� ��ŷ���� ����
void CTerrainScript::tick()
{
	static Vec3 vMouse;

	//�������� ���콺 ������Ʈ�� Set���� �Է� ���� ��
	CTransform* pTransform = GetOwner()->Transform();
	Vec3 vPos = pTransform->GetRelativePos();
	Vec3 vScale = pTransform->GetRelativeScale();

	Vec3 vLeftBottom = vPos - vScale;
	Vec3 vRightTop = vPos + vScale;

	if (KEY_PRESSED(KEY::LBTN))
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
		cout << "���콺 [x] " << rayorigin.x << "[y] " << rayorigin.y << "[z] " << rayorigin.z << endl;

		GetOwner()->Terrain()->GetMesh()->SetTextureID(ray, m_id);

		//
	}
}

void CTerrainScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CTerrainScript::Overlap(CCollider2D* _pOther)
{
}

void CTerrainScript::EndOverlap(CCollider2D* _pOther)
{
}

void CTerrainScript::Initialize(void* _pAddr)
{
	CGameObject** pGameAddr = (CGameObject**)_pAddr;

	m_pCameraObject = pGameAddr[0];
	m_pMouseObject = pGameAddr[1];
	m_vRenderResolution = CDevice::GetInst()->GetRenderResolution();
}

void CTerrainScript::SetTileInfo(UINT _ibyOption)
{
	m_id = _ibyOption;
}