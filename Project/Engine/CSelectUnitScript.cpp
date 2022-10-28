#include "pch.h"
#include "CSelectUnitScript.h"

CSelectUnitScript::CSelectUnitScript()
	:m_fSpeed{0.f}
{
}

CSelectUnitScript::~CSelectUnitScript()
{
}

void CSelectUnitScript::begin()
{
}

void CSelectUnitScript::tick()
{
	Vec3 vTargetPos = GetOwner()->Get_Parent()->Transform()->GetRelativePos();

	Transform()->SetRelativePos(vTargetPos);
}
