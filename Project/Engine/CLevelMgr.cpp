#include "pch.h"

#include "CLevelMgr.h"
#include "CResMgr.h"

#include "CDevice.h"
#include "CLevel.h"
#include "CGameObject.h"
#include "CAnimator2D.h"

#include "GlobalComponent.h"
#include "GlobalScript.h"

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
	// Level 하나 제작하기

	m_pCurLevel = new CLevel;
	m_pCurLevel->SetName(L"Level 0");
	// Layer 이름 설정

	// 텍스쳐 로딩
}

void CLevelMgr::progress()
{
	m_pCurLevel->ClearLayer();

	if (LEVEL_STATE::PLAY == m_pCurLevel->GetState())
	{
		//키 입력이 안먹는다.
		m_pCurLevel->tick();
		m_pCurLevel->finaltick();
	}
	//엔진에서 제공하는 Component(Transform, register 맵핑, Particle) finaltick동작
}

void CLevelMgr::render()
{
	//m_pCurLevel->render();
}

CGameObject* CLevelMgr::FindObjectByName(const wstring& _strName)
{
	return m_pCurLevel->FindObjectByName(_strName);
}

CGameObject* CLevelMgr::FindSelectedObject(const wstring& _strName)
{
	return m_pCurLevel->FindSelectedObject(_strName);
}

void CLevelMgr::ChangeLevel(CLevel* _NextLevel)
{
	if (m_pCurLevel)
	{
		delete m_pCurLevel;
	}

	m_pCurLevel = _NextLevel;
}

void CLevelMgr::ChangeLevelState(LEVEL_STATE _State)
{
	assert(!(m_pCurLevel->GetState() == _State));
	
	m_pCurLevel->SetState(_State);
	
	if (LEVEL_STATE::PLAY == _State)
		m_pCurLevel->begin();
}

LEVEL_STATE CLevelMgr::GetLevelState()
{
	return m_pCurLevel->GetState();
}
