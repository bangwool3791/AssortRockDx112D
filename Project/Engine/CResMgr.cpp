#include "pch.h"
#include "CResMgr.h"

CResMgr::CResMgr()
	:m_vecLayoutInfo{}
	, m_iLayoutOffset{0}
{
	for (UINT i{ 0 }; i < (UINT)RES_TYPE::END; ++i)
	{
		m_arrRes[i] = {};
	}
}

CResMgr::~CResMgr()
{
	for (UINT i{ 0 }; i < (UINT)RES_TYPE::END; ++i)
	{
		Safe_Del_Map(m_arrRes[i]);
	}
}

bool CResMgr::DeleteRes(RES_TYPE _Type, const wstring& _strKey)
{
	map<wstring, Ptr<CRes>>::iterator iter = m_arrRes[(UINT)_Type].find(_strKey);

	if (m_arrRes[(UINT)_Type].end() != iter)
	{
		m_arrRes[(UINT)_Type].erase(iter);
		m_bChanged = true;
		return true;
	}

	return false;
}

