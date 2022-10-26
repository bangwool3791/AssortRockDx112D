#include "pch.h"
#include "CMissileScript.h"

CMissileScript::CMissileScript()
	: m_fSpeed(200.f)
{
}

CMissileScript::~CMissileScript()
{
}

void CMissileScript::tick()
{
	Vec3 vPos = Transform()->GetRelativePos();

	vPos.z = 1.f;
	vPos.y += m_fSpeed * DT;

	Transform()->SetRelativePos(vPos);
}