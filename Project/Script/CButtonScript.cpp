#include "pch.h"
#include "CButtonScript.h"

#include <Engine\CDevice.h>
#include <Engine\CLevel.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CTransform.h>
#include <Engine\CTerrain.h>

#include <Engine\CInterfaceMgr.h>
#include <Script\CMouseScript.h>

CButtonScript::CButtonScript()
	:CScript{ SCRIPT_TYPE::BUTTONSCRIPT }
	, m_vMousePos{}
	, m_bClicked{false}
	, m_iColumn{}
{
	SetName(L"CButtonScript");
}

CButtonScript::~CButtonScript()
{
}


void CButtonScript::begin()
{
	m_pLevelMouseObject = CLevelMgr::GetInst()->GetCurLevel()->FindParentObjectByName(L"MouseObject");

	m_Atlas = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Interface\\Atlas1_LQ.dds");
	m_IconAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Interface\\Icons.png");
	m_AtlasAlpha = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Mask\\TileMask.png");

	Ptr<CTexture> TexPortraits = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Interface\\Portraits.dds");
	float fPortraitWidth = TexPortraits->GetWidth();
	float fPortraitHeight = TexPortraits->GetHeight();

	float width = m_Atlas->GetWidth();
	float height = m_Atlas->GetHeight();
	GetOwner()->MeshRender()->GetCurMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_Atlas);
	int a = 1;
	GetOwner()->GetRenderComponent()->GetCurMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, &a);
	Vec2 vLeftTop = { 1960.f / width, 660.f / height };
	GetOwner()->GetRenderComponent()->GetCurMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_0, &vLeftTop);
	Vec2 vSlice = { 100.f / width, 100.f / height };
	GetOwner()->GetRenderComponent()->GetCurMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_1, &vSlice);

	float fIconWidth = m_IconAtlas->GetWidth();
	float fIconHeight = m_IconAtlas->GetHeight();

	m_arrTab[0][0].Texture = m_IconAtlas;
	m_arrTab[0][0].vUV.x = 275.f / fIconWidth;
	m_arrTab[0][0].vUV.y = 830.f / fIconHeight;
	m_arrTab[0][0].vSlice = Vec2{ 150.f / fIconWidth, 140.f / fIconHeight };
	m_arrTab[0][0].vUV2 = Vec2{ 640.f / fIconWidth, 1400.f / fIconHeight };
	m_arrTab[0][0].vSlice2 = Vec2{ 60.f / fIconWidth, 60.f / fIconHeight };

	m_arrTab[0][1].Texture = m_IconAtlas;
	m_arrTab[0][1].vUV.x = 0.f / fIconWidth;
	m_arrTab[0][1].vUV.y = 970.f / height;
	m_arrTab[0][1].vSlice = Vec2{ 150.f / fIconWidth, 140.f / fIconHeight };
	m_arrTab[0][1].vUV2 = Vec2{ 640.f / fIconWidth, 1400.f / fIconHeight };
	m_arrTab[0][1].vSlice2 = Vec2{ 60.f / fIconWidth, 60.f / fIconHeight };

	m_arrTab[0][2].Texture = m_IconAtlas;
	m_arrTab[0][2].vUV.x = 860.f / fIconWidth;
	m_arrTab[0][2].vUV.y = 850.f / fIconHeight;
	m_arrTab[0][2].vSlice = Vec2{ 140.f / fIconWidth, 140.f / fIconHeight };
	m_arrTab[0][2].vUV2 = Vec2{ 640.f / fIconWidth, 1400.f / fIconHeight };
	m_arrTab[0][2].vSlice2 = Vec2{ 60.f / fIconWidth, 60.f / fIconHeight };

	m_arrTab[0][3].Texture = m_IconAtlas;
	m_arrTab[0][3].vUV.x = 560.f / fIconWidth;
	m_arrTab[0][3].vUV.y = 860.f / fIconHeight;
	m_arrTab[0][3].vSlice = Vec2{ 150.f / fIconWidth, 140.f / fIconHeight };
	m_arrTab[0][3].vUV2 = Vec2{ 640.f / fIconWidth, 1400.f / fIconHeight };
	m_arrTab[0][3].vSlice2 = Vec2{ 60.f / fIconWidth, 60.f / fIconHeight };

	m_arrTab[0][4].Texture = m_Atlas;
	m_arrTab[0][4].vUV = vLeftTop;
	m_arrTab[0][4].vSlice = vSlice;

	m_arrTab[0][5].Texture = m_Atlas;
	m_arrTab[0][5].vUV = vLeftTop;
	m_arrTab[0][5].vSlice = vSlice;

	//
	m_arrTab[1][0].Texture = TexPortraits;
	m_arrTab[1][0].vUV.x = 700.f / fPortraitWidth;
	m_arrTab[1][0].vUV.y = 860.f / fPortraitHeight;
	m_arrTab[1][0].vSlice = Vec2{ 100.f / fPortraitWidth, 100.f / fPortraitHeight };
	m_arrTab[1][0].vUV2 = Vec2{ 640.f / fIconWidth, 1400.f / fIconHeight };
	m_arrTab[1][0].vSlice2 = Vec2{ 60.f / fIconWidth, 60.f / fIconHeight };
			 
	m_arrTab[1][1].Texture = TexPortraits;
	m_arrTab[1][1].vUV.x = 1420.f / fPortraitWidth;
	m_arrTab[1][1].vUV.y = 0.f / fPortraitHeight;
	m_arrTab[1][1].vSlice = Vec2{ 150.f / fPortraitWidth, 140.f / fPortraitHeight };
	m_arrTab[1][1].vUV2 = Vec2{ 640.f / fIconWidth, 1400.f / fIconHeight };
	m_arrTab[1][1].vSlice2 = Vec2{ 60.f / fIconWidth, 60.f / fIconHeight };
			 
	m_arrTab[1][2].Texture = TexPortraits;
	m_arrTab[1][2].vUV.x = 132.f / fPortraitWidth;
	m_arrTab[1][2].vUV.y = 1360.f/ fPortraitHeight;
	m_arrTab[1][2].vSlice = Vec2{ 150.f / fPortraitWidth, 140.f / fPortraitHeight };
	m_arrTab[1][2].vUV2 = Vec2{ 640.f / fIconWidth, 1400.f / fIconHeight };
	m_arrTab[1][2].vSlice2 = Vec2{ 60.f / fIconWidth, 60.f / fIconHeight };
			 
	m_arrTab[1][3].Texture = m_Atlas;
	m_arrTab[1][3].vUV = vLeftTop;
	m_arrTab[1][3].vSlice = vSlice;

	m_arrTab[1][4].Texture = m_Atlas;
	m_arrTab[1][4].vUV = vLeftTop;
	m_arrTab[1][4].vSlice = vSlice;
			 
	m_arrTab[1][5].Texture = m_IconAtlas;
	m_arrTab[1][5].vUV.x = 570.f / fIconWidth;
	m_arrTab[1][5].vUV.y = 1260.f / fIconHeight;
	m_arrTab[1][5].vSlice = Vec2{ 100.f / fIconWidth, 120.f / fIconHeight };
	m_arrTab[1][5].vUV2 = Vec2{ 640.f / fIconWidth, 1400.f / fIconHeight };
	m_arrTab[1][5].vSlice2 = Vec2{ 60.f / fIconWidth, 60.f / fIconHeight };

	CInterfaceMgr::GetInst()->AddTapButton(GetOwner());
}

void CButtonScript::tick()
{
	GetOwner()->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_arrTab[m_iColumn][m_iRow].Texture);
	GetOwner()->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_1, m_IconAtlas);
	GetOwner()->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_2, m_AtlasAlpha);
	GetOwner()->GetRenderComponent()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_0, &m_arrTab[m_iColumn][m_iRow].vUV);
	GetOwner()->GetRenderComponent()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_1, &m_arrTab[m_iColumn][m_iRow].vSlice);
	GetOwner()->GetRenderComponent()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_2, &m_arrTab[m_iColumn][m_iRow].vUV2);
	GetOwner()->GetRenderComponent()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_3, &m_arrTab[m_iColumn][m_iRow].vSlice2);
}

void CButtonScript::finaltick()
{
	if (KEY_PRESSED(KEY::LBTN))
	{
		if (GetOwner()->Transform()->Picking(m_pLevelMouseObject->GetScript<CMouseScript>()->GetUiRay(), m_vMousePos))
		{
			m_bClicked = true;
		}
	}
}

void CButtonScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CButtonScript::Overlap(CCollider2D* _pOther)
{
}

void CButtonScript::EndOverlap(CCollider2D* _pOther)
{
}
void CButtonScript::SaveToFile(FILE* _File)
{
	CScript::SaveToFile(_File);
	fwrite(&m_iRow, sizeof(float), 1, _File);
}

void CButtonScript::LoadFromFile(FILE* _File)
{
	CScript::LoadFromFile(_File);
	fread(&m_iRow, sizeof(float), 1, _File);
}

