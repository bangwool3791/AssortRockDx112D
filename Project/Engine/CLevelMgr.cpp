#include "pch.h"
#include "CLevelMgr.h"

#include "CLevel.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CMesh.h"
#include "CGraphicsShader.h"
#include "CResMgr.h"

#include "CTexture.h"
#include "CMaterial.h"
#include "CPlayerScript.h"

CLevelMgr::CLevelMgr()
	:m_pCurLevel{nullptr}
{
}

CLevelMgr::~CLevelMgr()
{
	Safe_Delete(m_pCurLevel);
}

void CLevelMgr::init()
{
	m_pCurLevel = new CLevel;

	m_pCurLevel->GetLayer(1)->SetName(L"Player");
	m_pCurLevel->GetLayer(2)->SetName(L"PlayerProjectile");
	m_pCurLevel->GetLayer(3)->SetName(L"Monster");
	m_pCurLevel->GetLayer(4)->SetName(L"MonsterProjectile");

	Ptr<CTexture> pTexture = CResMgr::GetInst()->Load<CTexture>(L"MagicCircle", L"texture\\Player.bmp");

	auto pObject = new CGameObject;

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CPlayerScript);
	
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));
	int a = 0;
	pObject->MeshRender()->GetSharedMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, &a);
	pObject->MeshRender()->GetSharedMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTexture);
	m_pCurLevel->AddGameObject(pObject, 0);

	//auto pMonster = new CGameObject;

	//pMonster->AddComponent(new CTransform);
	//pMonster->AddComponent(new CMeshRender);

	//pMonster->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pMonster->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	////pMonster->MeshRender()->GetSharedMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, &a);
	////pMonster->MeshRender()->GetSharedMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTexture);
	//m_pCurLevel->AddGameObject(pObject, 0);
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
