#include "pch.h"
#include "CLevel.h"

CLevel::CLevel()
{
	m_arrLayer[0].SetName(L"Default");

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].m_iLayerIdx = i;
	}
}

CLevel::~CLevel()
{

}

void CLevel::begin()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].begin();
	}
}

void CLevel::tick()
{
	clear_function(m_arrLayer);
	tick_function(m_arrLayer);
}

void CLevel::finaltick()
{
	finaltick_function(m_arrLayer);
}

void CLevel::render()
{
	render_function(m_arrLayer);
}

CLayer* CLevel::GetLayer(const wstring& _strLayerName)
{
	for (auto iter{ m_arrLayer.begin() }; iter != m_arrLayer.end(); ++iter)
	{
		if (iter->GetName() == _strLayerName)
		{
			return &(*iter);
		}
	}
	return nullptr;
}

void CLevel::AddGameObject(CGameObject* _pObject, UINT _iLayerIdx)
{
	assert(0 <= _iLayerIdx && _iLayerIdx < MAX_LAYER);

	m_arrLayer[_iLayerIdx].AddGameObject(_pObject);
}

void CLevel::AddGameObject(CGameObject* _pObject, const wstring& _strLayerName)
{
	auto Layer = GetLayer(_strLayerName);
	Layer->AddGameObject(_pObject);
}

