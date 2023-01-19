#include "pch.h"
#include "CInterfaceMgr.h"
#include "CGameObject.h"


CInterfaceMgr::CInterfaceMgr()
{

}

CInterfaceMgr::~CInterfaceMgr()
{

}

CGameObject* CInterfaceMgr::GetTarget()
{
	if(m_pTarget)
		if (m_pTarget->IsDead())
			m_pTarget = nullptr;

	return m_pTarget;
}

void CInterfaceMgr::AddTapButton(CGameObject* _pGameObject)
{
	m_vecTapButton.push_back(_pGameObject);
}
