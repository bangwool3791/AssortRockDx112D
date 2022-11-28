#include "pch.h"
#include "InspectorUI.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CGameObject.h>

#include "TransformUI.h"
#include "MeshRenderUI.h"
#include "Collider2DUI.h"
#include "ShadowUI.h"
#include "Light2DUI.h"

#include <Engine/CShadowScript.h>

InspectorUI::InspectorUI()
	: UI("Inspector")
	, m_TargetObj(nullptr)
	, m_arrComUI{}
{
	/*
	* 자동 조절
	*/
	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM] = new TransformUI;	
	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM]->SetSize(ImVec2(0.f, 150.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM]);

	m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER] = new MeshRenderUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER]->SetSize(ImVec2(0.f, 150.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER]);

	m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D] = new Collider2DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D]->SetSize(ImVec2(0.f, 150.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D]);

	m_arrComUI[(UINT)COMPONENT_TYPE::SCRIPT] = new ShadowUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::SCRIPT]->SetSize(ImVec2(0.f, 150.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::SCRIPT]);

	m_arrObjUI[(UINT)OBJECT_TYPE::LIGHT2D] = new Light2DUI;
	m_arrObjUI[(UINT)OBJECT_TYPE::LIGHT2D]->SetSize(ImVec2(0.f, 150.f));
	AddChild(m_arrObjUI[(UINT)OBJECT_TYPE::LIGHT2D]);
}

InspectorUI::~InspectorUI()
{	
}

void InspectorUI::update()
{

	SetTarget(CLevelMgr::GetInst()->FindSelectedObject(L"Player"));

	SetLight(nullptr);

	UI::update();
}

void InspectorUI::render_update()
{

}

void InspectorUI::SetTarget(CGameObject* _Target)
{
	if (!_Target)
		return;

	m_TargetObj = _Target;

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrComUI[i])
		{
			if (nullptr == m_TargetObj->GetComponent((COMPONENT_TYPE)i))
			{
				m_arrComUI[i]->SetTarget(nullptr);
				m_arrComUI[i]->Close();
			}		
			else
			{
				m_arrComUI[i]->SetTarget(_Target);
				m_arrComUI[i]->Open();				
			}
		}		
	}	

	auto pShadow = m_TargetObj->GetChild(L"Shadow");
	auto pShadowScript = m_TargetObj->GetChild(L"Shadow")->GetScript<CShadowScript>(L"ShadowScript");

	if (pShadowScript)
	{
		m_arrComUI[(UINT)COMPONENT_TYPE::SCRIPT]->SetTarget(pShadow);
		m_arrComUI[(UINT)COMPONENT_TYPE::SCRIPT]->Open();
	}
	else
	{
		m_arrComUI[(UINT)COMPONENT_TYPE::SCRIPT]->Close();
		m_arrComUI[(UINT)COMPONENT_TYPE::SCRIPT]->SetTarget(nullptr);
	}
}

void InspectorUI::SetLight(CGameObject* _Light)
{
	Light2DUI* pLight2D = static_cast<Light2DUI*>(m_arrObjUI[(UINT)OBJECT_TYPE::LIGHT2D]);
	CGameObject* pLight{};
	pLight2D->SetDirectionalLight(pLight = CLevelMgr::GetInst()->FindObjectByName(L"DirectionalLight"));
	pLight2D->SetSpotLight(CLevelMgr::GetInst()->FindSelectedObject(L"SpotLight"));
	if (pLight)
		m_arrObjUI[(UINT)OBJECT_TYPE::LIGHT2D]->Open();
	else
		m_arrObjUI[(UINT)OBJECT_TYPE::LIGHT2D]->Close();

}





