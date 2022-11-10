#include "pch.h"
#include "CEditor.h"
#include <Engine/CResMgr.h>
#include <Engine/CRenderMgr.h>

#include <Engine/GlobalComponent.h>
#include <Engine/CGameObject.h>
#include <Engine/CGrid2DScript.h>
#include "CGameObjectEx.h"

CEditor::CEditor()
	:m_vecEditorObj{}
	, m_DebugDrawObject{}
{
}

CEditor::~CEditor()
{

	for (UINT i{ 0 }; i < m_vecEditorObj.size(); ++i)
	{
		delete m_vecEditorObj[i];
	}

	//Safe_Del_Vec(m_vecEditorObj);
	Safe_Del_Array(m_DebugDrawObject);
}

void CEditor::init()
{
	CreateDebugDrawObject();

	////CGameObjectEx* pGridObj = new CGameObjectEx;
	////pGridObj->SetName(L"Grid Object");

	////pGridObj->AddComponent(new CTransform);
	////pGridObj->AddComponent(new CMeshRender);
	////pGridObj->AddComponent(new CGrid2DScript);

	////pGridObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	////pGridObj->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"EditMaterial"));

	////pGridObj->GetScript<CGrid2DScript>()->SetGridColor(Vec4(0.2f, 0.9f, 0.2f, 1.f));
	////pGridObj->GetScript<CGrid2DScript>()->SetGridInterval(100.f);
	////pGridObj->GetScript<CGrid2DScript>()->SetThickness(2.f);


	//CGameObjectEx* pTile{};
	//Vec4 vColor = Vec4{ 0.f, 1.f, 0.f, 1.f };
	//for (int i = 0; i < TILEY; ++i)
	//{
	//	for (int j = 0; j < TILEX; ++j)
	//	{
	//		pTile = new CGameObjectEx;
	//		pTile->SetName(L"Tile");
	//		//pTile->AddComponent(new CTransform);
	//		//pTile->AddComponent(new CMeshRender);

	//		//pTile->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"Tile"));
	//		//pTile->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DebugMaterial"));
	//		//pTile->MeshRender()->GetCurMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, &vColor);

	//		float fX = (TILECX * j) + ((TILECX / 2.f) * (i % 2));
	//		float fY = (TILECY / 2.f) * i;

	//		//pTile->Transform()->SetRelativePos(fX -800.f, fY - 450.f, 1.f);
	//		//pTile->Transform()->SetRelativeScale(TILECX, TILECY, 0.f);
	//		m_vecEditorObj.push_back(pTile);
	//	}
	//}
}

void CEditor::progress()
{
	tick();

	render();
}

void CEditor::tick()
{
	for (auto iter{ m_vecEditorObj.begin() }; iter != m_vecEditorObj.end(); ++iter)
	{
		(*iter)->tick();
	}

	for (auto iter{ m_vecEditorObj.begin() }; iter != m_vecEditorObj.end(); ++iter)
	{
		(*iter)->finaltick();
	}
}

void CEditor::render()
{
	for (auto iter{ m_vecEditorObj.begin() }; iter != m_vecEditorObj.end(); ++iter)
	{
		(*iter)->render();
	}

	vector<tDebugShapeInfo>& vecInfo = CRenderMgr::GetInst()->GetDebugDrawInfo();

	for (auto iter{ vecInfo.begin() }; iter != vecInfo.end(); ++iter)
	{
		DebugDraw((*iter));
	}
	vecInfo.clear();
}

void CEditor::CreateDebugDrawObject()
{
	auto pDebugObj = new CGameObjectEx();

	pDebugObj->AddComponent(new CTransform);
	pDebugObj->AddComponent(new CMeshRender);
	
	pDebugObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_Debug"));
	pDebugObj->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DebugMaterial"));
	m_DebugDrawObject[(UINT)DEBUG_SHAPE::RECT] = pDebugObj;

	pDebugObj = new CGameObjectEx();

	pDebugObj->AddComponent(new CTransform);
	pDebugObj->AddComponent(new CMeshRender);

	pDebugObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CircleMesh_Debug"));
	pDebugObj->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DebugMaterial"));
	m_DebugDrawObject[(UINT)DEBUG_SHAPE::CIRCLE] = pDebugObj;
}

void CEditor::DebugDraw(tDebugShapeInfo& _info)
{
	CGameObjectEx* pDebugObj = m_DebugDrawObject[(UINT)_info.eShape];

	pDebugObj->Transform()->SetRelativePos(_info.vPosition);
	pDebugObj->Transform()->SetRelativeRotation(_info.vRot);

	if (_info.eShape == DEBUG_SHAPE::CIRCLE || _info.eShape == DEBUG_SHAPE::CUBE)
	{
		pDebugObj->Transform()->SetRelativeScale(Vec3{ _info.fRadius,_info.fRadius ,_info.fRadius });
	}
	else
	{
		pDebugObj->Transform()->SetRelativeScale(_info.vScale);
	}

	pDebugObj->MeshRender()->GetCurMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, &_info.vColor);

	auto pCamera = CRenderMgr::GetInst()->GetMainCam();

	pDebugObj->Transform()->finaltick();

	g_transform.matWorld = pDebugObj->Transform()->GetWorldMat();
	/*
	* 주석 처리하고 테스트 필요
	*/
	g_transform.matView = pCamera->GetViewMat();
	g_transform.matProj= pCamera->GetProjMat();

	pDebugObj->render();
}

