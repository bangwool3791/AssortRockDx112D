#include "pch.h"

#include "CMesh.h"
#include "CGraphicsShader.h"

#include "CGameObject.h"
#include "CComponent.h"
#include "CMeshRender.h"


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
}

void CGameObject::tick()
{
	for (auto Iter{ m_arrCom.begin() }; Iter != m_arrCom.end(); ++Iter)
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

	assert(!m_arrCom[(UINT)eComType]);

	//[220930] 아래 코드 없을 때, m_arrCom에서 포인터 못찾음
	_pComponent->m_pOwnerObject = this;
	m_arrCom[(UINT)eComType] = _pComponent;
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