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

void CLayer::begin()
{
	for (size_t i = 0; i < m_vecGameObject.size(); ++i)
	{
		m_vecGameObject[i]->begin();
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
