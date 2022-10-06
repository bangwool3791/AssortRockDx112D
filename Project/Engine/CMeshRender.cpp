#include "pch.h"
#include "CMeshRender.h"
#include "CTransform.h"

CMeshRender::CMeshRender()
	:CComponent{ COMPONENT_TYPE::MESHRENDER }
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
	//���⼭ �� Ʈ������?
	if (nullptr == m_pMtrl || nullptr == m_pMesh)
		return;

	Transform()->UpdateData();

	m_pMtrl->UpdateData();

	m_pMesh->render();
}

