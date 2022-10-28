#pragma once

#include "singleton.h"
#include "pch.h"

class CGameObject;

class CUIMgr
	:public CSingletone<CUIMgr>
{
private:
	vector<CGameObject*> m_vecSelectUnit;
public:
	void AddSelectUnit(CGameObject* _pObj){m_vecSelectUnit.push_back(_pObj);}
	void Clear(){m_vecSelectUnit.clear();}
public:
	CUIMgr();
	~CUIMgr();
};