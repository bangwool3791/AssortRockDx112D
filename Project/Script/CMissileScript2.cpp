#include "pch.h"
#include "CMissileScript2.h"

CMissileScript2::CMissileScript2()
	: CScript(MISSILESCRIPT2)
	, m_fSpeed(200.f)
{
}

CMissileScript2::~CMissileScript2()
{
}

void CMissileScript2::tick()
{
	Vec3 vPos = Transform()->GetRelativePos();

	vPos.z = 1.f;
	vPos.y += m_fSpeed * DT;

	Transform()->SetRelativePos(vPos);
}