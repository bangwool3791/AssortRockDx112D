#include "pch.h"
#include "CLevelMgr.h"

#include "CResMgr.h"

#include "CLevel.h"
#include "CGameObject.h"

#include "GlobalComponent.h"
#include "CGrid2DScript.h"
#include "CPlayerScript.h"
#include "CMonsterScript.h"
#include "CCameraScript.h"
#include "CDragScript.h"

#include "CCollisionMgr.h"

CLevelMgr::CLevelMgr()
	: m_pCurLevel(nullptr)
{
}

CLevelMgr::~CLevelMgr()
{
	if (nullptr != m_pCurLevel)
		delete m_pCurLevel;
}

void CLevelMgr::init()
{
	// Level 하나 제작하기
	m_pCurLevel = new CLevel;

	// Layer 이름 설정
	m_pCurLevel->GetLayer(1)->SetName(L"Player");
	m_pCurLevel->GetLayer(2)->SetName(L"PlayerProjectile");
	m_pCurLevel->GetLayer(3)->SetName(L"Monster");
	m_pCurLevel->GetLayer(4)->SetName(L"MonsterProjectile");


	// 텍스쳐 로딩
	// 텍스쳐 로딩
	Ptr<CTexture> pTexture = CResMgr::GetInst()->Load<CTexture>(L"MagicCircle", L"texture\\Player.bmp");
	Ptr<CTexture> pSmokeTex = CResMgr::GetInst()->Load<CTexture>(L"Smoke", L"texture\\smokeparticle.png");
	Ptr<CTexture> pCharacterTex = CResMgr::GetInst()->Load<CTexture>(L"Character", L"texture\\Character.png");


	// Camera Object 추가
	CGameObject* pCamObj = new CGameObject;
	pCamObj->SetName(L"MainCamera");

	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->AddComponent(new CCameraScript);
	/*
	* LAYER_MAX까지 Render
	*/
	pCamObj->Camera()->SetLayerMaskAll();
	pCamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAHPICS);

	m_pCurLevel->AddGameObject(pCamObj, 0);
	
	// GameObject 초기화
	CGameObject* pObject = nullptr;

	for (float i{ -800.f }; i < 800.f; i += 50)
	{
		pObject = new CGameObject;
		pObject->SetName(L"Player");

		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);
		pObject->AddComponent(new CCollider2D);
		pObject->AddComponent(new CPlayerScript);

		pObject->Transform()->SetRelativePos(Vec3(i, 0.f, 10.f));
		pObject->Transform()->SetRelativeScale(Vec3(40.f, 40.f, 0.f));
		pObject->Transform()->SetRelativeRotation(Vec3(-XM_PI * 0.25f, 0.f, 0.f));

		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));

		pObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::COLLIDER2D_RECT);
		pObject->MeshRender()->GetSharedMaterial()->SetTexParam(TEX_PARAM::TEX_0, pCharacterTex);

		m_pCurLevel->AddGameObject(pObject, 1);
	}
	/*
	* Mouse
	*/
	Ptr<CPrefab> pPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"MousePrefab");
	Instantiate(pPrefab->Instantiate());

	CCollisionMgr::GetInst()->CollisionLayerCheck(0, 1);
	CCollisionMgr::GetInst()->CollisionLayerCheck(1, 1);

	m_pCurLevel->begin();
}

void CLevelMgr::tick()
{
	m_pCurLevel->tick();
}

void CLevelMgr::finaltick()
{
	m_pCurLevel->finaltick();
}

void CLevelMgr::render()
{
	m_pCurLevel->render();
}
