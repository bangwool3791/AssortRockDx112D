#include "pch.h"
#include "CLevelMgr.h"

#include "CResMgr.h"

#include "CLevel.h"
#include "CGameObject.h"
#include "GlobalComponent.h"
#include "CPlayerScript.h"
#include "CMonsterScript.h"
#include "CCameraScript.h"


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

	m_pCurLevel->AddGameObject(pCamObj, 0);

	// GameObject 초기화
	CGameObject* pObject = nullptr;

	pObject = new CGameObject;
	pObject->SetName(L"Player");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CPlayerScript);

	pObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, 500.f));
	pObject->Transform()->SetRelativeScale(Vec3(512.f, 512.f, 1.f));

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAlphaBlendMtrl"));

	pObject->MeshRender()->GetSharedMaterial()->SetTexParam(TEX_PARAM::TEX_0, pCharacterTex);

	m_pCurLevel->AddGameObject(pObject, 0);

	/*
	* 몬스터
	*/
	pObject = new CGameObject;
	pObject->SetName(L"Monster");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CMonsterScript);

	pObject->Transform()->SetRelativePos(Vec3(300.f, 0.f, 700.f));
	pObject->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 0.9f));

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	m_pCurLevel->AddGameObject(pObject, 0);
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
