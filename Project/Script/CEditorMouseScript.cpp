#include "pch.h"
#include <Engine\CDevice.h>
#include <Engine\CResMgr.h>
#include <Engine\CCamera.h>
#include "CEditorMouseScript.h"

CEditorMouseScript::CEditorMouseScript()
	: CScript(EDITOR_MOUSESCRIPT)
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
		float px = 0.0f;
		float py = 0.0f;

		Vec2 vMouse = CKeyMgr::GetInst()->GetMousePos();
		Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();
		Vec3  vCameraPos = m_pCameraObejct->Transform()->GetRelativePos();
		float fCameraScale = m_pCameraObejct->Camera()->GetOrthographicScale();

		Vec3 vTarget = Vec3{ vMouse.x - (vResolution.x / 2) , -vMouse.y + (vResolution.y / 2), 1.f };
		vTarget += vCameraPos;
		vTarget *= fCameraScale;

		Matrix InvView;
		InvView = XMMatrixInverse(nullptr, g_transform.matView);
		vTarget = XMVector3TransformCoord(vTarget, InvView);

		Ptr<CMesh> mesh = CResMgr::GetInst()->FindRes<CMesh>(L"TerrainMesh");
		mesh->Collision(vTarget);

		cout << "d [x] " << vTarget.x << "[y] " << vTarget.y << "[z] " << vTarget.z << endl;
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