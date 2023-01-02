#include "pch.h"
#include "CEditor.h"

#include "imgui.h"
#include "CGameObjectEx.h"

#include "CGrid2DScript.h"
#include "CBorderScript.h"

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

#include <Script\CDragScript.h>
#include <Script\CEditorMouseScript.h>
#include <Script\CMouseScript.h>
#include <Script\CTileScript.h>
#include <Script\CRefAniScript.h>

CEditor::CEditor()
	:m_editmode{EDIT_MODE::MAPTOOL}
{

}

CEditor::~CEditor()
{
	
	for (auto iter{ m_EditorObj.begin() }; iter != m_EditorObj.end(); ++iter)
	{
		for (auto iter2{ iter->begin() }; iter2 != iter->end(); ++iter2)
		{
			Safe_Delete(iter2->second);
		}
	}
	Safe_Del_Array(m_DebugDrawObject);
	
	for (size_t i{}; i < (UINT)COMPONENT_TYPE::END; ++i)
		Safe_Delete(m_arrCom[i]);
	
	Safe_Delete(m_GirdObject);
	Safe_Delete(m_MouseObject);
	Safe_Delete(m_pCameraObject);
}

void CEditor::init()
{
	m_EditorObj.resize((UINT)EDIT_MODE::END);

	CreateDebugDrawObject();
	// Editor 용도 Grid Object 추가
	m_GirdObject = new CGameObjectEx;
	m_GirdObject->SetName(L"Grid Object");

	m_GirdObject->AddComponent(new CTransform);
	m_GirdObject->AddComponent(new CMeshRender);
	m_GirdObject->AddComponent(new CBorderScript);

	m_GirdObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"BorderMesh"));
	m_GirdObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"BorderMtrl"));

	m_GirdObject->GetScript<CBorderScript>()->SetGridColor(Vec4(0.2f, 0.9f, 0.2f, 1.f));
	m_GirdObject->GetScript<CBorderScript>()->SetGridInterval(100.f);
	m_GirdObject->GetScript<CBorderScript>()->SetThickness(2.f);

	m_GirdObject->MeshRender()->SetInstancingType(INSTANCING_TYPE::NONE);

	m_pCameraObject= new CGameObjectEx;
	m_pCameraObject->SetName(L"Editor Camera");

	m_pCameraObject->AddComponent(new CTransform);
	m_pCameraObject->AddComponent(new CEditorCam);
	m_pCameraObject->AddComponent(new CCameraScript);
	m_pCameraObject->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAHPICS);
	m_pCameraObject->Camera()->SetFar(100000.f);
	m_pCameraObject->Camera()->SetLayerMaskAll();
	m_pCameraObject->Camera()->SetLayerMask(31);
	m_pCameraObject->Transform()->SetRelativePos(1200.f, 500.f, 1000.f);
	m_pCameraObject->Transform()->SetRelativeRotation(Vec3(-XM_PI * 0.25f, 0.f, 0.f));
	CRenderMgr::GetInst()->RegisterEditCam(m_pCameraObject->Camera());

	m_MouseObject = new CGameObjectEx;
	m_MouseObject->SetName(L"MouseObject");
	m_MouseObject->AddComponent(new CTransform);
	m_MouseObject->AddComponent(new CEditorMouseScript);
	m_MouseObject->GetScript<CEditorMouseScript>(L"CEditorMouseScript")->Initialize(m_pCameraObject);
	

	CreateTileMap(m_pCameraObject, m_MouseObject);

	CreateAnimatorTool(m_pCameraObject, m_MouseObject);
	
	/*
	* Component List
	*/
	m_arrCom[(UINT)COMPONENT_TYPE::TRANSFORM]	= new CTransform();
	m_arrCom[(UINT)COMPONENT_TYPE::TRANSFORM]->SetName(L"Transform");
	m_arrCom[(UINT)COMPONENT_TYPE::COLLIDER2D]	= new CCollider2D();
	m_arrCom[(UINT)COMPONENT_TYPE::COLLIDER2D]->SetName(L"Colider2D");
	m_arrCom[(UINT)COMPONENT_TYPE::ANIMATOR2D]	= new CAnimator2D();
	((CAnimator2D*)m_arrCom[(UINT)COMPONENT_TYPE::ANIMATOR2D])->CreateAnimation(L"LeftWalk", CResMgr::GetInst()->FindRes<CTexture>(L"texture\\HumansA_LQ.png"), Vec2(0.f, 650.f), Vec2(120.f, 130.f), 120.f, 10, 16);
	((CAnimator2D*)m_arrCom[(UINT)COMPONENT_TYPE::ANIMATOR2D])->Play(L"LeftWalk", true);
	m_arrCom[(UINT)COMPONENT_TYPE::ANIMATOR2D]->SetName(L"Animator2D");
	//m_arrCom[(UINT)COMPONENT_TYPE::LIGHT2D]		= new CLight2D();
	//m_arrCom[(UINT)COMPONENT_TYPE::LIGHT2D]->SetName(L"Light2D");
	m_arrCom[(UINT)COMPONENT_TYPE::PARTICLESYSTEM] = new CParticleSystem();
	m_arrCom[(UINT)COMPONENT_TYPE::PARTICLESYSTEM]->SetName(L"CParticleSystem");
	m_arrCom[(UINT)COMPONENT_TYPE::LIGHT2D] = new CLight2D();
	m_arrCom[(UINT)COMPONENT_TYPE::LIGHT2D]->SetName(L"CLight2D");
	m_arrCom[(UINT)COMPONENT_TYPE::MESHRENDER] = new CMeshRender();
	m_arrCom[(UINT)COMPONENT_TYPE::MESHRENDER]->SetName(L"CMeshRender");
}

/*
* Editor 상태 분기 
* progress, tick render
*/
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
	m_pCameraObject->tick();
	m_MouseObject->tick();
	
	for (auto iter{ m_EditorObj[(UINT)m_editmode].begin() }; iter != m_EditorObj[(UINT)m_editmode].end(); ++iter)
	{
		iter->second->tick();
	}

	switch (m_editmode)
	{
	case EDIT_MODE::ANIMATOR:
		if(m_pAnimationObject)
			m_pAnimationObject->tick();
		break;
	case EDIT_MODE::MAPTOOL:
		m_GirdObject->tick();
		break;
	case EDIT_MODE::OBJECT:
		break;
	case EDIT_MODE::END:
		break;
	default:
		break;
	}

	m_pCameraObject->finaltick();
	m_MouseObject->finaltick();

	for (auto iter{ m_EditorObj[(UINT)m_editmode].begin() }; iter != m_EditorObj[(UINT)m_editmode].end(); ++iter)
	{
		iter->second->finaltick();
	}

	switch (m_editmode)
	{
	case EDIT_MODE::ANIMATOR:
		if (m_pAnimationObject)
			m_pAnimationObject->finaltick();
		break;
	case EDIT_MODE::MAPTOOL:
		m_GirdObject->finaltick();
		break;
	case EDIT_MODE::OBJECT:
		break;
	case EDIT_MODE::END:
		break;
	default:
		break;
	}
}

void CEditor::render()
{
	m_MouseObject->render();

	for (auto iter{ m_EditorObj[(UINT)m_editmode].begin() }; iter != m_EditorObj[(UINT)m_editmode].end(); ++iter)
	{
		iter->second->render();
	}

	switch (m_editmode)
	{
	case EDIT_MODE::ANIMATOR:
		if (m_pAnimationObject)
			m_pAnimationObject->render();
		break;
	case EDIT_MODE::MAPTOOL:
		m_GirdObject->render();
		break;
	case EDIT_MODE::OBJECT:
		break;
	case EDIT_MODE::END:
		break;
	default:
		break;
	}

	m_pCameraObject->Camera()->render();
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
	pDebugObj->MeshRender()->SetInstancingType(INSTANCING_TYPE::USED);
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
	CGameObjectEx* pGameObectEx = new CGameObjectEx;
	pGameObectEx->AddComponent(new CTransform);
	pGameObectEx->AddComponent(new CTileMap);
	pGameObectEx->AddComponent(new CTileScript);
	pGameObectEx->SetName(L"EditorTileMap");

	CGameObject* arr[2] = { _pCamera, _pMouse };
	pGameObectEx->TileMap()->SetCamera(_pCamera);
	pGameObectEx->GetScript<CTileScript>(L"CTileScript")->Initialize(arr);
	pGameObectEx->Transform()->SetRelativePos(0.f, 0.f, 0.f);
	pGameObectEx->Transform()->SetRelativeScale(1.f, 1.f, 1.f);

	for (UINT i{}; i < TEX_32 + 1; ++i)
	{
		wstring str = L"texture\\Terrain\\Tile\\Tile";
		str += std::to_wstring(i);
		str += L".png";
		pGameObectEx->TileMap()->SetTileAtlas(CResMgr::GetInst()->FindRes<CTexture>(str));
	}

	pGameObectEx->TileMap()->SetTileCount(TILEX, TILEZ);
	pGameObectEx->begin();
	m_EditorObj[(UINT)EDIT_MODE::MAPTOOL].emplace(L"MapTool", pGameObectEx);
}

void CEditor::CreateAnimatorTool(CGameObject* _pCamera, CGameObject* _pMouse)
{
	CGameObjectEx* pObject;

	pObject = new CGameObjectEx;
	pObject->SetName(L"AnimationTool");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender(INSTANCING_TYPE::NONE));
	pObject->AddComponent(new CRefAniScript);
	pObject->AddComponent(new CAnimator2D);

	Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\HumansA_LQ.png");
	UINT width = pTex->GetWidth();
	UINT height = pTex->GetHeight();

	pObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, 10.f));
	pObject->Transform()->SetRelativeScale(Vec3((float)width, 0.f, height));
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"RefAniMtrl"));
	pObject->MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, pTex);
	pObject->begin();

	m_EditorObj[(UINT)EDIT_MODE::ANIMATOR].emplace(L"AnimationTool", pObject);

	pObject = new CGameObjectEx;
	pObject->SetName(L"AnimationToolDrag");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CDragScript);

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"MouseDragMaterial"));
	pObject->GetScript<CDragScript>(L"CDragScript")->SetCamera(m_pCameraObject);
	pObject->begin();

	m_EditorObj[(UINT)EDIT_MODE::ANIMATOR].emplace(L"AnimationToolDrag", pObject);

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
	g_transform.matView = CRenderMgr::GetInst()->GetMainCam()->GetViewMat();
	g_transform.matProj = CRenderMgr::GetInst()->GetMainCam()->GetProjMat();

	pDebugObj->MeshRender()->SetInstancingType(INSTANCING_TYPE::NONE);
	pDebugObj->render();
}

void CEditor::Add_Editobject(EDIT_MODE _emode, CGameObjectEx* _pGameObject)
{
	const wstring& wstrName = _pGameObject->GetName();

	map<const wchar_t*, CGameObjectEx*>& map = m_EditorObj[(UINT)_emode];

	for (auto iter{ map.begin() }; iter != map.end(); ++iter)
	{
		if (!lstrcmp(wstrName.c_str(), iter->first))
			return;
	}

	map.emplace(make_pair(wstrName.c_str(), _pGameObject));
}

void CEditor::UpdateAnimationObject(CGameObject* _pGameObject)
{
	m_pAnimationObject = _pGameObject;
}

void CEditor::Add_Editobject(EDIT_MODE _emode, const wchar_t* _pName, CGameObjectEx* _pGameObject)
{
	map<const wchar_t*, CGameObjectEx*>& map = m_EditorObj[(UINT)_emode];
	
	for (auto iter{ map.begin() }; iter != map.end(); ++iter)
	{
		if (!lstrcmp(_pName, iter->first))
			return;
	}

	map.emplace(make_pair(_pName, _pGameObject));
}

CGameObjectEx* CEditor::FindByName(const wstring& _strky)
{
	if (!lstrcmp(_strky.c_str(), L"MouseObject"))
	{
		return m_MouseObject;
	}
	else if (!lstrcmp(_strky.c_str(), L"Editor Camera"))
	{
		return m_pCameraObject;
	}

	for (auto iter{ m_EditorObj.begin() }; iter != m_EditorObj.end(); ++iter)
	{
		for (auto iter2{ iter->begin() }; iter2 != iter->end(); ++iter2)
		{
			if (!lstrcmp(_strky.c_str(), iter2->first))
			{
				return iter2->second;
			}
		}
	}
	return nullptr;
}

void CEditor::PopByName(const wstring& _strky)
{

	for (auto iter{ m_EditorObj.begin() }; iter != m_EditorObj.end(); ++iter)
	{
		for (auto iter2{ iter->begin() }; iter2 != iter->end(); ++iter2)
		{
			if (!lstrcmp(_strky.c_str(), iter2->first))
			{
				iter->erase(iter2);
				break;
			}
		}
	}
}

void CEditor::SetEditMode(EDIT_MODE _editmode)
{
	assert(_editmode != EDIT_MODE::END);

	m_editmode = _editmode;

	Vec3 vPos{};
	CGameObjectEx* pGmaeObject{};

	switch (_editmode)
	{
	case EDIT_MODE::ANIMATOR:
	{
		//pGmaeObject = m_EditorObj[(UINT)m_editmode].find(L"AnimationTool")->second;
		//vPos = pGmaeObject->Transform()->GetRelativePos();
	}
		break;
	case EDIT_MODE::MAPTOOL:
		//pGmaeObject = m_EditorObj[(UINT)m_editmode].find(L"MapTool")->second;
		//vPos = pGmaeObject->Transform()->GetRelativePos();
		break;
	}
	//m_pCameraObject->Transform()->SetRelativePos(vPos);
}

