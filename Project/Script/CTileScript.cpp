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
	, m_id{}
{
	SetName(L"CTileScript");
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
	if (KEY_PRESSED(KEY::LBTN))
	{
		Vec2 vMouse = CKeyMgr::GetInst()->GetMousePos();
		Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();
		Vec3  vCameraPos = m_pCameraObject->Transform()->GetRelativePos();
		float fCameraScale = m_pCameraObject->Camera()->GetOrthographicScale();

		Vec3 vTarget = Vec3{ vMouse.x - (vResolution.x / 2) , -vMouse.y + (vResolution.y / 2), 1.f };
		vTarget += vCameraPos;
		//Matrix matRot = XMMatrixRotationX(XM_PI * 0.25f);
		//vTarget = XMVector3TransformNormal(vTarget, matRot);
		vTarget *= fCameraScale;

		Matrix InvView;
		InvView = XMMatrixInverse(nullptr, g_transform.matView);
		vTarget = XMVector3Transform(vTarget, InvView);

		cout << "마우스 [x] " << vTarget.x << "[y] " << vTarget.y << "[z] " << vTarget.z << endl;

		GetOwner()->TileMap()->GetMesh()->SetTextureID(vTarget, m_id);

		//
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

void CTileScript::Initialize(void* _pAddr)
{
	CGameObject** pGameAddr = (CGameObject**)_pAddr;

	m_pCameraObject = pGameAddr[0];
	m_pMouseObject =  pGameAddr[1];
	m_vRenderResolution = CDevice::GetInst()->GetRenderResolution();
}

void CTileScript::SetTileInfo(UINT _ibyOption)
{
	m_id = _ibyOption; 
}