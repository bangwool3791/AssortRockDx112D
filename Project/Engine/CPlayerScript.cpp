#include "pch.h"
#include "CPlayerScript.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"

#include "CTransform.h"
#include "CMaterial.h"
#include "CMeshRender.h"

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
	Vec3 vPos = Transform()->GetRelativePos();

	if (PRESS == KEY_PRESSED(KEY::LEFT))
	{
		vPos.x -= CTimeMgr::GetInst()->GetDeltaTime() * 1.f;
	}

	if (PRESS == KEY_PRESSED(KEY::RIGHT))
	{
		vPos.x += CTimeMgr::GetInst()->GetDeltaTime() * 1.f;
	}

	if (PRESS == KEY_PRESSED(KEY::UP))
	{
		vPos.y += CTimeMgr::GetInst()->GetDeltaTime() * 1.f;
	}

	if (PRESS == KEY_PRESSED(KEY::DOWN))
	{
		vPos.y -= CTimeMgr::GetInst()->GetDeltaTime() * 1.f;
	}

	Transform()->SetRelativePos(vPos);

	int a{};

	if (PRESS == KEY_PRESSED(KEY::NUM_1))
	{
		if (nullptr != MeshRender() && nullptr != MeshRender()->GetSharedMaterial())
		{
			MeshRender()->GetDynamicMaterial()->SetScalarParam(INT_0, &a);
		}1
	}

	if (PRESS == KEY_PRESSED(KEY::NUM_2))
	{
		a = 1;
		if (nullptr != MeshRender() && nullptr != MeshRender()->GetSharedMaterial())
		{
			MeshRender()->GetDynamicMaterial()->SetScalarParam(INT_0, &a);
		}
	}
}
