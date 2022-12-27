#pragma once

#include "pch.h"

class CLevel;
class CLayer;
class CGameObject;

class CSaveLoadMgr
	:public CSingleton<CSaveLoadMgr>
{
private:

public :
	void SaveLevel(CLevel* _Level, wstring _strRelativePath);
	void SaveGameObject(CGameObject* _Object, FILE* _File);
	CLevel* LoadLevel(const wstring& _strRelativePath);
	CGameObject* LoadGameObject(FILE* _File);
private:
	CSaveLoadMgr();
	~CSaveLoadMgr();

	friend class CSingleton<CSaveLoadMgr>;
};