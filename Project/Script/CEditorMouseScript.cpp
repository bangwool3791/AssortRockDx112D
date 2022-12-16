#include "pch.h"
#include <Engine\CDevice.h>
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
	float fCameraScale = m_pCameraObejct->Camera()->GetOrthographicScale();
	Vec3  vCameraPos = m_pCameraObejct->Transform()->GetRelativePos();

	Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();
	Vec2 vMouse = CKeyMgr::GetInst()->GetMousePos();
	vMouse.x -= vResolution.x * 0.5f;
	vMouse.y = vMouse.y * -1.f + vResolution.y * 0.5f;
	vMouse *= fCameraScale;
	vMouse.x += vCameraPos.x;
	vMouse.y += vCameraPos.y;

	Transform()->SetRelativePos(Vec3{ vMouse.x, vMouse.y , 1.f});
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