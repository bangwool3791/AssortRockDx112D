#include "pch.h"
#include "InterfaceScript.h"

#include <Engine\CDevice.h>
#include <Engine\CLevel.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CTransform.h>
#include <Engine\CTerrain.h>


InterfaceScript::InterfaceScript()
	:CScript{ INTERFACESCRIPT }
	, m_id{}
{
	SetName(L"InterfaceScript");
}

InterfaceScript::~InterfaceScript()
{
}


void InterfaceScript::begin()
{
}
//레이식 피킹으로 수정
void InterfaceScript::tick()
{

}

void InterfaceScript::BeginOverlap(CCollider2D* _pOther)
{
}

void InterfaceScript::Overlap(CCollider2D* _pOther)
{
}

void InterfaceScript::EndOverlap(CCollider2D* _pOther)
{
}
