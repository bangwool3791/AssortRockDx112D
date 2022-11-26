#pragma once
#include "CRenderComponent.h"
#include "CMesh.h"
#include "CMaterial.h"

class CMeshRender
	:public CRenderComponent
{
private:
	INSTANCING_TYPE m_eInsType;
public :
	void SetInstancingType(INSTANCING_TYPE _eType) { m_eInsType = _eType; }
public:
	virtual void tick() override;
	virtual void finaltick() override;
	virtual void render();

	CLONE(CMeshRender);
public :
	CMeshRender();
	CMeshRender(INSTANCING_TYPE _eInsType);
	CMeshRender(const CMeshRender& rhs);
	virtual ~CMeshRender();
};
