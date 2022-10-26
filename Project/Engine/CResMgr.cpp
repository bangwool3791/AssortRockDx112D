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
