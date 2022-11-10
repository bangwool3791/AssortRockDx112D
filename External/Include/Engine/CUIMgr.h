#pragma once
#include "pch.h"

#include "singleton.h"

class CGameObject;

class CUIMgr
	:public CSingletone<CUIMgr>
{
private:
	array<vector<CGameObject*>, (UINT)UI_TYPE::END> m_arrUi;
public:
	void AddUI(CGameObject* _pObj, UI_TYPE _eType);
	void DeleteUI(UI_TYPE _eType);
public:
	void Clear_Objects(UI_TYPE _eType)
	{
		m_arrUi[(UINT)_eType].clear();
	}
	const vector<CGameObject*>& Get_Objects(UI_TYPE _eType) {
		return m_arrUi[(UINT)_eType];
	}

	CGameObject* Get_Object(UI_TYPE _eType) {
		return m_arrUi[(UINT)_eType].front();
	}

public:
	CUIMgr();
	~CUIMgr();
};