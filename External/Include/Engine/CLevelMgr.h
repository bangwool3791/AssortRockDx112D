#pragma once

class CLevel;

class CLevelMgr
	:public CSingleton<CLevelMgr>
{
private:
	CLevel*			m_pCurLevel;

public:
	CLevel* GetCurLevel()
	{
		return m_pCurLevel;
	}
	CGameObject* FindObjectByName(const wstring& _strName);
	CGameObject* FindSelectedObject(const wstring& _strName);
public:
	void init();
	void tick();
	void finaltick();
	void render();

	CLevelMgr();
	virtual ~CLevelMgr();
};

