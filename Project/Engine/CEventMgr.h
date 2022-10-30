#pragma once
#include "singleton.h"

class CGameObject;

class CEventMgr :
	public CSingletone<CEventMgr>
{
private:
	vector<tEvent>			m_vecEvent;
	vector<CGameObject*>	m_vecGarbage;
public :
	void tick();
public:
	void AddEvent(tEvent& _evn) { m_vecEvent.push_back(_evn); }
public:
	CEventMgr();
	~CEventMgr();
};