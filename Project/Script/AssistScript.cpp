#include "pch.h"
#include "CAssistScript.h"

#include <Engine/CRenderMgr.h>

#include <Engine/CCamera.h>
#include <Engine/CDevice.h>
#include <Engine/CTransform.h>
#include <Engine/CMaterial.h>
#include <Engine/CMeshRender.h>
#include <Engine\CLevel.h>
#include <Engine\CLevelMgr.h>

CAssistScript::CAssistScript()
	: CScript(ASSISTSCRIPT)
	, m_pMainCam{}
	, m_vColor{}
	, m_fThickness{}
	, m_fGridInterval{}
{

}

CAssistScript::~CAssistScript()
{

}


void CAssistScript::begin()
{
	m_pMainCam = CLevelMgr::GetInst()->GetCurLevel()->FindParentObjectByName(L"MainCamera")->Camera();
}

void CAssistScript::tick()
{
	if (!m_pMainCam)
		return;

	if (PROJ_TYPE::PERSPECTIVE == m_pMainCam->GetProjType())
	{
		MeshRender()->Deactivate();
	}
	else
	{
		if (KEY_TAP(KEY::T))
		{
			if (MeshRender()->IsActive())
			{
				MeshRender()->Deactivate();
			}
			else
			{
				MeshRender()->Activate();
			}
		}

		Vec3	vPos = m_pMainCam->Transform()->GetRelativePos();
		float	fScale = m_pMainCam->GetOrthographicScale();
		Vec2	v2 = CDevice::GetInst()->GetRenderResolution();
		Vec3    vRenderResolution = Vec3(v2.x, v2.y, 100000.f - 1.f);

		MeshRender()->GetCurMaterial()->SetScalarParam(FLOAT_0, &fScale);
		MeshRender()->GetCurMaterial()->SetScalarParam(VEC4_0, &vPos);
		MeshRender()->GetCurMaterial()->SetScalarParam(VEC4_1, &vRenderResolution);

		MeshRender()->GetCurMaterial()->SetScalarParam(FLOAT_1, &m_fThickness);
		MeshRender()->GetCurMaterial()->SetScalarParam(FLOAT_2, &m_fGridInterval);
		MeshRender()->GetCurMaterial()->SetScalarParam(VEC4_1, &m_vColor);
	}
}
