#include "pch.h"
#include "CCrystalScript.h"

#include <Engine\CDevice.h>
#include <Engine\CLevel.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CTransform.h>

#include <Engine\CInterfaceMgr.h>
#include <Script\CMouseScript.h>

CCrystalScript::CCrystalScript()
	:CScript{ SCRIPT_TYPE::CRYSTALSCRIPT }
	, m_vMousePos{}
	, m_pTileObject{}
{
	SetName(L"CCrystalScript");
}

CCrystalScript::~CCrystalScript()
{
}

void CCrystalScript::begin()
{
	Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\geology\\Atlas1_LQ.dds");

	m_pTileObject = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"LevelTile");

	GetOwner()->GetRenderComponent()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\geology\\Atlas1_LQ.dds"));
	GetOwner()->GetRenderComponent()->SetInstancingType(INSTANCING_TYPE::USED);


	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();

	tTile tTile = m_pTileObject->TileMap()->GetInfo(vPos);

	GetOwner()->Transform()->SetRelativePos(vPos);

	SetTile(tTile.iIndex, (UINT)TILE_TYPE::CRYSTAL);

	m_pTileObject->TileMap()->On();
}

void CCrystalScript::tick()
{

}

void CCrystalScript::finaltick()
{
}

void CCrystalScript::SetTileInfo(UINT _iTile, UINT _iValue)
{
	m_pTileObject->TileMap()->SetInfo(_iTile, _iValue);
}

void  CCrystalScript::SetTile(UINT _iTile, UINT _iValue)
{
	SetTileInfo(_iTile, _iValue);
}
