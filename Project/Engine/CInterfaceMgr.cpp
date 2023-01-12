#include "pch.h"
#include "CInterfaceMgr.h"
#include "CGameObject.h"


CInterfaceMgr::CInterfaceMgr()
{

}

CInterfaceMgr::~CInterfaceMgr()
{

}

void CInterfaceMgr::AddTapButton(CGameObject* _pGameObject)
{
	m_vecTapButton.push_back(_pGameObject);
}
