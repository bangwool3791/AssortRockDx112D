#include "pch.h"

#include "CMesh.h"
#include "CGraphicsShader.h"

#include "CGameObject.h"
#include "CComponent.h"
#include "CMeshRender.h"
#include "CScript.h"


template<typename T>
void Tick(T* obj) { obj->tick(); }

template<typename T>
void FinalTick(T* obj) { obj->finaltick(); }

CGameObject::CGameObject()
	:m_arrCom{}
{

}

CGameObject::~CGameObject()
{
	Safe_Del_Array(m_arrCom.begin(), m_arrCom.end());
	Safe_Del_Array(m_vecScripts.begin(), m_vecScripts.end());
}

void CGameObject::begin()
{

}

void CGameObject::tick()
{
	for (auto Iter{ m_arrCom.begin() }; Iter != m_arrCom.end(); ++Iter)
	{
		(*Iter)->tick();
	}

	for (auto Iter{ m_vecScripts.begin() }; Iter != m_vecScripts.end(); ++Iter)
	{
		(*Iter)->tick();
	}
}

void CGameObject::finaltick()
{
	for (auto Iter{ m_arrCom.begin() }; Iter != m_arrCom.end(); ++Iter)
	{
		(*Iter)->finaltick();
	}
}

void CGameObject::render()
{
	if (MeshRender() != nullptr)
	{
		MeshRender()->render();
	}
}

void CGameObject::AddComponent(CComponent* _pComponent)
{
	auto eComType = _pComponent->GetType();

	//[220930] 아래 코드 없을 때, m_arrCom에서 포인터 못찾음
	_pComponent->m_pOwnerObject = this;

	switch (eComType)
	{
	case TRANSFORM:
	case MESHRENDER:
		assert(!m_arrCom[(UINT)eComType]);
		m_arrCom[(UINT)eComType] = _pComponent;
		break;
	case SCRIPT:
		m_vecScripts.push_back((CScript*)_pComponent);
		break;
	}
}

CComponent* CGameObject::GetComponent(COMPONENT_TYPE _eComType)
{
	//for (auto iter{ m_arrCom.begin() }; iter != m_arrCom.end(); ++iter)
	//{
	//	if ((*iter)->GetType() == _eComType)
	//		return(*iter);
	//}
	return m_arrCom[(UINT)_eComType];
}