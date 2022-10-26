#include "pch.h"
#include "CDragScript.h"

#include "CDevice.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"

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

		//cout << "마우스 위치 {x} " << vMousePos.x  << "{y} "<< vMousePos.y << endl;
		StartPos = Vec2{ vMousePos.x - vRenderResolution.x /2.f, -vMousePos.y + vRenderResolution.y / 2.f };

		//cout << "눌림 {x} " << StartPos.x << " {y} " << StartPos.y << endl;
		bClicked = true;
	}

	EndPos = Vec2{ vMousePos.x - vRenderResolution.x / 2.f , -vMousePos.y + vRenderResolution.y / 2.f };

	Vec2 SumPos = StartPos + EndPos;

	SumPos /= 2.f;
	//cout << "마우스 위치 {x} " << vMousePos.x << "{y} " << vMousePos.y << endl;
	//cout << "떼짐 {x} " << EndPos.x << " {y} " << EndPos.y << endl;
	//cout << "결과 {x} " << SumPos.x << " {y} " << SumPos.y << endl;
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
