#include "pch.h"
#include "ComInspector.h"

#include <Engine/CRes.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CGameObject.h>

#include "TransformUI.h"
#include "MeshRenderUI.h"
#include "Collider2DUI.h"
#include "ShadowUI.h"
#include "Light2DUI.h"
#include "Animator2DUI.h"
/*
* Resource
*/
#include "ResUI.h"

#include <Script/CShadowScript.h>

ComInspector::ComInspector()
	: UI("ComInspector")
	, m_TargetCom(nullptr)
	, m_arrComUI{}
{
	/*
	* 자동 조절
	*/
	m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR2D] = new Animator2DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR2D]->begin();
	m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR2D]->SetSize(ImVec2(0.f, 0.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR2D]);

}

ComInspector::~ComInspector()
{
}

void ComInspector::update()
{
	//SetTargetCom(m_TargetCom);
	UI::update();
}

void ComInspector::render_update()
{

}

void ComInspector::SetTargetCom(CComponent* _Target)
{
	m_TargetCom = _Target;

	if (!_Target)
	{
		for (UINT i{}; i < (UINT)COMPONENT_TYPE::END; ++i)
			if (m_arrComUI[i])
				m_arrComUI[i]->Close();
	}
	else
	{
		COMPONENT_TYPE eType = _Target->GetType();

		for (UINT i{}; i < (UINT)COMPONENT_TYPE::END; ++i)
		{
			if (nullptr == m_arrComUI[i])
				continue;

			if ((UINT)eType == i)
			{
				m_arrComUI[i]->Open();
			}
			else
			{
				m_arrComUI[i]->Close();
			}
		}
	}
}


