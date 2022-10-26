#include "pch.h"
#include "CLayer.h"

#include "CGameObject.h"

CLayer::CLayer()
{
}

CLayer::~CLayer()
{
	for (auto iter{ m_vecParent.begin() }; iter != m_vecParent.end(); ++iter)
	{
		Safe_Delete(*iter);
	}

}

void CLayer::begin()
{
	for (size_t i = 0; i < m_vecParent.size(); ++i)
	{
		m_vecParent[i]->begin();
	}
}

void CLayer::tick()
{
	for (auto iter{ m_vecParent.begin()}; iter != m_vecParent.end(); ++iter)
	{
		(*iter)->tick();
	}
}

void CLayer::finaltick()
{
	for (auto iter{ m_vecParent.begin() }; iter != m_vecParent.end(); ++iter)
	{
		(*iter)->finaltick();
	}
}

void CLayer::render()
{
	for (auto iter{ m_vecParent.begin() }; iter != m_vecParent.end(); ++iter)
	{
		(*iter)->render();
	}
}

void CLayer::AddGameObject(CGameObject*_pObject)
{
	m_vecParent.push_back(_pObject);

	static queue<CGameObject*> que;

	que.push(_pObject);

	while (!que.empty())
	{
		auto pGameObecjt = que.front();
		que.pop();

		auto pVecChilds = pGameObecjt->GetChilds();

		for (auto elem{ pVecChilds.begin() }; elem != pVecChilds.end(); ++elem)
		{
			que.push(*elem);
		}

		pGameObecjt->m_iLayerIdx = m_iLayerIdx;
	}
}