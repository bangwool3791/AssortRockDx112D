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

	//여기서 왜 트랜스폼?
	if (nullptr == GetCurMaterial() || nullptr == GetMesh())
		return;
	/*
	* 텍스처 버퍼 업데이트 처리
	* 
	* CRenderMgr::UpdateLight2D() 참고
	* 싱글 톤 또는 static vector 처리
	*/
	//삭제 예정
	Transform()->UpdateData();

	//삭제 예정
	GetCurMaterial()->UpdateData();

	if (Animator2D())
	{
		//삭제 예정
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

