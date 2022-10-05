#include "pch.h"
#include "CLayer.h"

#include "CGameObject.h"

CLayer::CLayer()
{
}

CLayer::~CLayer()
{
	for (auto iter{ m_vecGameObject.begin() }; iter != m_vecGameObject.end(); ++iter)
	{
		Safe_Delete(*iter);
	}

}

void CLayer::tick()
{
	for (auto iter{ m_vecGameObject.begin()}; iter != m_vecGameObject.end(); ++iter)
	{
		(*iter)->tick();
	}
}

void CLayer::finaltick()
{
	for (auto iter{ m_vecGameObject.begin() }; iter != m_vecGameObject.end(); ++iter)
	{
		(*iter)->finaltick();
	}
}

void CLayer::render()
{
	for (auto iter{ m_vecGameObject.begin() }; iter != m_vecGameObject.end(); ++iter)
	{
		(*iter)->render();
	}
}
