#pragma once

#include <vector>
#include <string>

enum SCRIPT_TYPE
{
	ARROWSCRIPT,
	ASSISTSCRIPT,
	BUTTONSCRIPT,
	CIRCLEARROWSCRIPT,
	COMMANDSCRIPT,
	CRYSTALSCRIPT,
	DRAGSCRIPT,
	EDITORMOUSESCRIPT,
	EFFECTSCRIPT,
	HILLSCRIPT,
	HUNTSCRIPT,
	INFECTEDGIANTSCRIPT,
	INFECTEDMEDIUM_A,
	INFECTEDMEDIUM_B,
	INFECTEDSTRONG_A,
	INFECTEDVENOM,
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
	RANGERSCRIPT,
	REFANISCRIPT,
	SAWSCRIPT,
	SCSCRIPT,
	SELECTUNITSCRIPT,
	SHADOWSCRIPT,
	SNIPERSCRIPT,
	SOLDIERSCRIPT,
	TENTSCRIPT,
	TERRAINSCRIPT,
	TESLATOWERSCRIPT,
	TILESCRIPT,
	TITANSCRIPT,
	TREESCRIPT,
	UISCRIPT,
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
