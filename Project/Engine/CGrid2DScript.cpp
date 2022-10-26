#include "pch.h"
#include "CGrid2DScript.h"

#include "CRenderMgr.h"

#include "CCamera.h"
#include "CDevice.h"
#include "CTransform.h"
#include "CMaterial.h"
#include "CMeshRender.h"

CGrid2DScript::CGrid2DScript()
	:m_pMainCam{}
	, m_vColor{}
	, m_fThickness{}
	, m_fGridInterval{}
{

}

CGrid2DScript::~CGrid2DScript()
{

}


void CGrid2DScript::begin()
{
}
void CGrid2DScript::tick()
{
	m_pMainCam = CRenderMgr::GetInst()->GetMainCam();

	Vec3	vPos	= m_pMainCam->Transform()->GetRelativePos();
	float	fScale  = m_pMainCam->GetOrthographicScale();
	Vec2	vRenderResolution = CDevice::GetInst()->GetRenderResolution();

	MeshRender()->GetCurMaterial()->SetScalarParam(FLOAT_0, &fScale);
	MeshRender()->GetCurMaterial()->SetScalarParam(VEC4_0, &vPos);
	MeshRender()->GetCurMaterial()->SetScalarParam(VEC2_0, &vRenderResolution);
	
	MeshRender()->GetCurMaterial()->SetScalarParam(FLOAT_1, &m_fThickness);
	MeshRender()->GetCurMaterial()->SetScalarParam(FLOAT_2, &m_fGridInterval);
	MeshRender()->GetCurMaterial()->SetScalarParam(VEC4_1,  &m_vColor);
}
