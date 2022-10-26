#include "pch.h"
#include "CPlayerScript.h"

#include "CDevice.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"

#include "CTransform.h"
#include "CMaterial.h"
#include "CMeshRender.h"

#include "CPrefab.h"

CPlayerScript::CPlayerScript()
{
}

CPlayerScript::~CPlayerScript()
{
}


void CPlayerScript::begin()
{
}

void CPlayerScript::tick()
{
	Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();
	Vec3 vPos = Transform()->GetRelativePos();
	Vec2 vRenderResolution = CDevice::GetInst()->GetRenderResolution();
	static Vec3 vTarget;

	if ((vTarget - vPos).Length() > 0.5)
	{
		auto vMove = vTarget - vPos;

		//cout << "마우스 위치 " << vTarget.x << " " << vTarget.y << endl;

		vPos += vMove * DT;

		//cout << "플레이어 위치 " << vPos.x << " " << vPos.y << endl;
		Transform()->SetRelativePos(vPos);
	}

	if (KEY_PRESSED(KEY::RBTN) == PRESS)
	{
		/*
		* y좌표는 DirectX 좌표계에서, 아래로 갈 수록 음수이다.
		* 좌상단을 기준으로 마우스의 좌표를 계산했다.
		*/
		vTarget = Vec3{ vMousePos.x - (vRenderResolution.x / 2) , -vMousePos.y + (vRenderResolution.y / 2), 500.f };
		
		auto vMove = vTarget - vPos ;

		//cout << "마우스 위치 " << vTarget.x << " " << vTarget.y << endl;

		vPos += vMove * DT;

		//cout << "플레이어 위치 " << vPos.x << " " << vPos.y << endl;
		Transform()->SetRelativePos(vPos);
	}

	if (KEY_PRESSED(KEY::SPACE) == PRESS)
	{
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.y += Transform()->GetRelativeScale().y / 2;
		
		Ptr<CPrefab> pMissilePrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"MissilePrefab");
		Instantiate(pMissilePrefab->Instantiate(), vPos);
	}
}
