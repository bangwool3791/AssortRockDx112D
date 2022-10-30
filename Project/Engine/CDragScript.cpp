#include "pch.h"
#include "CDragScript.h"

#include "CDevice.h"
#include "CKeyMgr.h"
#include "CTransform.h"


CDragScript::CDragScript()
	:bClicked{false}
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

	/*
	* ���콺 ó�� ���� ����
	*/
	if (KEY_PRESSED(KEY::LBTN) && !bClicked)
	{
		/*
		* -�ػ�.��, +�ػ�.����
		*/
		StartPos = Vec2{ vMousePos.x - vRenderResolution.x /2.f, -vMousePos.y + vRenderResolution.y / 2.f };
		bClicked = true;
	}

	EndPos = Vec2{ vMousePos.x - vRenderResolution.x / 2.f , -vMousePos.y + vRenderResolution.y / 2.f };

	Vec2 SumPos = StartPos + EndPos;

	SumPos /= 2.f;
	vPos = Vec3{ SumPos.x,SumPos.y, 0.f };

	Vec3 vScale = Vec3{ std::fabsf(EndPos.x - StartPos.x), std::fabsf(EndPos.y - StartPos.y), 0.f };

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
}

void CDragScript::Overlap(CCollider2D* _pOther)
{

}

void CDragScript::EndOverlap(CCollider2D* _pOther)
{

}

//cout << "���콺 ��ġ {x} " << vMousePos.x << "{y} " << vMousePos.y << endl;
//cout << "���� {x} " << EndPos.x << " {y} " << EndPos.y << endl;
//cout << "��� {x} " << SumPos.x << " {y} " << SumPos.y << endl;