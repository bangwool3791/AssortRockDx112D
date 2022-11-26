#include "pch.h"
#include "CMeshRender.h"
#include "CRenderMgr.h"
#include "CTransform.h"
#include "CAnimator2D.h"

CMeshRender::CMeshRender()
	:CRenderComponent{ COMPONENT_TYPE::MESHRENDER }
	, m_eInsType{INSTANCING_TYPE::USED}
{	

}

CMeshRender::CMeshRender(INSTANCING_TYPE _eInsType)
	:CRenderComponent{ COMPONENT_TYPE::MESHRENDER }
	, m_eInsType{ _eInsType }
{

}


CMeshRender::CMeshRender(const CMeshRender& rhs)
	:CRenderComponent{ rhs }
	,m_eInsType{rhs.m_eInsType}
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
	if (!IsActive())
		return;

	//���⼭ �� Ʈ������?
	if (nullptr == GetCurMaterial() || nullptr == GetMesh())
		return;
	/*
	* �ؽ�ó ���� ������Ʈ ó��
	* 
	* CRenderMgr::UpdateLight2D() ����
	* �̱� �� �Ǵ� static vector ó��
	*/
	//���� ����
	Transform()->UpdateData();

	//���� ����
	GetCurMaterial()->UpdateData();

	if (Animator2D())
	{
		//���� ����
		Animator2D()->UpdateData();
	}

	if (m_eInsType == INSTANCING_TYPE::USED)
	{
		memset(&g_objectInfo, 0, sizeof(tObjectRender));
		g_objectInfo.transform = g_transform;
		g_objectInfo.mtrl = GetCurMaterial()->GetMaterial();

		if(Animator2D())
			g_objectInfo.animation = Animator2D()->GetAniInfo();
	}
	else
	{
		GetMesh()->render();

		CMaterial::Clear();

		if (Animator2D())
			Animator2D()->Clear();
	}
}

