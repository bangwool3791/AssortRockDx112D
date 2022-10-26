#include "pch.h"
#include "CComponent.h"

CComponent::CComponent(COMPONENT_TYPE _eType)
	:m_eType{_eType}
	, m_pOwnerObject{ nullptr }
{

}

CComponent::CComponent(const CComponent& rhs)
	:CEntity(rhs)
	,m_eType{rhs.m_eType}
	,m_pOwnerObject{nullptr}
{

}

CComponent::~CComponent()
{

}