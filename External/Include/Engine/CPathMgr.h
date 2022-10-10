#pragma once

class CPathMgr
	:public CSingletone<CPathMgr>
{
private:
	wchar_t		m_szContentPath[256];

public:
	void init();

	const wchar_t* GetContentPath() { return m_szContentPath; }
	
	CPathMgr();
	~CPathMgr();
};

