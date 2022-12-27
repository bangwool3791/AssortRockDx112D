#include "pch.h"
#include "CMouseScript.h"

#include "CPlayerScript.h"
#include <Engine\CDevice.h>
#include <Engine\CLayer.h>
#include <Engine\CLevel.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CUIMgr.h>
#include <Engine\CCollisionMgr.h>
#include <Engine\CCollider2D.h>
#include <Engine\CTransform.h>
#include <Engine\CMaterial.h>
#include <Engine\CMeshRender.h>

CMouseScript::CMouseScript()
	:CScript{ MOUSESCRIPT }
	,bClicked{ false }
{
	SetName(L"CMouseScript");
}

CMouseScript::~CMouseScript()
{
}


void CMouseScript::begin()
{
	GetOwner()->Transform()->SetRelativeScale(Vec3{ 3.f, 5.f, 0.f });
	m_pCamera = CLevelMgr::GetInst()->GetCurLevel()->FindParentObjectByName(L"MainCamera");
	GetOwner()->Collider2D()->SetPause();
}

void CMouseScript::tick()
{
	if (m_pCamera)
	{
		m_fCameraScale = m_pCamera->Camera()->GetOrthographicScale();
		m_vCameraPos   = m_pCamera->Transform()->GetRelativePos();
	}

	m_vMousePos = CKeyMgr::GetInst()->GetMousePos();
	m_vRenderResolution = CDevice::GetInst()->GetRenderResolution();

	m_vTarget = Vec3{ m_vMousePos.x - (m_vRenderResolution.x / 2) , -m_vMousePos.y + (m_vRenderResolution.y / 2), 1.f };
	m_vTarget *= m_fCameraScale;
	m_vTarget += m_vCameraPos;
	GetOwner()->Transform()->SetRelativePos(Vec3{ m_vTarget.x, m_vTarget.y, 1.f });
}

void CMouseScript::finaltick()
{
	if (KEY_PRESSED(KEY::RBTN))
	{
		//툴에서 마우스 관련 객체 생성으로 트리 갱신되어 주석처리
		//const vector<CGameObject*>& objects = CUIMgr::GetInst()->Get_Objects(UI_TYPE::GAMEOBJECT);
		//for (auto iter{ objects.begin() }; iter != objects.end(); ++iter)
		//{
		//	if((*iter)->GetScript<CPlayerScript>())
		//		(*iter)->GetScript<CPlayerScript>()->Set_Target(m_vTarget);
		//}
	}

	if (KEY_PRESSED(KEY::LBTN) && !bClicked)
	{
		/*
		* child로 변경해서, Layer 별로 가져 와 볼 것.
		*/
		/*
		* 드래그 UNIT CLEAR
		*/
		//툴에서 마우스 관련 객체 생성으로 트리 갱신되어 주석처리
		//Ptr<CPrefab> pPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"MouseDragPrefab");
		//CGameObject* pDrag = pPrefab->Instantiate();
		//Instantiate(pDrag, Vec3{ 0.f, 0.f, 0.f }, 31);

		//CUIMgr::GetInst()->AddUI(pDrag, UI_TYPE::DRAG);
		//////마우스 클릭 충돌 활성화
		//GetOwner()->Collider2D()->ReleasePause();
		//bClicked = true;
	}

	if (KEY_RELEASE(KEY::LBTN) && bClicked)
	{
		//bClicked = false;
		//CUIMgr::GetInst()->DeleteUI(UI_TYPE::DRAG);
	}
}

void CMouseScript::BeginOverlap(CCollider2D* _pOther)
{
	if (_pOther->GetOwner()->GetName() == L"Player")
	{
		CUIMgr::GetInst()->DeleteUI(UI_TYPE::UNIT_UI);
		CUIMgr::GetInst()->Clear_Objects(UI_TYPE::GAMEOBJECT);
	}
}

void CMouseScript::Overlap(CCollider2D* _pOther)
{
	if (_pOther->GetOwner()->GetName() == L"Player")
	{
		//CUIMgr::GetInst()->DeleteUI(UI_TYPE::UNIT_UI);
		//CUIMgr::GetInst()->Clear_Objects(UI_TYPE::GAMEOBJECT);
		//Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"UnitSelectUIPrefab");
		//CGameObject* pUnit_UI = pUIPrefab->Instantiate();
		//Instantiate(pUnit_UI, _pOther->GetOwner());

		//CUIMgr::GetInst()->AddUI(_pOther->GetOwner(), UI_TYPE::GAMEOBJECT);
		//CUIMgr::GetInst()->AddUI(pUnit_UI, UI_TYPE::UNIT_UI);
	}
}

void CMouseScript::EndOverlap(CCollider2D* _pOther)
{
	GetOwner()->Collider2D()->SetPause();
}
