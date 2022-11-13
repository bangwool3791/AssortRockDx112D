#pragma once
#include "CComponent.h"

#include "CMesh.h"
#include "CMaterial.h"

/*
* 매쉬
* 재질
*/
class CRenderComponent
	:public CComponent
{
public:
	CRenderComponent(COMPONENT_TYPE _eComponentType);
	CRenderComponent(const CRenderComponent& rhs);
	virtual ~CRenderComponent()
	{
		m_pDynamicMtrl = nullptr;
	}
private:
	/*
	* static 변수로 변경 예정
	*/
	Ptr<CMesh>				m_pMesh;
	//
	Ptr<CMaterial>			m_pSharedMtrl;
	Ptr<CMaterial>			m_pDynamicMtrl;
	Ptr<CMaterial>			m_pCurMtrl;

public:
	void SetMesh(Ptr<CMesh> _pMesh) { m_pMesh = _pMesh; }
	Ptr<CMesh> GetMesh() { return m_pMesh; }

	void SetSharedMaterial(Ptr<CMaterial> _pMtrl) 
	{ 
		m_pSharedMtrl = _pMtrl; 
		m_pCurMtrl = _pMtrl; 
	}
	Ptr<CMaterial> GetSharedMaterial();

	Ptr<CMaterial> GetCurMaterial() 
	{
 		return m_pCurMtrl; 
	}
	Ptr<CMaterial> GetDynamicMaterial();
public:
	virtual CRenderComponent* Clone() = 0;
public:
	virtual void render() = 0;

};
