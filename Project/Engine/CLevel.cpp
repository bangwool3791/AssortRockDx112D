#include "pch.h"
#include "CLevel.h"
#include "func.cpp"

CLevel::CLevel()
{
	m_arrLayer[0].SetName(L"Default");
}

CLevel::~CLevel()
{

}

void CLevel::tick()
{
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

