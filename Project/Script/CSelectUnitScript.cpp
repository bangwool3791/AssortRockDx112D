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
	* ����� ũ�� ��Ұ� �Ÿ����� ����
	*/
	//[221030] �÷��̾��� z�� UnitSelectUIed z������ ��������, UnitSelectUIed�� ����.
	//UnitSelectUIed�� �Ⱥ����� ����
	//GetOwner()->Get_Parent()->Transform()->finaltick();
	//GetOwner()->Transform()->finaltick();
	//auto TransformT = GetOwner()->Transform()->GetWorldMat();
	//Vec3 vTargetPos = GetOwner()->Get_Parent()->Transform()->GetRelativePos();
	//CTransform* Transforma = GetOwner()->Get_Parent()->Transform();
	//Transform()->SetRelativePos(vTargetPos);
	//GetOwner()->Transform()->finaltick();
}
