#include "pch.h"
#include "InspectorUI.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CGameObject.h>

#include "TransformUI.h"
#include "MeshRenderUI.h"
#include "Collider2DUI.h"

InspectorUI::InspectorUI()
	: UI("Inspector")
	, m_TargetObj(nullptr)
	, m_arrComUI{}
{
	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM] = new TransformUI;	
	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM]->SetSize(ImVec2(0.f, 150.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM]);

	m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER] = new MeshRenderUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER]->SetSize(ImVec2(0.f, 150.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER]);

	m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D] = new Collider2DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D]->SetSize(ImVec2(0.f, 150.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D]);

}

InspectorUI::~InspectorUI()
{	
}

void InspectorUI::update()
{

	SetTarget(CLevelMgr::GetInst()->FindSelectedObject(L"Player"));


	UI::update();
}

void InspectorUI::render_update()
{

}


void InspectorUI::SetTarget(CGameObject* _Target)
{
	m_TargetObj = _Target;

	if (!m_TargetObj)
		return;

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
}





