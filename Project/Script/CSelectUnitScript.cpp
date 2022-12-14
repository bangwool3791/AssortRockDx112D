#include "pch.h"
#include "CSelectUnitScript.h"

CSelectUnitScript::CSelectUnitScript()
	:CScript{SELECTUNITSCRIPT}
	,m_fSpeed{0.f}
{
}

CSelectUnitScript::~CSelectUnitScript()
{
}

void CSelectUnitScript::begin()
{
	GetOwner()->MeshRender()->GetCurMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4{0.f, 1.f, 0.f, 1.f});
	//GetOwner()->Transform()->SetIgnoreParentScale(true);
	GetOwner()->Transform()->SetRelativePos(0.f, -1.f, 1.2f);
}

void CSelectUnitScript::tick()
{
	/*
	* 행렬의 크기 요소가 거리에도 적용
	*/
	//[221030] 플레이어의 z값 UnitSelectUIed z값보다 작음에도, UnitSelectUIed가 보임.
	//UnitSelectUIed가 안보여야 정상
	//GetOwner()->Get_Parent()->Transform()->finaltick();
	//GetOwner()->Transform()->finaltick();
	//auto TransformT = GetOwner()->Transform()->GetWorldMat();
	//Vec3 vTargetPos = GetOwner()->Get_Parent()->Transform()->GetRelativePos();
	//CTransform* Transforma = GetOwner()->Get_Parent()->Transform();
	//Transform()->SetRelativePos(vTargetPos);
	//GetOwner()->Transform()->finaltick();
}
