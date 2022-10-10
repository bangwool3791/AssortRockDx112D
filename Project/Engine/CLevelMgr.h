#pragma once

class CLevel;

class CLevelMgr
	:public CSingletone<CLevelMgr>
{
private:
	CLevel*			m_pCurLevel;

public:
	void init();
	void tick();
	void finaltick();
	void render();

	CLevelMgr();
	~CLevelMgr();
};

