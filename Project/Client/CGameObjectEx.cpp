#include "pch.h"
#include "CGameObjectEx.h"

#include <Engine/CGameObject.h>
#include <Engine/CComponent.h>

CGameObjectEx::CGameObjectEx()
{
}

CGameObjectEx::~CGameObjectEx()
{
}

/*
* Engine Level���� Render ����� �������� �θ��� finaltick�� ���� ����
*/
void CGameObjectEx::finaltick()
{
	for (UINT i{ 0 }; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if(GetComponent((COMPONENT_TYPE)i))
			GetComponent((COMPONENT_TYPE)i)->finaltick();
	}

	const vector<CGameObject*> m_vecGameObject = GetChilds();

	for(auto iter{m_vecGameObject.begin()}; iter != m_vecGameObject.end(); ++iter)
	{
		(*iter)->finaltick();
	}
}
