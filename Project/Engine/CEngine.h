#pragma once


class CEngine
	: public CSingleton<CEngine>
{

private:
	HWND		m_hMainWnd;
	Vec2		m_vResolution;	// 윈도우 해상도


public:
	int init(HWND _hWnd, UINT _iWidth, UINT _iHeight);
	void progress();

private:
	void tick();
	void finaltick();
	void render();

private:
	CEngine();
	virtual ~CEngine();
	friend class CSingleton<CEngine>;

public :
	static wchar_t g_szHp[255];
	static wchar_t g_szFullName[255];
	static vector<pair<wstring, Vec2>> g_IconText;
public:
	HWND GetMainHwnd() { return m_hMainWnd; }
};

