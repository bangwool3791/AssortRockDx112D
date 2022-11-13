#include "pch.h"
#include "CMeshRender.h"
#include "CTransform.h"
#include "CAnimator2D.h"

CMeshRender::CMeshRender()
	:CRenderComponent{ COMPONENT_TYPE::MESHRENDER }
{	

}

CMeshRender::CMeshRender(const CMeshRender& rhs)
	:CRenderComponent{ rhs }
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

/*
* 	Transform()->UpdateData();
	m_ParticleBuffer->UpdateData(15, PIPELINE_STAGE::VS | PIPELINE_STAGE::GS | PIPELINE_STAGE::PS);

	GetCurMaterial()->UpdateData();
	GetMesh()->render_particle(m_iMaxCount);

	m_ParticleBuffer->Clear();
	
	render를 모은다.
	Transform을 버퍼에 업데이트 시킨다.
	카운트를 증가시킨다.
	랜더한다.
*/
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
	Transform()->UpdateData();
	
	GetCurMaterial()->UpdateData();

	if (Animator2D())
	{
		Animator2D()->UpdateData();
	}

	/*
	* 함수 밖으로 뺀다.
	*/
	GetMesh()->render();

	CMaterial::Clear();

	if (Animator2D())
		Animator2D()->Clear();
}

