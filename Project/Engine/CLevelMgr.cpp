#include "pch.h"
#include "CLevelMgr.h"

#include "CResMgr.h"

#include "CLevel.h"
#include "CGameObject.h"
#include "CAnimator2D.h"

#include "GlobalComponent.h"
#include "CGrid2DScript.h"
#include "CPlayerScript.h"
#include "CMonsterScript.h"
#include "CCameraScript.h"
#include "CDragScript.h"

#include "CPaintShader.h"

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
	// Level �ϳ� �����ϱ�

	m_pCurLevel = new CLevel;

	// Layer �̸� ����
	m_pCurLevel->GetLayer(1)->SetName(L"Player");
	m_pCurLevel->GetLayer(2)->SetName(L"PlayerProjectile");
	m_pCurLevel->GetLayer(3)->SetName(L"Monster");
	m_pCurLevel->GetLayer(4)->SetName(L"MonsterProjectile");


	// �ؽ��� �ε�
	// �ؽ��� �ε�
	Ptr<CTexture> pTexture = CResMgr::GetInst()->Load<CTexture>(L"MagicCircle", L"texture\\Player.bmp");
	Ptr<CTexture> pSmokeTex = CResMgr::GetInst()->Load<CTexture>(L"Smoke", L"texture\\smokeparticle.png");
	Ptr<CTexture> pCharacterTex = CResMgr::GetInst()->Load<CTexture>(L"Character", L"texture\\Character.png");


	// Camera Object �߰�
	CGameObject* pCamObj = new CGameObject;
	pCamObj->SetName(L"MainCamera");

	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->AddComponent(new CCameraScript);
	/*
	* LAYER_MAX���� Render
	*/
	pCamObj->Camera()->SetLayerMaskAll();
	pCamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAHPICS);

	m_pCurLevel->AddGameObject(pCamObj, 0);
	
	// Directional Light �߰�
	CGameObject* pDirLight = new CGameObject;
	pDirLight->SetName(L"DirectionalLight");

	pDirLight->AddComponent(new CTransform);
	pDirLight->AddComponent(new CLight2D);

	pDirLight->Light2D()->SetLightColor(Vec3(0.f, 0.f, 0.f));
	pDirLight->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);

	m_pCurLevel->AddGameObject(pDirLight, 0);


	// PointLight �߰�
	CGameObject* pPointLight{};// = new CGameObject;
	//pPointLight->SetName(L"PointLight");

	//pPointLight->AddComponent(new CTransform);
	//pPointLight->AddComponent(new CLight2D);

	//pPointLight->Transform()->SetRelativePos(-500.f, 0.f, 0.f);

	//pPointLight->Light2D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	//pPointLight->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	//pPointLight->Light2D()->SetRadius(500.f);


	//m_pCurLevel->AddGameObject(pPointLight, 0);

	// SpotLight �߰�
	pPointLight = new CGameObject;
	pPointLight->SetName(L"SpotLight");

	pPointLight->AddComponent(new CTransform);
	pPointLight->AddComponent(new CLight2D);

	pPointLight->Transform()->SetRelativePos(0.f, 0.f, 0.f);

	pPointLight->Light2D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	pPointLight->Light2D()->SetLightType(LIGHT_TYPE::SPOT);
	pPointLight->Light2D()->SetRadius(500.f);
	pPointLight->Light2D()->SetAngle(XM_PI * 0.25f);
	m_pCurLevel->AddGameObject(pPointLight, 0);
	// GameObject �ʱ�ȭ
	CGameObject* pObject = nullptr;

	pObject = new CGameObject;
	pObject->SetName(L"Player");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CCollider2D);
	pObject->AddComponent(new CPlayerScript);
	pObject->AddComponent(new CAnimator2D);

	pObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, 10.f));
	pObject->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 0.f));
	pObject->Transform()->SetRelativeRotation(Vec3(-XM_PI * 0.25f, 0.f, 0.f));

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));

	pObject->Animator2D()->CreateAnimation(L"LeftWalk", CResMgr::GetInst()->FindRes<CTexture>(L"Link"), Vec2(0.f, 650.f), Vec2(120.f, 130.f), 120.f, 10, 16);
	pObject->Animator2D()->Play(L"LeftWalk", true);

	pObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::COLLIDER2D_RECT);
	pObject->MeshRender()->GetSharedMaterial()->SetTexParam(TEX_PARAM::TEX_0, pCharacterTex);

	m_pCurLevel->AddGameObject(pObject, 1);

	Ptr<CPaintShader> pComputeShader = (CPaintShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"PaintShader").Get();
	pComputeShader->SetTexture(CResMgr::GetInst()->FindRes<CTexture>(L"UAVTex"));
	pComputeShader->SetColor(Vec4(0.f, 0.f, 1.f, 1.f));
	pComputeShader->Excute();

	CGameObject* pChild = new CGameObject;
	pChild->SetName(L"Child");

	pChild->AddComponent(new CTransform);
	pChild->AddComponent(new CMeshRender);
	pChild->AddComponent(new CCollider2D);

	pChild->Transform()->SetIgnoreParentScale(true);
	pChild->Transform()->SetRelativePos(Vec3(512.f, 0.f, 100.f));
	pChild->Transform()->SetRelativeScale(Vec3(256.f, 256.f, 1.f));

	pChild->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pChild->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
	pChild->MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"UAVTex"));
	/*
	* Mouse
	*/
	//Ptr<CPrefab> pPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"MousePrefab");
	//Instantiate(pPrefab->Instantiate(), 31);

	//CCollisionMgr::GetInst()->CollisionLayerCheck(1, 1);
	//CCollisionMgr::GetInst()->CollisionLayerCheck(1, 31);
	m_pCurLevel->AddGameObject(pChild, 1);
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
