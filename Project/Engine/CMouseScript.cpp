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

/*
* Script
*/

/*
* Component
*/
#include "CTransform.h"
#include "CMaterial.h"
#include "CMeshRender.h"
#include "CScript.h"
#include "CPlayerScript.h"
CMouseScript::CMouseScript()
	:bClicked{ false }
{
}

CMouseScript::~CMouseScript()
{
}


void CMouseScript::begin()
{

}

//if ((vTarget - vPos).Length() > 0.5)
//{
//	auto vMove = vTarget - vPos;
//	vPos += vMove * DT;
//	Transform()->SetRelativePos(vPos);
//}

//if (KEY_PRESSED(KEY::RBTN) == PRESS)
//{
//	/*
//	* y좌표는 DirectX 좌표계에서, 아래로 갈 수록 음수이다.
//	* 좌상단을 기준으로 마우스의 좌표를 계산했다.
//	*/
//	vTarget = Vec3{ vMousePos.x - (vRenderResolution.x / 2) , -vMousePos.y + (vRenderResolution.y / 2), vPos.z };
//	
//	auto vMove = vTarget - vPos ;
//	vPos += vMove * DT;
//	Transform()->SetRelativePos(vPos);
//}

//if (KEY_PRESSED(KEY::SPACE) == PRESS)
//{
//	Vec3 vPos = Transform()->GetRelativePos();
//	vPos.y += Transform()->GetRelativeScale().y / 2;
//	
//	Ptr<CPrefab> pMissilePrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"MissilePrefab");
//	Instantiate(pMissilePrefab->Instantiate(), vPos);
//}

void CMouseScript::tick()
{
	static Vec3 vTarget;
	Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();
	Vec2 vRenderResolution = CDevice::GetInst()->GetRenderResolution();

	if (KEY_PRESSED(KEY::RBTN))
	{
		vTarget = Vec3{ vMousePos.x - (vRenderResolution.x / 2) , -vMousePos.y + (vRenderResolution.y / 2), 1.f };

		CLayer* pLayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(1);

		const vector<CGameObject*>& objects = pLayer->GetParentObjects();

		for (auto iter{ objects.begin() }; iter != objects.end(); ++iter)
		{
			if (!lstrcmp(L"SelectUnit", (*iter)->GetName().c_str()))
			{
				(*iter)->GetScript<CPlayerScript>()->Set_Target(vTarget);
			}
		}
	}

	if (KEY_PRESSED(KEY::LBTN) && !bClicked)
	{
		/*
		* child로 변경해서, Layer 별로 가져 와 볼 것.
		*/
		CLayer* pLayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(1);

		const vector<CGameObject*>& objects2 = pLayer->GetParentObjects();

		for (auto iter{ objects2.begin() }; iter != objects2.end(); ++iter)
		{
			if (!lstrcmp(L"SelectUnit", (*iter)->GetName().c_str()))
			{
				(*iter)->SetName(L"Player");
			}

			const vector<CGameObject*>& childs = (*iter)->GetChilds();

			for (auto iter2{ childs.begin() }; iter2 != childs.end(); ++iter2)
			{
				if (!lstrcmp(L"UnitSelectUI", (*iter2)->GetName().c_str()))
				{
					(*iter2)->SetDead();
					break;
				}
			}
		}

		Ptr<CPrefab> pPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"MouseDragPrefab");
		Instantiate(pPrefab->Instantiate(), Vec3{ 0.f, 0.f, 0.f });
		bClicked = true;
	}

	if (KEY_PRESSED(KEY::LBTN) && bClicked)
	{
	}

	if (KEY_RELEASE(KEY::LBTN) && bClicked)
	{
		bClicked = false;

		CLayer* pLayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(0);

		const vector<CGameObject*>& objects = pLayer->GetParentObjects();

		for (auto iter{ objects.begin() }; iter != objects.end(); ++iter)
		{
			if (!lstrcmp(L"MouseDrag", (*iter)->GetName().c_str()))
			{
				(*iter)->SetDead();
				break;
			}
		}

		pLayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(1);
		/*
		* 게임 오브젝트 상태 컴포넌트 추가
		* 상태 읽어와서, UI 벡터에 추가.
		*/
		const vector<CGameObject*>& objects2 = pLayer->GetParentObjects();
		for (auto iter{ objects2.begin() }; iter != objects2.end(); ++iter)
		{
			if (0 < (*iter)->GetScript<CPlayerScript>()->IsDrag())
			{
				(*iter)->SetName(L"SelectUnit");
				Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"UnitSelectUIPrefab");
				Instantiate(pUIPrefab->Instantiate(), *iter, 0);
			}
		}
	}
}

void CMouseScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CMouseScript::Overlap(CCollider2D* _pOther)
{

}

void CMouseScript::EndOverlap(CCollider2D* _pOther)
{

}
