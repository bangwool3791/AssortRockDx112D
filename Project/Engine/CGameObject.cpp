#include "pch.h"

#include "CMesh.h"
#include "CGraphicsShader.h"

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
{

}

CGameObject::~CGameObject()
{
	Safe_Del_Array(m_arrCom.begin(), m_arrCom.end());
	Safe_Del_Array(m_vecScripts.begin(), m_vecScripts.end());
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
}

void CGameObject::finaltick()
{
	for (auto Iter{ m_arrCom.begin() }; Iter != m_arrCom.end(); ++Iter)
	{
		if ((*Iter))
			(*Iter)->finaltick();
	}
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

	//[220930] 아래 코드 없을 때, m_arrCom에서 포인터 못찾음
	switch (eComType)
	{
	case SCRIPT:
		_pComponent->m_pOwnerObject = this;
		m_vecScripts.push_back((CScript*)_pComponent);
		break;
	default:
		{
			assert(!m_arrCom[(UINT)eComType]);

			CRenderComponent* pRenderCom = dynamic_cast<CRenderComponent*>(_pComponent);
			if (nullptr != pRenderCom)
			{
				assert(!m_pRenderComponent); // render 기능 컴포넌트는 한개만 가질 수 있다.
				m_pRenderComponent = pRenderCom;
			}
			_pComponent->m_pOwnerObject = this;
			m_arrCom[(UINT)eComType] = _pComponent;
			break;
		}
	}
}

CComponent* CGameObject::GetComponent(COMPONENT_TYPE _eComType)
{
	return m_arrCom[(UINT)_eComType];
}