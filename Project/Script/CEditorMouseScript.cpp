#include "pch.h"
#include <Engine\CDevice.h>
#include <Engine\CResMgr.h>
#include <Engine\CCamera.h>
#include "CEditorMouseScript.h"

CEditorMouseScript::CEditorMouseScript()
	: CScript(EDITORMOUSESCRIPT)
{
	SetName(L"CEditorMouseScript");
}

CEditorMouseScript::~CEditorMouseScript()
{
}


void CEditorMouseScript::begin()
{
}

void CEditorMouseScript::tick()
{
	if (KEY_PRESSED(KEY::LBTN))
	{
		Vec2 p = CKeyMgr::GetInst()->GetMousePos();
		Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();

		p.x = (2.0f * p.x) / vResolution.x - 1.0f;
		p.y = 1.0f - (2.0f * p.y) / vResolution.y;

		XMVECTOR det; //Determinant, needed for matrix inverse function call
		Vector3 origin = Vector3(p.x, p.y, -1);
		Vector3 faraway = Vector3(p.x, p.y, 1);

		XMMATRIX invViewProj = XMMatrixInverse(&det, g_transform.matView * g_transform.matProj);
		Vector3 rayorigin = XMVector3Transform(origin, invViewProj);
		Vector3 rayend = XMVector3Transform(faraway, invViewProj);
		Vector3 raydirection = rayend - rayorigin;
		raydirection.Normalize();
		m_Ray.position = rayorigin;
		m_Ray.direction = raydirection;
		cout << "¸¶¿ì½º [x] " << rayorigin.x << "[y] " << rayorigin.y << "[z] " << rayorigin.z << endl;
	}
}

void CEditorMouseScript::finaltick()
{
	Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();
	Vec2 vMouse = CKeyMgr::GetInst()->GetMousePos();
	vMouse.x -= vResolution.x * 0.5f;
	vMouse.y += vResolution.y * 0.5f;

	Transform()->SetRelativePos(Vec3{ vMouse.x, vMouse.y , 1.f });
}

void CEditorMouseScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CEditorMouseScript::Overlap(CCollider2D* _pOther)
{
}

void CEditorMouseScript::EndOverlap(CCollider2D* _pOther)
{
}

void CEditorMouseScript::Initialize(void* pAddr)
{
	CGameObject* pGameObject = (CGameObject*)pAddr;
	m_pCameraObejct = pGameObject;
}