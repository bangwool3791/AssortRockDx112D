#include "pch.h"
#include "CPrefab.h"

#include "CGameObject.h"

CPrefab::CPrefab()
	:CRes(RES_TYPE::PREFAB)
	, m_pProtoObj{nullptr}
{

}

CPrefab::CPrefab(CGameObject* _pProto)
	:CRes(RES_TYPE::PREFAB)
	, m_pProtoObj{_pProto}
{
	if (-1 != _pProto->GetLayerIndex())
	{
		m_pProtoObj = _pProto->Clone();
	}
}

CPrefab::~CPrefab()
{
	if (m_pProtoObj)
		delete m_pProtoObj;
}

CGameObject* CPrefab::Instantiate() 
{
	if (m_pProtoObj)
		return m_pProtoObj->Clone();

	return nullptr;
}
