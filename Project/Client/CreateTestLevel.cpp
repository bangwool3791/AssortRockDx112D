#include "pch.h"

#include "CreateTestLevel.h"

#include <Engine\CPrefab.h>
#include <Engine\CResMgr.h>

#include <Engine\CCamera.h>
#include <Engine\CTransform.h>
#include <Engine\CGameObject.h>
#include <Engine\CLevel.h>
#include <Engine\CLevelMgr.h>

#include <Engine\CCollisionMgr.h>
#include <Script\CPlayerScript.h>
#include <Script\CRefAniScript.h>
#include <Script\CMissileScript.h>
#include <Script\CSelectUnitScript.h>
#include <Script\CDragScript.h>
#include <Script\CMouseScript.h>
#include <Script\CShadowScript.h>

void CreateDefaultPrefab()
{
	CGameObject* pObject = new CGameObject;
	pObject->SetName(L"Missile");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CMissileScript);

	pObject->Transform()->SetRelativeScale(Vec3{ 50.f, 50.f, 1.f });
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
	pObject->MeshRender()->GetSharedMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Plane"));

	CResMgr::GetInst()->AddRes<CPrefab>(L"MissilePrefab", new CPrefab(pObject));

	pObject = new CGameObject;
	pObject->SetName(L"UnitSelectUI");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender(INSTANCING_TYPE::USED));
	pObject->AddComponent(new CSelectUnitScript);

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CircleMesh_Debug"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"UnitSelectUIMaterial"));

	CResMgr::GetInst()->AddRes<CPrefab>(L"UnitSelectUIPrefab", new CPrefab(pObject));

	/*
	* MouseDragPrefab
	*/
	pObject = new CGameObject;
	pObject->SetName(L"MouseDrag");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CDragScript);
	pObject->AddComponent(new CCollider2D);

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"MouseDragMaterial"));
	pObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::COLLIDER2D_RECT);
	CResMgr::GetInst()->AddRes<CPrefab>(L"MouseDragPrefab", new CPrefab(pObject));

	pObject = new CGameObject;
	pObject->SetName(L"MouseObject");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider2D);
	pObject->AddComponent(new CMouseScript);
	CResMgr::GetInst()->AddRes<CPrefab>(L"MousePrefab", new CPrefab(pObject));


	pObject = new CGameObject;
	pObject->SetName(L"Shadow");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender(INSTANCING_TYPE::USED));
	pObject->AddComponent(new CAnimator2D);
	pObject->AddComponent(new CShadowScript);

	pObject->Transform()->SetRelativePos(Vec3(0.5f, 0.5f, 10.f));
	pObject->Transform()->SetRelativeScale(Vec3(1.f, 1.f, 1.f));
	pObject->Transform()->SetRelativeRotation(Vec3(1.f, 0.f, 0.f));
	pObject->Transform()->SetIgnoreParentScale(false);
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"Tile"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ShadowMtrl"));

	pObject->Animator2D()->CreateAnimation(L"LeftWalk", CResMgr::GetInst()->FindRes<CTexture>(L"Link"), Vec2(0.f, 650.f), Vec2(120.f, 130.f), 120.f, 10, 16);
	pObject->Animator2D()->Play(L"LeftWalk", true);

	Ptr<CTexture> pCharacterTex = CResMgr::GetInst()->Load<CTexture>(L"Character", L"texture\\Character.dds");

	pObject->MeshRender()->GetSharedMaterial()->SetTexParam(TEX_PARAM::TEX_0, pCharacterTex);
	CResMgr::GetInst()->AddRes<CPrefab>(L"ShadowPrefab", new CPrefab(pObject));
}

void CreateTestLelvel()
{
	CreateDefaultPrefab();

	CLevel* pLevel = new CLevel;
#if false

	pLevel->GetLayer(1)->SetName(L"Player");
	pLevel->GetLayer(2)->SetName(L"PlayerProjecttile");
	pLevel->GetLayer(3)->SetName(L"Monster");
	pLevel->GetLayer(4)->SetName(L"MonsterProjecttile");

	// Camera Object 추가
	CGameObject* pCamObj = new CGameObject;
	pCamObj->SetName(L"MainCamera");

	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	/*
	* LAYER_MAX까지 Render
	*/
	pCamObj->Camera()->SetLayerMaskAll();
	pCamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAHPICS);
	pCamObj->Transform()->SetRelativePos(Vec3{ 0.f, 0.f, 500.f });
	pLevel->AddGameObject(pCamObj, 0);

	// Directional Light 추가
	CGameObject* pDirLight = new CGameObject;
	pDirLight->SetName(L"DirectionalLight");

	pDirLight->AddComponent(new CTransform);
	pDirLight->AddComponent(new CLight2D);

	pDirLight->Light2D()->SetLightColor(Vec3(0.5f, 0.5f, 0.5f));
	pDirLight->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);

	pLevel->AddGameObject(pDirLight, 0);


	// PointLight 추가
	CGameObject* pPointLight{};// = new CGameObject;
	//pPointLight->SetName(L"PointLight");

	//pPointLight->AddComponent(new CTransform);
	//pPointLight->AddComponent(new CLight2D);

	//pPointLight->Transform()->SetRelativePos(-500.f, 0.f, 0.f);

	//pPointLight->Light2D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	//pPointLight->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	//pPointLight->Light2D()->SetRadius(500.f);


	//pLevel->AddGameObject(pPointLight, 0);

	// SpotLight 추가
	pPointLight = new CGameObject;
	pPointLight->SetName(L"SpotLight");

	pPointLight->AddComponent(new CTransform);
	pPointLight->AddComponent(new CLight2D);
	pPointLight->AddComponent(new CCollider2D);
	pPointLight->AddComponent(new CPlayerScript);

	pPointLight->Transform()->SetRelativePos(0.f, 0.f, 0.f);
	pPointLight->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 0.f));

	pPointLight->Light2D()->SetLightColor(Vec3(0.5f, 0.5f, 0.5f));
	pPointLight->Light2D()->SetLightType(LIGHT_TYPE::SPOT);
	pPointLight->Light2D()->SetRadius(500.f);
	pPointLight->Light2D()->SetAngle(XM_PI * 0.25f);
	pPointLight->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::COLLIDER2D_RECT);
	pLevel->AddGameObject(pPointLight, 1);
	// GameObject 초기화
	CGameObject* pObject = nullptr;

	//Edit Test를 위해 주석 처리
	for (float i{ -500.f }; i < 500.f; i += 100.f)
	{
		pObject = new CGameObject;
		pObject->SetName(L"Player");

		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender(INSTANCING_TYPE::USED));
		pObject->AddComponent(new CCollider2D);
		pObject->AddComponent(new CPlayerScript);
		pObject->AddComponent(new CAnimator2D);

		pObject->Transform()->SetRelativePos(Vec3(i, 0.f, 10.f));
		pObject->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 0.f));
		pObject->Transform()->SetRelativeRotation(Vec3(-XM_PI * 0.25f, 0.f, 0.f));
		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ObjectMtrl"));

		pObject->Animator2D()->CreateAnimation(L"LeftWalk", CResMgr::GetInst()->FindRes<CTexture>(L"Link"), Vec2(0.f, 650.f), Vec2(120.f, 130.f), 120.f, 10, 16);
		pObject->Animator2D()->Play(L"LeftWalk", true);

		pObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::COLLIDER2D_RECT);
		pObject->MeshRender()->GetSharedMaterial()->SetTexParam(TEX_PARAM::TEX_0, pCharacterTex);

		Ptr<CPrefab> pPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"ShadowPrefab");
		Instantiate(pPrefab->Instantiate(), pObject, 1);

		pLevel->AddGameObject(pObject, 1);
	}
	CCollisionMgr::GetInst()->CollisionLayerCheck(1, 1);
	/*
	* Mouse
	*/
	Ptr<CPrefab> pPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"MousePrefab");
	Instantiate(pPrefab->Instantiate(), 31);

	CCollisionMgr::GetInst()->CollisionLayerCheck(1, 31);

	// Particle Object
	//CGameObject* pParticle = new CGameObject;
	//pParticle->SetName(L"Particle");
	//pParticle->AddComponent(new CTransform);
	//pParticle->AddComponent(new CParticleExplosion);

	//pParticle->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));
	//pLevel->AddGameObject(pParticle, 0);

	//pParticle = new CGameObject;
	//pParticle->SetName(L"ParticleWood");
	//pParticle->AddComponent(new CTransform);
	//pParticle->AddComponent(new CParticleWood);

	//pParticle->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));
	//pLevel->AddGameObject(pParticle, 0);

	// PostProcess Object
	//CGameObject* pPostProcess = new CGameObject;
	//pPostProcess->AddComponent(new CTransform);
	//pPostProcess->AddComponent(new CMeshRender(INSTANCING_TYPE::NONE));

	//pPostProcess->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));
	//pPostProcess->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));

	//pPostProcess->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pPostProcess->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"PostProcessMtrl"));

	pObject = new CGameObject;
	pObject->SetName(L"RefAni");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender(INSTANCING_TYPE::NONE));
	pObject->AddComponent(new CCollider2D);
	pObject->AddComponent(new CRefAniScript);

	Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(L"Link");
	UINT width = pTex->GetWidth();
	UINT height = pTex->GetHeight();

	pObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, 10.f));
	pObject->Transform()->SetRelativeScale(Vec3((float)width, (float)height, 10.f));
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"RefAniMtrl"));
	pObject->MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, pTex);
	pLevel->AddGameObject(pObject, 30);
	CCollisionMgr::GetInst()->CollisionLayerCheck(30, 31);
#endif
	CLevelMgr::GetInst()->ChangeLevel(pLevel);
	pLevel->begin();
}