#pragma once

class CEngine
{
	SINGLE(CEngine);

private:
	HWND m_hMainWnd;
	Vec2 m_vResolution;

public :
	int init(HWND _hWnd, UINT _iWidht, UINT _iHeight);
	void progress();

private:
	void tick();
	void render();

public :
	HWND GetMainHwnd() { return m_hMainWnd; }
};

