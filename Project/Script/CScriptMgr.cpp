#include "pch.h"
#include "CScriptMgr.h"

#include "CArrowScript.h"
#include "CAssistScript.h"
#include "CButtonScript.h"
#include "CCircleArrowScript.h"
#include "CCommandScript.h"
#include "CCrystalScript.h"
#include "CDragScript.h"
#include "CEditorMouseScript.h"
#include "CEffectScript.h"
#include "CHillScript.h"
#include "CHuntScript.h"
#include "CInfectedGiantScript.h"
#include "CInfectedMedium_A.h"
#include "CInfectedMedium_B.h"
#include "CInfectedStrong_A.h"
#include "CInfectedVenom.h"
#include "CInterfaceScript.h"
#include "CLevelCameraScript.h"
#include "CLightScript.h"
#include "CMissileScript.h"
#include "CMissileScript2.h"
#include "CMonsterScript.h"
#include "CMouseScript.h"
#include "CPlayerScript.h"
#include "CPOSScript.h"
#include "CQuarryScript.h"
#include "CRangerScript.h"
#include "CRefAniScript.h"
#include "CSawScript.h"
#include "CSCScript.h"
#include "CSelectUnitScript.h"
#include "CShadowScript.h"
#include "CSniperScript.h"
#include "CSoldierScript.h"
#include "CTentScript.h"
#include "CTerrainScript.h"
#include "CTeslaTowerScript.h"
#include "CTileScript.h"
#include "CTitanScript.h"
#include "CTreeScript.h"
#include "CUiScript.h"
#include "CWWallScript.h"
#include "CWWCScript.h"
#include "CWWSScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CArrowScript");
	_vec.push_back(L"CAssistScript");
	_vec.push_back(L"CButtonScript");
	_vec.push_back(L"CCircleArrowScript");
	_vec.push_back(L"CCommandScript");
	_vec.push_back(L"CCrystalScript");
	_vec.push_back(L"CDragScript");
	_vec.push_back(L"CEditorMouseScript");
	_vec.push_back(L"CEffectScript");
	_vec.push_back(L"CHillScript");
	_vec.push_back(L"CHuntScript");
	_vec.push_back(L"CInfectedGiantScript");
	_vec.push_back(L"CInfectedMedium_A");
	_vec.push_back(L"CInfectedMedium_B");
	_vec.push_back(L"CInfectedStrong_A");
	_vec.push_back(L"CInfectedVenom");
	_vec.push_back(L"CInterfaceScript");
	_vec.push_back(L"CLevelCameraScript");
	_vec.push_back(L"CLightScript");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CMissileScript2");
	_vec.push_back(L"CMonsterScript");
	_vec.push_back(L"CMouseScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CPOSScript");
	_vec.push_back(L"CQuarryScript");
	_vec.push_back(L"CRangerScript");
	_vec.push_back(L"CRefAniScript");
	_vec.push_back(L"CSawScript");
	_vec.push_back(L"CSCScript");
	_vec.push_back(L"CSelectUnitScript");
	_vec.push_back(L"CShadowScript");
	_vec.push_back(L"CSniperScript");
	_vec.push_back(L"CSoldierScript");
	_vec.push_back(L"CTentScript");
	_vec.push_back(L"CTerrainScript");
	_vec.push_back(L"CTeslaTowerScript");
	_vec.push_back(L"CTileScript");
	_vec.push_back(L"CTitanScript");
	_vec.push_back(L"CTreeScript");
	_vec.push_back(L"CUiScript");
	_vec.push_back(L"CWWallScript");
	_vec.push_back(L"CWWCScript");
	_vec.push_back(L"CWWSScript");
}


void CScriptMgr::GetScriptInfo(vector<string>& _vec)
{
	_vec.push_back("CArrowScript");
	_vec.push_back("CAssistScript");
	_vec.push_back("CButtonScript");
	_vec.push_back("CCircleArrowScript");
	_vec.push_back("CCommandScript");
	_vec.push_back("CCrystalScript");
	_vec.push_back("CDragScript");
	_vec.push_back("CEditorMouseScript");
	_vec.push_back("CEffectScript");
	_vec.push_back("CHillScript");
	_vec.push_back("CHuntScript");
	_vec.push_back("CInfectedGiantScript");
	_vec.push_back("CInfectedMedium_A");
	_vec.push_back("CInfectedMedium_B");
	_vec.push_back("CInfectedStrong_A");
	_vec.push_back("CInfectedVenom");
	_vec.push_back("CInterfaceScript");
	_vec.push_back("CLevelCameraScript");
	_vec.push_back("CLightScript");
	_vec.push_back("CMissileScript");
	_vec.push_back("CMissileScript2");
	_vec.push_back("CMonsterScript");
	_vec.push_back("CMouseScript");
	_vec.push_back("CPlayerScript");
	_vec.push_back("CPOSScript");
	_vec.push_back("CQuarryScript");
	_vec.push_back("CRangerScript");
	_vec.push_back("CRefAniScript");
	_vec.push_back("CSawScript");
	_vec.push_back("CSCScript");
	_vec.push_back("CSelectUnitScript");
	_vec.push_back("CShadowScript");
	_vec.push_back("CSniperScript");
	_vec.push_back("CSoldierScript");
	_vec.push_back("CTentScript");
	_vec.push_back("CTerrainScript");
	_vec.push_back("CTeslaTowerScript");
	_vec.push_back("CTileScript");
	_vec.push_back("CTitanScript");
	_vec.push_back("CTreeScript");
	_vec.push_back("CUiScript");
	_vec.push_back("CWWallScript");
	_vec.push_back("CWWCScript");
	_vec.push_back("CWWSScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CArrowScript" == _strScriptName)
		return new CArrowScript;
	if (L"CAssistScript" == _strScriptName)
		return new CAssistScript;
	if (L"CButtonScript" == _strScriptName)
		return new CButtonScript;
	if (L"CCircleArrowScript" == _strScriptName)
		return new CCircleArrowScript;
	if (L"CCommandScript" == _strScriptName)
		return new CCommandScript;
	if (L"CCrystalScript" == _strScriptName)
		return new CCrystalScript;
	if (L"CDragScript" == _strScriptName)
		return new CDragScript;
	if (L"CEditorMouseScript" == _strScriptName)
		return new CEditorMouseScript;
	if (L"CEffectScript" == _strScriptName)
		return new CEffectScript;
	if (L"CHillScript" == _strScriptName)
		return new CHillScript;
	if (L"CHuntScript" == _strScriptName)
		return new CHuntScript;
	if (L"CInfectedGiantScript" == _strScriptName)
		return new CInfectedGiantScript;
	if (L"CInfectedMedium_A" == _strScriptName)
		return new CInfectedMedium_A;
	if (L"CInfectedMedium_B" == _strScriptName)
		return new CInfectedMedium_B;
	if (L"CInfectedStrong_A" == _strScriptName)
		return new CInfectedStrong_A;
	if (L"CInfectedVenom" == _strScriptName)
		return new CInfectedVenom;
	if (L"CInterfaceScript" == _strScriptName)
		return new CInterfaceScript;
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
	if (L"CPOSScript" == _strScriptName)
		return new CPOSScript;
	if (L"CQuarryScript" == _strScriptName)
		return new CQuarryScript;
	if (L"CRangerScript" == _strScriptName)
		return new CRangerScript;
	if (L"CRefAniScript" == _strScriptName)
		return new CRefAniScript;
	if (L"CSawScript" == _strScriptName)
		return new CSawScript;
	if (L"CSCScript" == _strScriptName)
		return new CSCScript;
	if (L"CSelectUnitScript" == _strScriptName)
		return new CSelectUnitScript;
	if (L"CShadowScript" == _strScriptName)
		return new CShadowScript;
	if (L"CSniperScript" == _strScriptName)
		return new CSniperScript;
	if (L"CSoldierScript" == _strScriptName)
		return new CSoldierScript;
	if (L"CTentScript" == _strScriptName)
		return new CTentScript;
	if (L"CTerrainScript" == _strScriptName)
		return new CTerrainScript;
	if (L"CTeslaTowerScript" == _strScriptName)
		return new CTeslaTowerScript;
	if (L"CTileScript" == _strScriptName)
		return new CTileScript;
	if (L"CTitanScript" == _strScriptName)
		return new CTitanScript;
	if (L"CTreeScript" == _strScriptName)
		return new CTreeScript;
	if (L"CUiScript" == _strScriptName)
		return new CUiScript;
	if (L"CWWallScript" == _strScriptName)
		return new CWWallScript;
	if (L"CWWCScript" == _strScriptName)
		return new CWWCScript;
	if (L"CWWSScript" == _strScriptName)
		return new CWWSScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(const string& _strScriptName)
{
	if ("CArrowScript" == _strScriptName)
		return new CArrowScript;
	if ("CAssistScript" == _strScriptName)
		return new CAssistScript;
	if ("CButtonScript" == _strScriptName)
		return new CButtonScript;
	if ("CCircleArrowScript" == _strScriptName)
		return new CCircleArrowScript;
	if ("CCommandScript" == _strScriptName)
		return new CCommandScript;
	if ("CCrystalScript" == _strScriptName)
		return new CCrystalScript;
	if ("CDragScript" == _strScriptName)
		return new CDragScript;
	if ("CEditorMouseScript" == _strScriptName)
		return new CEditorMouseScript;
	if ("CEffectScript" == _strScriptName)
		return new CEffectScript;
	if ("CHillScript" == _strScriptName)
		return new CHillScript;
	if ("CHuntScript" == _strScriptName)
		return new CHuntScript;
	if ("CInfectedGiantScript" == _strScriptName)
		return new CInfectedGiantScript;
	if ("CInfectedMedium_A" == _strScriptName)
		return new CInfectedMedium_A;
	if ("CInfectedMedium_B" == _strScriptName)
		return new CInfectedMedium_B;
	if ("CInfectedStrong_A" == _strScriptName)
		return new CInfectedStrong_A;
	if ("CInfectedVenom" == _strScriptName)
		return new CInfectedVenom;
	if ("CInterfaceScript" == _strScriptName)
		return new CInterfaceScript;
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
	if ("CPOSScript" == _strScriptName)
		return new CPOSScript;
	if ("CQuarryScript" == _strScriptName)
		return new CQuarryScript;
	if ("CRangerScript" == _strScriptName)
		return new CRangerScript;
	if ("CRefAniScript" == _strScriptName)
		return new CRefAniScript;
	if ("CSawScript" == _strScriptName)
		return new CSawScript;
	if ("CSCScript" == _strScriptName)
		return new CSCScript;
	if ("CSelectUnitScript" == _strScriptName)
		return new CSelectUnitScript;
	if ("CShadowScript" == _strScriptName)
		return new CShadowScript;
	if ("CSniperScript" == _strScriptName)
		return new CSniperScript;
	if ("CSoldierScript" == _strScriptName)
		return new CSoldierScript;
	if ("CTentScript" == _strScriptName)
		return new CTentScript;
	if ("CTerrainScript" == _strScriptName)
		return new CTerrainScript;
	if ("CTeslaTowerScript" == _strScriptName)
		return new CTeslaTowerScript;
	if ("CTileScript" == _strScriptName)
		return new CTileScript;
	if ("CTitanScript" == _strScriptName)
		return new CTitanScript;
	if ("CTreeScript" == _strScriptName)
		return new CTreeScript;
	if ("CUiScript" == _strScriptName)
		return new CUiScript;
	if ("CWWallScript" == _strScriptName)
		return new CWWallScript;
	if ("CWWCScript" == _strScriptName)
		return new CWWCScript;
	if ("CWWSScript" == _strScriptName)
		return new CWWSScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::ARROWSCRIPT:
		return new CArrowScript;
		break;
	case (UINT)SCRIPT_TYPE::ASSISTSCRIPT:
		return new CAssistScript;
		break;
	case (UINT)SCRIPT_TYPE::BUTTONSCRIPT:
		return new CButtonScript;
		break;
	case (UINT)SCRIPT_TYPE::CIRCLEARROWSCRIPT:
		return new CCircleArrowScript;
		break;
	case (UINT)SCRIPT_TYPE::COMMANDSCRIPT:
		return new CCommandScript;
		break;
	case (UINT)SCRIPT_TYPE::CRYSTALSCRIPT:
		return new CCrystalScript;
		break;
	case (UINT)SCRIPT_TYPE::DRAGSCRIPT:
		return new CDragScript;
		break;
	case (UINT)SCRIPT_TYPE::EDITORMOUSESCRIPT:
		return new CEditorMouseScript;
		break;
	case (UINT)SCRIPT_TYPE::EFFECTSCRIPT:
		return new CEffectScript;
		break;
	case (UINT)SCRIPT_TYPE::HILLSCRIPT:
		return new CHillScript;
		break;
	case (UINT)SCRIPT_TYPE::HUNTSCRIPT:
		return new CHuntScript;
		break;
	case (UINT)SCRIPT_TYPE::INFECTEDGIANTSCRIPT:
		return new CInfectedGiantScript;
		break;
	case (UINT)SCRIPT_TYPE::INFECTEDMEDIUM_A:
		return new CInfectedMedium_A;
		break;
	case (UINT)SCRIPT_TYPE::INFECTEDMEDIUM_B:
		return new CInfectedMedium_B;
		break;
	case (UINT)SCRIPT_TYPE::INFECTEDSTRONG_A:
		return new CInfectedStrong_A;
		break;
	case (UINT)SCRIPT_TYPE::INFECTEDVENOM:
		return new CInfectedVenom;
		break;
	case (UINT)SCRIPT_TYPE::INTERFACESCRIPT:
		return new CInterfaceScript;
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
	case (UINT)SCRIPT_TYPE::POSSCRIPT:
		return new CPOSScript;
		break;
	case (UINT)SCRIPT_TYPE::QUARRYSCRIPT:
		return new CQuarryScript;
		break;
	case (UINT)SCRIPT_TYPE::RANGERSCRIPT:
		return new CRangerScript;
		break;
	case (UINT)SCRIPT_TYPE::REFANISCRIPT:
		return new CRefAniScript;
		break;
	case (UINT)SCRIPT_TYPE::SAWSCRIPT:
		return new CSawScript;
		break;
	case (UINT)SCRIPT_TYPE::SCSCRIPT:
		return new CSCScript;
		break;
	case (UINT)SCRIPT_TYPE::SELECTUNITSCRIPT:
		return new CSelectUnitScript;
		break;
	case (UINT)SCRIPT_TYPE::SHADOWSCRIPT:
		return new CShadowScript;
		break;
	case (UINT)SCRIPT_TYPE::SNIPERSCRIPT:
		return new CSniperScript;
		break;
	case (UINT)SCRIPT_TYPE::SOLDIERSCRIPT:
		return new CSoldierScript;
		break;
	case (UINT)SCRIPT_TYPE::TENTSCRIPT:
		return new CTentScript;
		break;
	case (UINT)SCRIPT_TYPE::TERRAINSCRIPT:
		return new CTerrainScript;
		break;
	case (UINT)SCRIPT_TYPE::TESLATOWERSCRIPT:
		return new CTeslaTowerScript;
		break;
	case (UINT)SCRIPT_TYPE::TILESCRIPT:
		return new CTileScript;
		break;
	case (UINT)SCRIPT_TYPE::TITANSCRIPT:
		return new CTitanScript;
		break;
	case (UINT)SCRIPT_TYPE::TREESCRIPT:
		return new CTreeScript;
		break;
	case (UINT)SCRIPT_TYPE::UISCRIPT:
		return new CUiScript;
		break;
	case (UINT)SCRIPT_TYPE::WWALLSCRIPT:
		return new CWWallScript;
		break;
	case (UINT)SCRIPT_TYPE::WWCSCRIPT:
		return new CWWCScript;
		break;
	case (UINT)SCRIPT_TYPE::WWSSCRIPT:
		return new CWWSScript;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptWName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::ARROWSCRIPT:
		return L"CArrowScript";
		break;

	case SCRIPT_TYPE::ASSISTSCRIPT:
		return L"CAssistScript";
		break;

	case SCRIPT_TYPE::BUTTONSCRIPT:
		return L"CButtonScript";
		break;

	case SCRIPT_TYPE::CIRCLEARROWSCRIPT:
		return L"CCircleArrowScript";
		break;

	case SCRIPT_TYPE::COMMANDSCRIPT:
		return L"CCommandScript";
		break;

	case SCRIPT_TYPE::CRYSTALSCRIPT:
		return L"CCrystalScript";
		break;

	case SCRIPT_TYPE::DRAGSCRIPT:
		return L"CDragScript";
		break;

	case SCRIPT_TYPE::EDITORMOUSESCRIPT:
		return L"CEditorMouseScript";
		break;

	case SCRIPT_TYPE::EFFECTSCRIPT:
		return L"CEffectScript";
		break;

	case SCRIPT_TYPE::HILLSCRIPT:
		return L"CHillScript";
		break;

	case SCRIPT_TYPE::HUNTSCRIPT:
		return L"CHuntScript";
		break;

	case SCRIPT_TYPE::INFECTEDGIANTSCRIPT:
		return L"CInfectedGiantScript";
		break;

	case SCRIPT_TYPE::INFECTEDMEDIUM_A:
		return L"CInfectedMedium_A";
		break;

	case SCRIPT_TYPE::INFECTEDMEDIUM_B:
		return L"CInfectedMedium_B";
		break;

	case SCRIPT_TYPE::INFECTEDSTRONG_A:
		return L"CInfectedStrong_A";
		break;

	case SCRIPT_TYPE::INFECTEDVENOM:
		return L"CInfectedVenom";
		break;

	case SCRIPT_TYPE::INTERFACESCRIPT:
		return L"CInterfaceScript";
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

	case SCRIPT_TYPE::POSSCRIPT:
		return L"CPOSScript";
		break;

	case SCRIPT_TYPE::QUARRYSCRIPT:
		return L"CQuarryScript";
		break;

	case SCRIPT_TYPE::RANGERSCRIPT:
		return L"CRangerScript";
		break;

	case SCRIPT_TYPE::REFANISCRIPT:
		return L"CRefAniScript";
		break;

	case SCRIPT_TYPE::SAWSCRIPT:
		return L"CSawScript";
		break;

	case SCRIPT_TYPE::SCSCRIPT:
		return L"CSCScript";
		break;

	case SCRIPT_TYPE::SELECTUNITSCRIPT:
		return L"CSelectUnitScript";
		break;

	case SCRIPT_TYPE::SHADOWSCRIPT:
		return L"CShadowScript";
		break;

	case SCRIPT_TYPE::SNIPERSCRIPT:
		return L"CSniperScript";
		break;

	case SCRIPT_TYPE::SOLDIERSCRIPT:
		return L"CSoldierScript";
		break;

	case SCRIPT_TYPE::TENTSCRIPT:
		return L"CTentScript";
		break;

	case SCRIPT_TYPE::TERRAINSCRIPT:
		return L"CTerrainScript";
		break;

	case SCRIPT_TYPE::TESLATOWERSCRIPT:
		return L"CTeslaTowerScript";
		break;

	case SCRIPT_TYPE::TILESCRIPT:
		return L"CTileScript";
		break;

	case SCRIPT_TYPE::TITANSCRIPT:
		return L"CTitanScript";
		break;

	case SCRIPT_TYPE::TREESCRIPT:
		return L"CTreeScript";
		break;

	case SCRIPT_TYPE::UISCRIPT:
		return L"CUiScript";
		break;

	case SCRIPT_TYPE::WWALLSCRIPT:
		return L"CWWallScript";
		break;

	case SCRIPT_TYPE::WWCSCRIPT:
		return L"CWWCScript";
		break;

	case SCRIPT_TYPE::WWSSCRIPT:
		return L"CWWSScript";
		break;

	}
	return nullptr;
}

const char* CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::ARROWSCRIPT:
		return "CArrowScript";
		break;

	case SCRIPT_TYPE::ASSISTSCRIPT:
		return "CAssistScript";
		break;

	case SCRIPT_TYPE::BUTTONSCRIPT:
		return "CButtonScript";
		break;

	case SCRIPT_TYPE::CIRCLEARROWSCRIPT:
		return "CCircleArrowScript";
		break;

	case SCRIPT_TYPE::COMMANDSCRIPT:
		return "CCommandScript";
		break;

	case SCRIPT_TYPE::CRYSTALSCRIPT:
		return "CCrystalScript";
		break;

	case SCRIPT_TYPE::DRAGSCRIPT:
		return "CDragScript";
		break;

	case SCRIPT_TYPE::EDITORMOUSESCRIPT:
		return "CEditorMouseScript";
		break;

	case SCRIPT_TYPE::EFFECTSCRIPT:
		return "CEffectScript";
		break;

	case SCRIPT_TYPE::HILLSCRIPT:
		return "CHillScript";
		break;

	case SCRIPT_TYPE::HUNTSCRIPT:
		return "CHuntScript";
		break;

	case SCRIPT_TYPE::INFECTEDGIANTSCRIPT:
		return "CInfectedGiantScript";
		break;

	case SCRIPT_TYPE::INFECTEDMEDIUM_A:
		return "CInfectedMedium_A";
		break;

	case SCRIPT_TYPE::INFECTEDMEDIUM_B:
		return "CInfectedMedium_B";
		break;

	case SCRIPT_TYPE::INFECTEDSTRONG_A:
		return "CInfectedStrong_A";
		break;

	case SCRIPT_TYPE::INFECTEDVENOM:
		return "CInfectedVenom";
		break;

	case SCRIPT_TYPE::INTERFACESCRIPT:
		return "CInterfaceScript";
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

	case SCRIPT_TYPE::POSSCRIPT:
		return "CPOSScript";
		break;

	case SCRIPT_TYPE::QUARRYSCRIPT:
		return "CQuarryScript";
		break;

	case SCRIPT_TYPE::RANGERSCRIPT:
		return "CRangerScript";
		break;

	case SCRIPT_TYPE::REFANISCRIPT:
		return "CRefAniScript";
		break;

	case SCRIPT_TYPE::SAWSCRIPT:
		return "CSawScript";
		break;

	case SCRIPT_TYPE::SCSCRIPT:
		return "CSCScript";
		break;

	case SCRIPT_TYPE::SELECTUNITSCRIPT:
		return "CSelectUnitScript";
		break;

	case SCRIPT_TYPE::SHADOWSCRIPT:
		return "CShadowScript";
		break;

	case SCRIPT_TYPE::SNIPERSCRIPT:
		return "CSniperScript";
		break;

	case SCRIPT_TYPE::SOLDIERSCRIPT:
		return "CSoldierScript";
		break;

	case SCRIPT_TYPE::TENTSCRIPT:
		return "CTentScript";
		break;

	case SCRIPT_TYPE::TERRAINSCRIPT:
		return "CTerrainScript";
		break;

	case SCRIPT_TYPE::TESLATOWERSCRIPT:
		return "CTeslaTowerScript";
		break;

	case SCRIPT_TYPE::TILESCRIPT:
		return "CTileScript";
		break;

	case SCRIPT_TYPE::TITANSCRIPT:
		return "CTitanScript";
		break;

	case SCRIPT_TYPE::TREESCRIPT:
		return "CTreeScript";
		break;

	case SCRIPT_TYPE::UISCRIPT:
		return "CUiScript";
		break;

	case SCRIPT_TYPE::WWALLSCRIPT:
		return "CWWallScript";
		break;

	case SCRIPT_TYPE::WWCSCRIPT:
		return "CWWCScript";
		break;

	case SCRIPT_TYPE::WWSSCRIPT:
		return "CWWSScript";
		break;

	}
	return nullptr;
}