#pragma once

#include <vector>
#include <string>

enum SCRIPT_TYPE
{
	BUTTONSCRIPT,
	COMMANDSCRIPT,
	CRYSTALSCRIPT,
	DRAGSCRIPT,
	EDITORMOUSESCRIPT,
	HUNTSCRIPT,
	INTERFACESCRIPT,
	LEVELCAMERASCRIPT,
	LIGHTSCRIPT,
	MISSILESCRIPT,
	MISSILESCRIPT2,
	MONSTERSCRIPT,
	MOUSESCRIPT,
	PLAYERSCRIPT,
	POSSCRIPT,
	QUARRYSCRIPT,
	REFANISCRIPT,
	SAWSCRIPT,
	SCSCRIPT,
	SELECTUNITSCRIPT,
	SHADOWSCRIPT,
	SOLDIERSCRIPT,
	TENTSCRIPT,
	TERRAINSCRIPT,
	TILESCRIPT,
	TREESCRIPT,
	WWALLSCRIPT,
	WWCSCRIPT,
	WWSSCRIPT,
	END,
};

using namespace std;

class CScript;

class CScriptMgr
{
public:
	static void GetScriptInfo(vector<wstring>& _vec);
	static void GetScriptInfo(vector<string>& _vec);
	static CScript * GetScript(const wstring& _strScriptName);
	static CScript * GetScript(const string& _strScriptName);
	static CScript * GetScript(UINT _iScriptType);
	static const wchar_t * GetScriptWName(CScript * _pScript);
	static const char* GetScriptName(CScript * _pScript);
};
