#include "pch.h"
#include "CTreeScript.h"

#include <Engine\CDevice.h>
#include <Engine\CLevel.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CTransform.h>

#include <Engine\CInterfaceMgr.h>
#include <Script\CMouseScript.h>

CTreeScript::CTreeScript()
	:CScript{ SCRIPT_TYPE::TREESCRIPT }
	, m_vMousePos{}
	, m_pTileObject{}
{
	SetName(L"CTreeScript");
}

CTreeScript::~CTreeScript()
{
}

void CTreeScript::begin()
{
	Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\vegetation\\Atlas1_HQ.dds");

	m_pTileObject = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"LevelTile");

	GetOwner()->GetRenderComponent()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\vegetation\\Atlas1_HQ.dds"));
	GetOwner()->GetRenderComponent()->SetInstancingType(INSTANCING_TYPE::USED);


	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();

	tTile tTile = m_pTileObject->TileMap()->GetInfo(vPos);

	GetOwner()->Transform()->SetRelativePos(tTile.vPos);

	SetTile(tTile.iIndex, (UINT)TILE_TYPE::WOOD);

	m_pTileObject->TileMap()->On();
}

void CTreeScript::tick()
{

}

void CTreeScript::finaltick()
{
}

void CTreeScript::SetTileInfo(UINT _iTile, UINT _iValue)
{
	m_pTileObject->TileMap()->SetInfo(_iTile, _iValue);
}

void  CTreeScript::SetTile(UINT _iTile, UINT _iValue)
{
	SetTileInfo(_iTile, _iValue);
}
