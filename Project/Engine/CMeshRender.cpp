#include "pch.h"
#include "CMeshRender.h"
#include "CTransform.h"

CMeshRender::CMeshRender()
	:CRenderComponent{ COMPONENT_TYPE::MESHRENDER }
{	

}

CMeshRender::~CMeshRender()
{

}

void CMeshRender::tick()
{

}

void CMeshRender::finaltick()
{

}

void CMeshRender::render()
{
	//여기서 왜 트랜스폼?
	if (nullptr == GetCurMaterial() || nullptr == GetMesh())
		return;

	Transform()->UpdateData();

	GetCurMaterial()->UpdateData();

	GetMesh()->render();
}

