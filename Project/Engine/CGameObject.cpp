#include "pch.h"

#include "CMesh.h"
#include "CGraphicsShader.h"

#include "CLevel.h"
#include "CLayer.h"
#include "CLevelMgr.h"

#include "CGameObject.h"
#include "CEventMgr.h"

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
	, m_pParent(nullptr)
	, m_arrCom{}
	, m_pRenderComponent(nullptr)
	, m_iLayerIdx(-1)
{
	for (size_t i{ 0 }; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (rhs.m_arrCom[i] == nullptr)
			continue;

		if (i == MESHRENDER)
		{
			int a = 0;
		}
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

	begin();
}

CGameObject::~CGameObject()
{
	if (!lstrcmp(L"Player", GetName().c_str()))
	{
		int a = 0;
	}
	for (int i{ 0 }; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		delete m_arrCom[i];
		m_arrCom[i] = nullptr;
	}
	
	for (auto iter{ m_vecScripts.begin() }; iter != m_vecScripts.end(); ++iter)
	{
		delete* iter;
		*iter = nullptr;
	}

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
	if (!lstrcmp(GetName().c_str(), L"SpotLight"))
	{
		int a{};

	}

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
	if (!lstrcmp(GetName().c_str(), L"SpotLight"))
	{
		int a{};

	}

	for (auto Iter{ m_arrCom.begin() }; Iter != m_arrCom.end(); ++Iter)
	{
		if ((*Iter))
			(*Iter)->finaltick();
	}

	for (auto Iter{ m_vecScripts.begin() }; Iter != m_vecScripts.end(); ++Iter)
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
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurLevel(); 
	CLayer* pLayer = pLevel->GetLayer(m_iLayerIdx);
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

void CGameObject::DestroyComponent(COMPONENT_TYPE _eComType)
{
	if (m_arrCom[(UINT)_eComType])
	{
		delete m_arrCom[(UINT)_eComType];
		m_arrCom[(UINT)_eComType] = nullptr;
	}

}

CComponent* CGameObject::GetComponent(COMPONENT_TYPE _eComType)
{
	return m_arrCom[(UINT)_eComType];
}

void CGameObject::Destroy()
{
	tEvent eve{};
	eve.eType = EVENT_TYPE::DELETE_OBJECT;
	eve.wParam = (DWORD_PTR)this;

	CEventMgr::GetInst()->AddEvent(eve);
}

CGameObject* CGameObject::GetChild(const wstring& _key)
{
	for (auto iter{ m_vecChild.begin() }; iter != m_vecChild.end(); ++iter)
	{
		if (!lstrcmp(_key.c_str(), (*iter)->GetName().c_str()))
		{
			return (*iter);
		}
	}
	return nullptr;
}