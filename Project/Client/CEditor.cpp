#include "pch.h"
#include "CEditor.h"

#include "imgui.h"
#include "CGameObjectEx.h"

#include "CGrid2DScript.h"

#include <Engine/CTimeMgr.h>
#include <Engine\CMeshRender.h>

#include <Engine\CComponent.h>
#include <Engine\GlobalComponent.h>

#include <Engine\CCamera.h>

#include "CEditorCam.h"
#include "CCameraScript.h"

#include <Engine\CRenderMgr.h>
#include <Engine/CLevel.h>
#include <Engine\CLevelMgr.h>

#include <Script\CEditorMouseScript.h>
#include <Script\CMouseScript.h>
#include <Script\CTileScript.h>

CEditor::CEditor()
{

}

CEditor::~CEditor()
{
	Safe_Del_Vec(m_vecEditorObj);
	Safe_Del_Vec(m_vecDummyObj);
	Safe_Del_Array(m_DebugDrawObject);
	Safe_Del_Array(m_arrCom);
}

void CEditor::init()
{
	CreateDebugDrawObject();
	// Editor 용도 Grid Object 추가
	CGameObjectEx* pGridObj = new CGameObjectEx;
	pGridObj->SetName(L"Grid Object");

	pGridObj->AddComponent(new CTransform);
	pGridObj->AddComponent(new CMeshRender);
	pGridObj->AddComponent(new CGrid2DScript);

	pGridObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pGridObj->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"EditMaterial"));

	pGridObj->GetScript<CGrid2DScript>()->SetGridColor(Vec4(0.2f, 0.9f, 0.2f, 1.f));
	pGridObj->GetScript<CGrid2DScript>()->SetGridInterval(100.f);
	pGridObj->GetScript<CGrid2DScript>()->SetThickness(2.f);

	pGridObj->MeshRender()->SetInstancingType(INSTANCING_TYPE::NONE);
	m_vecEditorObj.push_back(pGridObj);

	CGameObjectEx* pObject = new CGameObjectEx;
	pObject->SetName(L"Dummy Object");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender(INSTANCING_TYPE::NONE));
	pObject->AddComponent(new CCollider2D);
	//pObject->AddComponent(new CAnimator2D);

	pObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, 10.f));
	pObject->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 0.f));
	pObject->Transform()->SetRelativeRotation(Vec3(-XM_PI * 0.25f, 0.f, 0.f));
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));

	//pObject->Animator2D()->CreateAnimation(L"LeftWalk", CResMgr::GetInst()->FindRes<CTexture>(L"Link"), Vec2(0.f, 650.f), Vec2(120.f, 130.f), 120.f, 10, 16);
	//pObject->Animator2D()->Play(L"LeftWalk", true);
	m_vecDummyObj.push_back(pObject);

	CGameObjectEx* pEditorCam = new CGameObjectEx;
	pEditorCam->SetName(L"Editor Camera");

	pEditorCam->AddComponent(new CTransform);
	pEditorCam->AddComponent(new CEditorCam);
	pEditorCam->AddComponent(new CCameraScript);
	pEditorCam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAHPICS);
	pEditorCam->Camera()->SetFar(100000.f);
	pEditorCam->Camera()->SetLayerMaskAll();
	pEditorCam->Camera()->SetLayerMask(31);

	m_vecEditorObj.push_back(pEditorCam);
	CRenderMgr::GetInst()->RegisterEditCam(pEditorCam->Camera());

	CGameObjectEx* pMouseObject = new CGameObjectEx;
	pMouseObject->SetName(L"MouseObject");
	pMouseObject->AddComponent(new CTransform);
	pMouseObject->AddComponent(new CEditorMouseScript);
	pMouseObject->GetScript<CEditorMouseScript>(L"CEditorMouseScript")->Initialize(pEditorCam);
	m_vecEditorObj.push_back(pMouseObject);

	CreateTileMap(pEditorCam, pMouseObject);

	/*
	* Component List
	*/
	m_arrCom[(UINT)COMPONENT_TYPE::TRANSFORM]	= new CTransform();
	m_arrCom[(UINT)COMPONENT_TYPE::TRANSFORM]->SetName(L"Transform");
	m_arrCom[(UINT)COMPONENT_TYPE::COLLIDER2D]	= new CCollider2D();
	m_arrCom[(UINT)COMPONENT_TYPE::COLLIDER2D]->SetName(L"Colider2D");
	m_arrCom[(UINT)COMPONENT_TYPE::ANIMATOR2D]	= new CAnimator2D();
	((CAnimator2D*)m_arrCom[(UINT)COMPONENT_TYPE::ANIMATOR2D])->CreateAnimation(L"LeftWalk", CResMgr::GetInst()->FindRes<CTexture>(L"Link"), Vec2(0.f, 650.f), Vec2(120.f, 130.f), 120.f, 10, 16);
	((CAnimator2D*)m_arrCom[(UINT)COMPONENT_TYPE::ANIMATOR2D])->Play(L"LeftWalk", true);
	m_arrCom[(UINT)COMPONENT_TYPE::ANIMATOR2D]->SetName(L"Animator2D");
	m_arrCom[(UINT)COMPONENT_TYPE::LIGHT2D]		= new CLight2D();
	m_arrCom[(UINT)COMPONENT_TYPE::LIGHT2D]->SetName(L"Light2D");
}

void CEditor::progress()
{
	if (LEVEL_STATE::PLAY != CLevelMgr::GetInst()->GetCurLevel()->GetState())
	{
		tick();
		render();
	}
	//Debug Shape
	debug_render();
}

void CEditor::tick()
{
	for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
	{
		m_vecEditorObj[i]->tick();
	}

	for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
	{
		m_vecEditorObj[i]->finaltick();
	}
}

void CEditor::render()
{

	for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
	{
		m_vecEditorObj[i]->render();
	}
}

void CEditor::debug_render()
{
	// DebugDrawRender
// 일정 시간동안 렌더링 되는 Shape 
	list<tDebugShapeInfo>::iterator iter = m_DebugDrawList.begin();
	for (; iter != m_DebugDrawList.end(); )
	{
		iter->fCurTime += DT;
		if (iter->fDuration < iter->fCurTime)
		{
			iter = m_DebugDrawList.erase(iter);
		}
		else
		{
			DebugDraw(*iter);
			++iter;
		}
	}

	// 새로 추가된 DebugShape 확인
	vector<tDebugShapeInfo>& vecInfo = CRenderMgr::GetInst()->GetDebugDrawInfo();

	for (size_t i = 0; i < vecInfo.size(); ++i)
	{
		DebugDraw(vecInfo[i]);

		if (0.f < vecInfo[i].fDuration)
		{
			m_DebugDrawList.push_back(vecInfo[i]);
		}
	}
	vecInfo.clear();
}

void CEditor::CreateDebugDrawObject()
{
	CGameObjectEx* pDebugObj = nullptr;

	// DEBUG_SHAPE::RECT
	pDebugObj = new CGameObjectEx;

	pDebugObj->AddComponent(new CTransform);
	pDebugObj->AddComponent(new CMeshRender);

	pDebugObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_Debug"));
	pDebugObj->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DebugDrawMtrl"));

	m_DebugDrawObject[(UINT)DEBUG_SHAPE::RECT] = pDebugObj;

	// DEBUG_SHAPE::CIRCLE
	pDebugObj = new CGameObjectEx;

	pDebugObj->AddComponent(new CTransform);
	pDebugObj->AddComponent(new CMeshRender);

	pDebugObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CircleMesh_Debug"));
	pDebugObj->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DebugDrawMtrl"));

	m_DebugDrawObject[(UINT)DEBUG_SHAPE::CIRCLE] = pDebugObj;
}

void CEditor::CreateTileMap(CGameObject* _pCamera, CGameObject* _pMouse)
{
	CGameObjectEx* pTileMapObj = new CGameObjectEx;
	pTileMapObj->AddComponent(new CTransform);
	pTileMapObj->AddComponent(new CTileMap);
	pTileMapObj->AddComponent(new CTileScript);
	pTileMapObj->SetName(L"EditorTileMap");
	//TileMapMtrl
	CGameObject* arr[2] = { _pCamera, _pMouse };
	pTileMapObj->TileMap()->SetCamera(_pCamera);
	pTileMapObj->GetScript<CTileScript>(L"CTileScript")->Initialize(arr);
	pTileMapObj->Transform()->SetRelativePos(0.f, 0.f, 1000.f);
	pTileMapObj->Transform()->SetRelativeScale(Vec3(TILECX, TILECY, 1.f));

	for (UINT i{}; i < TEX_32 + 1; ++i)
	{
		wstring str = L"Tile";
		str += std::to_wstring(i);
		pTileMapObj->TileMap()->SetTileAtlas(CResMgr::GetInst()->FindRes<CTexture>(str));
	}

	pTileMapObj->TileMap()->SetTileCount(TILEX, TILEY);
	pTileMapObj->begin();
	m_vecEditorObj.push_back(pTileMapObj);
}

void CEditor::DebugDraw(tDebugShapeInfo& _info)
{
	CGameObjectEx* pDebugObj = m_DebugDrawObject[(UINT)_info.eShape];

	pDebugObj->Transform()->SetRelativePos(_info.vPosition);
	pDebugObj->Transform()->SetRelativeRotation(_info.vRot);

	if (DEBUG_SHAPE::CIRCLE == _info.eShape || DEBUG_SHAPE::SPHERE == _info.eShape)
	{
		pDebugObj->Transform()->SetRelativeScale(Vec3(_info.fRadius, _info.fRadius, _info.fRadius));
	}
	else
	{
		pDebugObj->Transform()->SetRelativeScale(_info.vScale);
	}

	pDebugObj->MeshRender()->GetCurMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, &_info.vColor);


	CCamera* pMainCam = CRenderMgr::GetInst()->GetMainCam();

	pDebugObj->Transform()->finaltick();

	g_transform.matWorld = pDebugObj->Transform()->GetWorldMat();
	g_transform.matView = pMainCam->GetViewMat();
	g_transform.matProj = pMainCam->GetProjMat();

	pDebugObj->MeshRender()->SetInstancingType(INSTANCING_TYPE::NONE);
	pDebugObj->render();
}

CGameObjectEx* CEditor::GetDummyObject(const wstring& _name)
{
	for (UINT i{}; i < m_vecDummyObj.size(); ++i)
	{
		if (m_vecDummyObj[i]->GetName() == _name)
		{
			return m_vecDummyObj[i];
		}
	}
	return nullptr;
}

void CEditor::AddEditObject(CGameObjectEx* _pGameObject)
{
	for (UINT i{}; i < m_vecEditorObj.size(); ++i)
	{
		if (m_vecEditorObj[i]->GetName() == _pGameObject->GetName())
		{
			/*
			* Message Box 추가
			*/
			return;
		}
	}
	m_vecEditorObj.push_back(_pGameObject);
}

CGameObjectEx* CEditor::FindByName(const wstring& _strky)
{
	for (UINT i{}; i < m_vecEditorObj.size(); ++i)
	{
		if (m_vecEditorObj[i]->GetName() == _strky)
			return m_vecEditorObj[i];
	}
	return nullptr;
}


