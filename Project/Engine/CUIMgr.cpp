#include "pch.h"
#include "CUIMgr.h"

#include "CGameObject.h"

CUIMgr::CUIMgr()
	:m_arrUi{}
{
}

CUIMgr::~CUIMgr()
{

	for (auto iter{ m_arrUi.begin() }; iter != m_arrUi.end(); ++iter)
	{
		//delete* iter;
		//(*iter) = nullptr;
	}
	for (auto iter{ m_vecUnitSelectUI.begin() }; iter != m_vecUnitSelectUI.end(); ++iter)
	{
		(*iter) = nullptr;
	}

	m_vecUnitSelectUI.clear();
}


void CUIMgr::AddUI(CGameObject* _pObj, UI_TYPE _eType)
{
	assert(!m_arrUi[(UINT)_eType]);
	m_arrUi[(UINT)(_eType)] = _pObj;
}

void CUIMgr::DeleteUI(UI_TYPE _eType)
{
	m_arrUi[(UINT)_eType]->SetDead();
	m_arrUi[(UINT)_eType] = nullptr;
}


CGameObject* CUIMgr::Get_Ui_Object(UI_TYPE _eType)
{
	if (!m_arrUi[(UINT)_eType])
		return nullptr;

	return m_arrUi[(UINT)_eType];
}

void CUIMgr::AddUnitSelectUI(CGameObject* _pObj)
{
	m_vecUnitSelectUI.push_back(_pObj);
}

void CUIMgr::Clear_UnitSelectUI()
{
	for (auto iter{ m_vecUnitSelectUI.begin() }; iter != m_vecUnitSelectUI.end(); ++iter)
	{
		CGameObject* pChild = (*iter)->GetChild(L"UnitSelectUI");
		pChild->SetDead();
	}
	m_vecUnitSelectUI.clear();
}