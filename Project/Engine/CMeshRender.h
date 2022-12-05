#pragma once
#include "CRenderComponent.h"
#include "CMesh.h"
#include "CMaterial.h"

class CMeshRender
	:public CRenderComponent
{
public:
	virtual void tick() override;
	virtual void finaltick() override;
	virtual void render();
	virtual void render_Instancing();
	CLONE(CMeshRender);
public :
	CMeshRender();
	CMeshRender(INSTANCING_TYPE _eInsType);
	CMeshRender(const CMeshRender& rhs);
	virtual ~CMeshRender();
};
