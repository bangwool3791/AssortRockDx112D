#include "pch.h"
#include "CMeshRender.h"

#include "CTransform.h"

#include "CMesh.h"
#include "CMaterial.h"

CMeshRender::CMeshRender()
	:CComponent{ COMPONENT_TYPE::MESHRENDER }
	, m_pMesh{}
	, m_pMtrl{}
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
	if (!m_pMtrl || !m_pMesh)
		return;

	Transform()->UpdateData();

	m_pMtrl->UpdateData();

	m_pMesh->render();
}

