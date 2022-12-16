#pragma once

#include <vector>
#include <string>

enum SCRIPT_TYPE
{
	DRAGSCRIPT,
	EDITORMOUSESCRIPT,
	LIGHTSCRIPT,
	MISSILESCRIPT,
	MISSILESCRIPT2,
	MONSTERSCRIPT,
	MOUSESCRIPT,
	PLAYERSCRIPT,
	REFANISCRIPT,
	EDITOR_MOUSESCRIPT,
	SELECTUNITSCRIPT,
	SHADOWSCRIPT,
	TILESCRIPT,
	HREADTILEMAP,
	END,
};

using namespace std;

class CScript;

class CScriptMgr
{
public:
	static void GetScriptInfo(vector<wstring>& _vec);
	static void GetScriptInfo(vector<string>& _vec);
	static CScript* GetScript(const wstring& _strScriptName);
	static CScript* GetScript(const string& _strScriptName);
	static CScript* GetScript(UINT _iScriptType);
	static const wchar_t* GetScriptWName(CScript* _pScript);
	static const char* GetScriptName(CScript* _pScript);
};
