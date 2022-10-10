#pragma once
#include "CComponent.h"

#include "CRenderComponent.h"

class CMeshRender
	:public CRenderComponent
{
public:
	virtual void tick() override;
	virtual void finaltick() override;
	void render();

public :
	CMeshRender();
	virtual ~CMeshRender();
};
