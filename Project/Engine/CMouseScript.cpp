#include "pch.h"
#include "CMouseScript.h"

#include "CDevice.h"

#include "CLayer.h"
#include "CLevel.h"
#include "CLevelMgr.h"
/*
* indepdent mgr
*/
#include "CUIMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CCollisionMgr.h"

/*
* Script
* 
*/
#include "CScript.h"
#include "CPlayerScript.h"
/*
* Component
*/
#include "CCollider2D.h"
#include "CTransform.h"
#include "CMaterial.h"
#include "CMeshRender.h"
CMouseScript::CMouseScript()
	:bClicked{ false }
{
}

CMouseScript::~CMouseScript()
{
}


void CMouseScript::begin()
{
	GetOwner()->Transform()->SetRelativeScale(Vec3{ 3.f, 5.f, 0.f });
}

void CMouseScript::tick()
{
	static Vec3 vTarget;
	static Vec2& vMousePos = CKeyMgr::GetInst()->GetMousePos_();
	const static Vec2 vRenderResolution = CDevice::GetInst()->GetRenderResolution();

	vTarget = Vec3{ vMousePos.x - (vRenderResolution.x / 2) , -vMousePos.y + (vRenderResolution.y / 2), 1.f };
	GetOwner()->Transform()->SetRelativePos(Vec3{ vTarget.x, vTarget.y, 1.f });
}

void CMouseScript::finaltick()
{
	static Vec3 vTarget;
	static Vec2& vMousePos = CKeyMgr::GetInst()->GetMousePos_();
	const static Vec2 vRenderResolution = CDevice::GetInst()->GetRenderResolution();

	if (KEY_PRESSED(KEY::RBTN))
	{
		vTarget = Vec3{ vMousePos.x - (vRenderResolution.x / 2) , -vMousePos.y + (vRenderResolution.y / 2), 1.f };

		const vector<CGameObject*>& objects = CUIMgr::GetInst()->Get_Objects(UI_TYPE::GAMEOBJECT);
		for (auto iter{ objects.begin() }; iter != objects.end(); ++iter)
		{
			(*iter)->GetScript<CPlayerScript>()->Set_Target(vTarget);
		}
	}

	if (KEY_PRESSED(KEY::LBTN) && !bClicked)
	{
		/*
		* child로 변경해서, Layer 별로 가져 와 볼 것.
		*/
		/*
		* 드래그 UNIT CLEAR
		*/
		CUIMgr::GetInst()->DeleteUI(UI_TYPE::UNIT_UI);
		CUIMgr::GetInst()->Clear_Objects(UI_TYPE::GAMEOBJECT);

		Ptr<CPrefab> pPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"MouseDragPrefab");
		CGameObject* pDrag = pPrefab->Instantiate();
		Instantiate(pDrag, Vec3{ 0.f, 0.f, 0.f }, 31);

		CUIMgr::GetInst()->AddUI(pDrag, UI_TYPE::DRAG);
		//마우스 클릭 충돌 활성화
		GetOwner()->Collider2D()->ReleasePause();
		bClicked = true;
	}

	if (KEY_RELEASE(KEY::LBTN) && bClicked)
	{
		bClicked = false;
		CUIMgr::GetInst()->DeleteUI(UI_TYPE::DRAG);
	}
}

void CMouseScript::BeginOverlap(CCollider2D* _pOther)
{
	CUIMgr::GetInst()->DeleteUI(UI_TYPE::UNIT_UI);
	CUIMgr::GetInst()->Clear_Objects(UI_TYPE::GAMEOBJECT);
}

void CMouseScript::Overlap(CCollider2D* _pOther)
{
	Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"UnitSelectUIPrefab");
	CGameObject* pUnit_UI = pUIPrefab->Instantiate();
	Instantiate(pUnit_UI, _pOther->GetOwner(), 0);

	CUIMgr::GetInst()->AddUI(_pOther->GetOwner(), UI_TYPE::GAMEOBJECT);
	CUIMgr::GetInst()->AddUI(pUnit_UI, UI_TYPE::UNIT_UI);
}

void CMouseScript::EndOverlap(CCollider2D* _pOther)
{
	GetOwner()->Collider2D()->SetPause();
}
