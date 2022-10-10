#include "pch.h"
#include "CRenderComponent.h"

CRenderComponent::CRenderComponent(COMPONENT_TYPE _eComponentType)
	:CComponent(_eComponentType)
{

}

CRenderComponent::~CRenderComponent()
{
}

Ptr<CMaterial> CRenderComponent::GetSharedMaterial()
{
	m_pCurMtrl = m_pSharedMtrl;

	if (nullptr != m_pDynamicMtrl)
	{
		m_pDynamicMtrl = nullptr;
	}

	return m_pSharedMtrl;
}

Ptr<CMaterial> CRenderComponent::GetDynamicMaterial()
{
	if (nullptr != m_pDynamicMtrl)
	{
		return m_pDynamicMtrl;
	}

	m_pDynamicMtrl = new CMaterial(*m_pSharedMtrl.Get());
	m_pDynamicMtrl->SetName(m_pSharedMtrl->GetName() + L"Clone");
	m_pCurMtrl = m_pDynamicMtrl;

	return m_pCurMtrl;
}
