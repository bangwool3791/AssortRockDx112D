#include "pch.h"
#include "CDragScript.h"

#include "CDevice.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"

#include "CSelectUnitScript.h"

#include "CTransform.h"
#include "CMaterial.h"
#include "CMeshRender.h"


CDragScript::CDragScript()
	:bClicked{false}
{
}

CDragScript::CDragScript(const CDragScript& rhs)
	:bClicked{ rhs.bClicked }
{

}

CDragScript::~CDragScript()
{
}


void CDragScript::begin()
{

}

void CDragScript::tick()
{
	Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();
	Vec3 vPos = Transform()->GetRelativePos();
	Vec2 vRenderResolution = CDevice::GetInst()->GetRenderResolution();

	if (KEY_PRESSED(KEY::LBTN) && !bClicked)
	{
		StartPos = Vec2{ vMousePos.x - vRenderResolution.x /2.f, -vMousePos.y + vRenderResolution.y / 2.f };
		bClicked = true;
	}

	EndPos = Vec2{ vMousePos.x - vRenderResolution.x / 2.f , -vMousePos.y + vRenderResolution.y / 2.f };

	Vec2 SumPos = StartPos + EndPos;

	SumPos /= 2.f;
	vPos = Vec3{ SumPos.x,SumPos.y, ALPHABLEND_Z_POS };

	Vec3 vScale = Vec3{ std::fabsf(StartPos.x - EndPos.x), std::fabsf(StartPos.y - EndPos.y), ALPHABLEND_Z_POS };

	if (KEY_PRESSED(KEY::LBTN) && bClicked)
	{
		Transform()->SetRelativePos(vPos);
		Transform()->SetRelativeScale(vScale);

	}

	if(KEY_RELEASE(KEY::LBTN) && bClicked)
	{
		bClicked = false;

		Transform()->SetRelativePos(vPos);
		Transform()->SetRelativeScale(vScale);
	}
}

void CDragScript::BeginOverlap(CCollider2D* _pOther)
{
	CGameObject* pObj = _pOther->GetOwner();
	Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"UnitSelectedPrefab");
	Instantiate(pUIPrefab->Instantiate(), pObj->Transform()->GetWorldMat());
}

void CDragScript::Overlap(CCollider2D* _pOther)
{

}

void CDragScript::EndOverlap(CCollider2D* _pOther)
{

}

//cout << "¸¶¿ì½º À§Ä¡ {x} " << vMousePos.x << "{y} " << vMousePos.y << endl;
//cout << "¶¼Áü {x} " << EndPos.x << " {y} " << EndPos.y << endl;
//cout << "°á°ú {x} " << SumPos.x << " {y} " << SumPos.y << endl;