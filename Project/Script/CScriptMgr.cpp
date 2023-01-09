#include "pch.h"
#include "CScriptMgr.h"

#include "CDragScript.h"
#include "CEditorMouseScript.h"
#include "CLevelCameraScript.h"
#include "CLightScript.h"
#include "CMissileScript.h"
#include "CMissileScript2.h"
#include "CMonsterScript.h"
#include "CMouseScript.h"
#include "CPlayerScript.h"
#include "CRefAniScript.h"
#include "CSelectUnitScript.h"
#include "CShadowScript.h"
#include "CTerrainScript.h"
#include "CTileScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CDragScript");
	_vec.push_back(L"CEditorMouseScript");
	_vec.push_back(L"CLevelCameraScript");
	_vec.push_back(L"CLightScript");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CMissileScript2");
	_vec.push_back(L"CMonsterScript");
	_vec.push_back(L"CMouseScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CRefAniScript");
	_vec.push_back(L"CSelectUnitScript");
	_vec.push_back(L"CShadowScript");
	_vec.push_back(L"CTerrainScript");
	_vec.push_back(L"CTileScript");
}


void CScriptMgr::GetScriptInfo(vector<string>& _vec)
{
	_vec.push_back("CDragScript");
	_vec.push_back("CEditorMouseScript");
	_vec.push_back("CLevelCameraScript");
	_vec.push_back("CLightScript");
	_vec.push_back("CMissileScript");
	_vec.push_back("CMissileScript2");
	_vec.push_back("CMonsterScript");
	_vec.push_back("CMouseScript");
	_vec.push_back("CPlayerScript");
	_vec.push_back("CRefAniScript");
	_vec.push_back("CSelectUnitScript");
	_vec.push_back("CShadowScript");
	_vec.push_back("CTerrainScript");
	_vec.push_back("CTileScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CDragScript" == _strScriptName)
		return new CDragScript;
	if (L"CEditorMouseScript" == _strScriptName)
		return new CEditorMouseScript;
	if (L"CLevelCameraScript" == _strScriptName)
		return new CLevelCameraScript;
	if (L"CLightScript" == _strScriptName)
		return new CLightScript;
	if (L"CMissileScript" == _strScriptName)
		return new CMissileScript;
	if (L"CMissileScript2" == _strScriptName)
		return new CMissileScript2;
	if (L"CMonsterScript" == _strScriptName)
		return new CMonsterScript;
	if (L"CMouseScript" == _strScriptName)
		return new CMouseScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CRefAniScript" == _strScriptName)
		return new CRefAniScript;
	if (L"CSelectUnitScript" == _strScriptName)
		return new CSelectUnitScript;
	if (L"CShadowScript" == _strScriptName)
		return new CShadowScript;
	if (L"CTerrainScript" == _strScriptName)
		return new CTerrainScript;
	if (L"CTileScript" == _strScriptName)
		return new CTileScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(const string& _strScriptName)
{
	if ("CDragScript" == _strScriptName)
		return new CDragScript;
	if ("CEditorMouseScript" == _strScriptName)
		return new CEditorMouseScript;
	if ("CLevelCameraScript" == _strScriptName)
		return new CLevelCameraScript;
	if ("CLightScript" == _strScriptName)
		return new CLightScript;
	if ("CMissileScript" == _strScriptName)
		return new CMissileScript;
	if ("CMissileScript2" == _strScriptName)
		return new CMissileScript2;
	if ("CMonsterScript" == _strScriptName)
		return new CMonsterScript;
	if ("CMouseScript" == _strScriptName)
		return new CMouseScript;
	if ("CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if ("CRefAniScript" == _strScriptName)
		return new CRefAniScript;
	if ("CSelectUnitScript" == _strScriptName)
		return new CSelectUnitScript;
	if ("CShadowScript" == _strScriptName)
		return new CShadowScript;
	if ("CTerrainScript" == _strScriptName)
		return new CTerrainScript;
	if ("CTileScript" == _strScriptName)
		return new CTileScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::DRAGSCRIPT:
		return new CDragScript;
		break;
	case (UINT)SCRIPT_TYPE::EDITORMOUSESCRIPT:
		return new CEditorMouseScript;
		break;
	case (UINT)SCRIPT_TYPE::LEVELCAMERASCRIPT:
		return new CLevelCameraScript;
		break;
	case (UINT)SCRIPT_TYPE::LIGHTSCRIPT:
		return new CLightScript;
		break;
	case (UINT)SCRIPT_TYPE::MISSILESCRIPT:
		return new CMissileScript;
		break;
	case (UINT)SCRIPT_TYPE::MISSILESCRIPT2:
		return new CMissileScript2;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERSCRIPT:
		return new CMonsterScript;
		break;
	case (UINT)SCRIPT_TYPE::MOUSESCRIPT:
		return new CMouseScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::REFANISCRIPT:
		return new CRefAniScript;
		break;
	case (UINT)SCRIPT_TYPE::SELECTUNITSCRIPT:
		return new CSelectUnitScript;
		break;
	case (UINT)SCRIPT_TYPE::SHADOWSCRIPT:
		return new CShadowScript;
		break;
	case (UINT)SCRIPT_TYPE::TERRAINSCRIPT:
		return new CTerrainScript;
		break;
	case (UINT)SCRIPT_TYPE::TILESCRIPT:
		return new CTileScript;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptWName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::DRAGSCRIPT:
		return L"CDragScript";
		break;

	case SCRIPT_TYPE::EDITORMOUSESCRIPT:
		return L"CEditorMouseScript";
		break;

	case SCRIPT_TYPE::LEVELCAMERASCRIPT:
		return L"CLevelCameraScript";
		break;

	case SCRIPT_TYPE::LIGHTSCRIPT:
		return L"CLightScript";
		break;

	case SCRIPT_TYPE::MISSILESCRIPT:
		return L"CMissileScript";
		break;

	case SCRIPT_TYPE::MISSILESCRIPT2:
		return L"CMissileScript2";
		break;

	case SCRIPT_TYPE::MONSTERSCRIPT:
		return L"CMonsterScript";
		break;

	case SCRIPT_TYPE::MOUSESCRIPT:
		return L"CMouseScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::REFANISCRIPT:
		return L"CRefAniScript";
		break;

	case SCRIPT_TYPE::SELECTUNITSCRIPT:
		return L"CSelectUnitScript";
		break;

	case SCRIPT_TYPE::SHADOWSCRIPT:
		return L"CShadowScript";
		break;

	case SCRIPT_TYPE::TERRAINSCRIPT:
		return L"CTerrainScript";
		break;

	case SCRIPT_TYPE::TILESCRIPT:
		return L"CTileScript";
		break;

	}
	return nullptr;
}

const char* CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::DRAGSCRIPT:
		return "CDragScript";
		break;

	case SCRIPT_TYPE::EDITORMOUSESCRIPT:
		return "CEditorMouseScript";
		break;

	case SCRIPT_TYPE::LEVELCAMERASCRIPT:
		return "CLevelCameraScript";
		break;

	case SCRIPT_TYPE::LIGHTSCRIPT:
		return "CLightScript";
		break;

	case SCRIPT_TYPE::MISSILESCRIPT:
		return "CMissileScript";
		break;

	case SCRIPT_TYPE::MISSILESCRIPT2:
		return "CMissileScript2";
		break;

	case SCRIPT_TYPE::MONSTERSCRIPT:
		return "CMonsterScript";
		break;

	case SCRIPT_TYPE::MOUSESCRIPT:
		return "CMouseScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return "CPlayerScript";
		break;

	case SCRIPT_TYPE::REFANISCRIPT:
		return "CRefAniScript";
		break;

	case SCRIPT_TYPE::SELECTUNITSCRIPT:
		return "CSelectUnitScript";
		break;

	case SCRIPT_TYPE::SHADOWSCRIPT:
		return "CShadowScript";
		break;

	case SCRIPT_TYPE::TERRAINSCRIPT:
		return "CTerrainScript";
		break;

	case SCRIPT_TYPE::TILESCRIPT:
		return "CTileScript";
		break;

	}
	return nullptr;
}