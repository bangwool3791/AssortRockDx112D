#pragma once

class CLevel;

class CLevelMgr
{
	SINGLE(CLevelMgr);
private:
	CLevel*			m_pCurLevel;

public:
	void init();
	void tick();
	void finaltick();
	void render();
};

