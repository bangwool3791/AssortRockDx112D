#include "pch.h"
#include "InterfaceScript.h"

#include <Engine\CDevice.h>
#include <Engine\CLevel.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CTransform.h>
#include <Engine\CTerrain.h>


InterfaceScript::InterfaceScript()
	:CScript{ INTERFACESCRIPT }
	, m_id{}
{
	SetName(L"InterfaceScript");
}

InterfaceScript::~InterfaceScript()
{
}


void InterfaceScript::begin()
{
	Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Interface\\Atlas1_LQ.dds");
	float width = pTex->GetWidth();
	float height = pTex->GetHeight();
	GetOwner()->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);
	int a = 1;
	GetOwner()->GetRenderComponent()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, &a);
	Vec2 vLeftTop = { 0.f, 0.f };
	GetOwner()->GetRenderComponent()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_0, &vLeftTop);
	Vec2 vSlice = { 1350.f / width, 213.f / height };
	GetOwner()->GetRenderComponent()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_1, &vSlice);
}

void InterfaceScript::tick()
{

}

void InterfaceScript::BeginOverlap(CCollider2D* _pOther)
{
}

void InterfaceScript::Overlap(CCollider2D* _pOther)
{
}

void InterfaceScript::EndOverlap(CCollider2D* _pOther)
{
}
