#include "pch.h"
#include "CCircleArrowScript.h"

#include <Engine\CDevice.h>
#include <Engine\CLevel.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CTransform.h>
#include <Engine\CTerrain.h>

#include <Engine\CInterfaceMgr.h>
#include <Script\CMouseScript.h>

//ºóÄ­ 
/*
	m_arrTab[RESOURCE][3].Texture = m_Atlas;
	m_arrTab[RESOURCE][3].vUV = vLeftTop;
	m_arrTab[RESOURCE][3].vSlice = vSlice;

	m_arrTab[RESOURCE][4].Texture = m_Atlas;
	m_arrTab[RESOURCE][4].vUV = vLeftTop;
	m_arrTab[RESOURCE][4].vSlice = vSlice;
*/

//µÚ·Î °¡±â
/*
	m_arrTab[RESOURCE][5].Texture = m_IconAtlas;
	m_arrTab[RESOURCE][5].vUV.x = 570.f / fIconWidth;
	m_arrTab[RESOURCE][5].vUV.y = 1260.f / fIconHeight;
	m_arrTab[RESOURCE][5].vSlice = Vec2{ 100.f / fIconWidth, 120.f / fIconHeight };
	m_arrTab[RESOURCE][5].vUV2 = Vec2{ 640.f / fIconWidth, 1400.f / fIconHeight };
	m_arrTab[RESOURCE][5].vSlice2 = Vec2{ 60.f / fIconWidth, 60.f / fIconHeight };
*/
CCircleArrowScript::CCircleArrowScript()
	:CScript{ SCRIPT_TYPE::CIRCLEARROWSCRIPT }
{
	SetName(L"CCircleArrowScript");
}

CCircleArrowScript::~CCircleArrowScript()
{
}


void CCircleArrowScript::begin()
{
}

void CCircleArrowScript::tick()
{
	if(GetOwner()->GetRenderComponent()->IsActive())
		m_fDeltaTime += DT;

	float fAngle = Transform()->GetRelativeRotation().y;
	fAngle += DT;
	Transform()->SetRelativeRotationY(fAngle);
}

void CCircleArrowScript::finaltick()
{
	if (m_fDeltaTime > 5.f)
	{
		GetOwner()->GetRenderComponent()->Deactivate();
		m_fDeltaTime = 0.f;
	}
}

void CCircleArrowScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CCircleArrowScript::Overlap(CCollider2D* _pOther)
{
}

void CCircleArrowScript::EndOverlap(CCollider2D* _pOther)
{
}
void CCircleArrowScript::SaveToFile(FILE* _File)
{

}

void CCircleArrowScript::LoadFromFile(FILE* _File)
{

}

