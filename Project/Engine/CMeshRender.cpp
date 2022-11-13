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
	
	render�� ������.
	Transform�� ���ۿ� ������Ʈ ��Ų��.
	ī��Ʈ�� ������Ų��.
	�����Ѵ�.
*/
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
	memset(&g_objectInfo, 0, sizeof(tObjectRender));
	g_objectInfo.transform = g_transform;

	//���� ����
	GetCurMaterial()->UpdateData();

	g_objectInfo.mtrl = GetCurMaterial()->GetMaterial();

	if (Animator2D())
	{
		//���� ����
		Animator2D()->UpdateData();
		g_objectInfo.animation = Animator2D()->GetAniInfo();
	}

	/*
	* �Լ� ������ ����.
	*/
	//GetMesh()->render_particle(1);

	//CMaterial::Clear();

	//if (Animator2D())
	//	Animator2D()->Clear();
}

