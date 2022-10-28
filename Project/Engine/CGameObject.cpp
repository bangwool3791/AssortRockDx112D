#include "pch.h"

#include "CMesh.h"
#include "CGraphicsShader.h"

#include "CLevel.h"
#include "CLayer.h"
#include "CLevelMgr.h"

#include "CGameObject.h"
#include "CComponent.h"
#include "CRenderComponent.h"
#include "CMeshRender.h"
#include "CScript.h"
#include "CCamera.h"


template<typename T>
void Tick(T* obj) { obj->tick(); }

template<typename T>
void FinalTick(T* obj) { obj->finaltick(); }

CGameObject::CGameObject()
	:m_arrCom{}
	, m_iLayerIdx{ -1 }
	, m_pParent{ nullptr }
	, m_pRenderComponent{ nullptr }
{

}

CGameObject::CGameObject(const CGameObject& rhs)
	:CEntity(rhs)
	,m_iLayerIdx{-1}
	, m_pParent{nullptr}
	, m_pRenderComponent{nullptr}
{
	for (size_t i{ 0 }; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (rhs.m_arrCom[i] == nullptr)
			continue;

		AddComponent(rhs.m_arrCom[i]->Clone());
	}

	for (size_t i{ 0 }; i < rhs.m_vecScripts.size(); ++i)
	{
		AddComponent(rhs.m_vecScripts[i]->Clone());
	}

	for (size_t i{ 0 }; i < rhs.m_vecChild.size(); ++i)
	{
		AddChild(rhs.m_vecChild[i]->Clone());
	}
}

CGameObject::~CGameObject()
{
	Safe_Del_Array(m_arrCom.begin(), m_arrCom.end());
	Safe_Del_Array(m_vecScripts.begin(), m_vecScripts.end());
	Safe_Del_Vec(m_vecChild);
}

void CGameObject::begin()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->begin();
	}

	for (size_t i = 0; i < m_vecScripts.size(); ++i)
	{
		m_vecScripts[i]->begin();
	}

	for (auto Iter{ m_vecChild.begin() }; Iter != m_vecChild.end(); ++Iter)
	{
		if ((*Iter))
			(*Iter)->begin();
	}

}

void CGameObject::tick()
{
	for (auto Iter{ m_arrCom.begin() }; Iter != m_arrCom.end(); ++Iter)
	{
		if((*Iter))
			(*Iter)->tick();
	}

	for (auto Iter{ m_vecScripts.begin() }; Iter != m_vecScripts.end(); ++Iter)
	{
		if ((*Iter))
			(*Iter)->tick();
	}

	for (auto Iter{ m_vecChild.begin() }; Iter != m_vecChild.end(); ++Iter)
	{
		if ((*Iter))
			(*Iter)->tick();
	}
}

void CGameObject::finaltick()
{
	for (auto Iter{ m_arrCom.begin() }; Iter != m_arrCom.end(); ++Iter)
	{
		if ((*Iter))
			(*Iter)->finaltick();
	}

	for (auto Iter{ m_vecChild.begin() }; Iter != m_vecChild.end(); ++Iter)
	{
		if ((*Iter))
			(*Iter)->finaltick();
	}

	for (auto iter{ m_vecChild.begin() }; iter != m_vecChild.end();)
	{
		if ((*iter)->IsDead())
		{
			iter = m_vecChild.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	auto pLevel = CLevelMgr::GetInst()->GetCurLevel(); 
	auto pLayer = pLevel->GetLayer(m_iLayerIdx);
	pLayer->RegisterObject(this);
}

void CGameObject::render()
{
	if (nullptr == m_pRenderComponent)
		return;

	m_pRenderComponent->render();
}

void CGameObject::AddComponent(CComponent* _pComponent)
{

	auto eComType = _pComponent->GetType();

	if(eComType != SCRIPT)
		assert(!m_arrCom[(UINT)eComType]);


	_pComponent->m_pOwnerObject = this;

	//[220930] 아래 코드 없을 때, m_arrCom에서 포인터 못찾음
	switch (eComType)
	{
	case SCRIPT:
		m_vecScripts.push_back((CScript*)_pComponent);
		break;
	default:
		auto pRenderCom = dynamic_cast<CRenderComponent*>(_pComponent);

		if (nullptr != pRenderCom)
		{
			assert(!m_pRenderComponent);
			m_pRenderComponent = pRenderCom;
		}

		m_arrCom[(UINT)eComType] = _pComponent;
		break;
	}
}

CComponent* CGameObject::GetComponent(COMPONENT_TYPE _eComType)
{
	return m_arrCom[(UINT)_eComType];
}