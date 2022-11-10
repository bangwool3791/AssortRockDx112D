#pragma once
#include "CRenderComponent.h"
#include "CMesh.h"
#include "CMaterial.h"

class CMeshRender
	:public CRenderComponent
//	,public SmallObjAllocator<CMeshRender, 1000>
{
public:
	virtual void tick() override;
	virtual void finaltick() override;
	virtual void render();

	CLONE(CMeshRender);
public :
	CMeshRender();
	CMeshRender(const CMeshRender& rhs);
	virtual ~CMeshRender();
};
