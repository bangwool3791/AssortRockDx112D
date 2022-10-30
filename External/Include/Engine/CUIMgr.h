#pragma once
#include "pch.h"

#include "singleton.h"

class CGameObject;

class CUIMgr
	:public CSingletone<CUIMgr>
{
private:
	array<CGameObject*, (UINT)UI_TYPE::END> m_arrUi;
	vector<CGameObject*>					m_vecUnitSelectUI;
public:
	void AddUnitSelectUI(CGameObject* _pObj);
	void Clear_UnitSelectUI();
	void AddUI(CGameObject* _pObj, UI_TYPE _eType);
	void DeleteUI(UI_TYPE _eType);
	CGameObject* Get_Ui_Object(UI_TYPE _eType);
public:
	const vector<CGameObject*>& Get_UnitSelectUIObjects() {
		return m_vecUnitSelectUI;
	}
public:
	CUIMgr();
	~CUIMgr();
};