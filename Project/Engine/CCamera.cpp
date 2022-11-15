#include "pch.h"
#include "CCamera.h"

#include "CMesh.h"
#include "CRenderMgr.h"
#include "CStructuredBuffer.h"

#include "CLevel.h"
#include "CLevelMgr.h"
#include "CLayer.h"
#include "CDevice.h"
#include "CTransform.h"
#include "CRenderComponent.h"

CCamera::CCamera()
	:CComponent(COMPONENT_TYPE::CAMERA)
	, m_eProjType{ PROJ_TYPE::ORTHOGRAHPICS }
	, m_matView{}
	, m_matProj{}
	, m_fAspectRatio{1.f}
	, m_fFar{100000.f}
	, m_fScale{1.f}
	, m_iLayerMask(0)
{
	Vec2 vRenderResolution	= CDevice::GetInst()->GetRenderResolution();
	m_fAspectRatio			= vRenderResolution.x / vRenderResolution.y;

	m_pObjectRenderBuffer = new CStructuredBuffer;
	m_pObjectRenderBuffer->Create(sizeof(tObjectRender), 2, SB_TYPE::SRV_ONLY, nullptr);
}

CCamera::~CCamera()
{
	Safe_Delete(m_pObjectRenderBuffer);
}

void CCamera::finaltick()
{
	Vec3	vPos = Transform()->GetRelativePos();
	Matrix	matViewTrnas = XMMatrixTranslation(-vPos.x, -vPos.y, -vPos.z);
	Vec3	vRight = Transform()->GetRelativeDir(DIR::RIGHT);
	Vec3	vUp = Transform()->GetRelativeDir(DIR::UP);
	Vec3	vLook = Transform()->GetRelativeDir(DIR::FRONT);

	Matrix matViewRot = XMMatrixIdentity();
	/*
	* 직교 벡터, 방향 벡터의 경우 전치 행렬을 만들면, 원본 행렬과 전치행렬 각 성분의 곱이 내적에 따라, 단위 행렬이 된다.
	*/
	matViewRot._11 = vRight.x;
	matViewRot._21 = vRight.y;
	matViewRot._31 = vRight.z;

	matViewRot._12 = vUp.x;
	matViewRot._22 = vUp.y;
	matViewRot._32 = vUp.z;

	matViewRot._13 = vLook.x;
	matViewRot._23 = vLook.y;
	matViewRot._33 = vLook.z;

	m_matView = matViewTrnas * matViewRot;
	//m_matView =  XMMatrixIdentity();
	Vec2 vRenderResolution = CDevice::GetInst()->GetRenderResolution();

	switch (m_eProjType)
	{
	case PERSPECTIVE:
		m_matProj = XMMatrixPerspectiveFovLH(XM_2PI / 6.f, m_fAspectRatio, 1.f, m_fFar);
		break;
	case ORTHOGRAHPICS:
		m_matProj = XMMatrixOrthographicLH(vRenderResolution.x * m_fScale, vRenderResolution.y * m_fScale, 1.f, m_fFar);
		break;
	}
	/*
	* 
	*/
	CRenderMgr::GetInst()->RegisterCamera(this);
}

void CCamera::render()
{
	g_transform.matView = m_matView;
	g_transform.matProj = m_matProj;

	SortObject();
	render_opaque();
	render_mask();
	render_transparent();
}

/*
* 상수 버퍼 불러와서, 오브젝트 -> 텍스쳐 업데이트
* 맵 -> 벡터 -> 오브젝트1 -> 업데이트
* 맵 -> 벡터 -> 오브젝트2 -> 업데이트
*/

/*
* 랜더가 아닌 벡터안에 정보를 집어 넣는 과정 후 Shader 돌리는 코드가 들어가야한다.
*/
void CCamera::render_opaque()
{
	Ptr<CMesh> p{};

	for (auto elem{ m_mapOpaqueVec.begin() }; elem != m_mapOpaqueVec.end(); ++elem)
	{
		for (auto elem2{ elem->second.begin() }; elem2 != elem->second.end(); ++elem2)
		{
			if (!p.Get())
				p = (*elem2)->GetRenderComponent()->GetMesh();

			(*elem2)->render();
			m_vecInfoObject.push_back(g_objectInfo);
			memset(&g_objectInfo, 0, sizeof(tObjectRender));
		}
		m_pObjectRenderBuffer->SetData(m_vecInfoObject.data(), m_vecInfoObject.size());
		m_pObjectRenderBuffer->UpdateData(16, PIPELINE_STAGE::VS | PIPELINE_STAGE::PS);
		if(p.Get())
			p->render_particle(m_vecInfoObject.size());
		CMaterial::Clear();
		m_pObjectRenderBuffer->Clear();
		m_vecInfoObject.clear();
		p = nullptr;
	}
}

void CCamera::render_mask()
{
	Ptr<CMesh> p{};

	for (auto elem{ m_mapMaskVec.begin() }; elem != m_mapMaskVec.end(); ++elem)
	{
		for (auto elem2{ elem->second.begin() }; elem2 != elem->second.end(); ++elem2)
		{
			if (!p.Get())
				p = (*elem2)->GetRenderComponent()->GetMesh();

			(*elem2)->render();
			m_vecInfoObject.push_back(g_objectInfo);
			memset(&g_objectInfo, 0, sizeof(tObjectRender));
		}
		m_pObjectRenderBuffer->SetData(m_vecInfoObject.data(), m_vecInfoObject.size());
		m_pObjectRenderBuffer->UpdateData(16, PIPELINE_STAGE::VS | PIPELINE_STAGE::PS);
		if (p.Get())
			p->render_particle(m_vecInfoObject.size());
		CMaterial::Clear();
		m_pObjectRenderBuffer->Clear();
		m_vecInfoObject.clear();
		p = nullptr;
	}
}

void CCamera::render_transparent()
{
	Ptr<CMesh> p{};

	for (auto elem{ m_mapTransparentVec.begin() }; elem != m_mapTransparentVec.end(); ++elem)
	{
		for (auto elem2{ elem->second.begin() }; elem2 != elem->second.end(); ++elem2)
		{
			if (!lstrcmp((*elem2)->GetName().c_str(), L"MouseDrag"))
			{
				int a = 0;
			}
			if (!p.Get())
				p = (*elem2)->GetRenderComponent()->GetMesh();

			(*elem2)->render();
			m_vecInfoObject.push_back(g_objectInfo);
			memset(&g_objectInfo, 0, sizeof(tObjectRender));
		}
		m_pObjectRenderBuffer->SetData(m_vecInfoObject.data(), m_vecInfoObject.size());
		m_pObjectRenderBuffer->UpdateData(16, PIPELINE_STAGE::VS | PIPELINE_STAGE::PS);

		if(p.Get())
			p->render_particle(m_vecInfoObject.size());

		CMaterial::Clear();
		m_pObjectRenderBuffer->Clear();
		m_vecInfoObject.clear();
		p = nullptr;
	}
}

void CCamera::SetLayerMask(const wstring& _strLayerName)
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	CLayer* pLayer    = pCurLevel->GetLayer(_strLayerName); 
	assert(pLayer);

	SetLayerMask(pLayer->GetLayerIndex());
}

void CCamera::SetLayerMask(int _iLayerIdx)
{
	if (m_iLayerMask & (1 <<_iLayerIdx))
	{
		m_iLayerMask &= ~(1 << m_iLayerMask);
	}
	else
	{
		m_iLayerMask |= (1 << m_iLayerMask);
	}
}

void CCamera::SortObject()
{
	Clear_VecOfMap(m_mapOpaqueVec);
	Clear_VecOfMap(m_mapMaskVec);
	Clear_VecOfMap(m_mapTransparentVec);

	auto pLevel = CLevelMgr::GetInst()->GetCurLevel();

	for (UINT i{ 0 }; i < (UINT)MAX_LAYER; ++i)
	{
		if (m_iLayerMask & (1 << i))
		{
			CLayer* pLayer = pLevel->GetLayer(i);

			const vector<CGameObject*>& vecGameObject = pLayer->GetObjects();

			for (size_t j{ 0 }; j < (size_t)vecGameObject.size(); ++j)
			{
				CRenderComponent* RenderCompoent = vecGameObject[j]->GetRenderComponent();

				if (RenderCompoent == nullptr ||
					RenderCompoent->GetCurMaterial() == nullptr ||
					RenderCompoent->GetCurMaterial()->GetShader() == nullptr ||
					RenderCompoent->GetMesh() == nullptr)
				{
					continue;
				}

				Ptr<CGraphicsShader> GraphicsShader = RenderCompoent->GetCurMaterial()->GetShader();

				SHADER_DOMAIN eDomain = GraphicsShader->GetDomain();

				switch (eDomain)
				{
				case SHADER_DOMAIN::DOMAIN_OPAQUE:
					m_mapOpaqueVec[vecGameObject[j]->GetName()].push_back(vecGameObject[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_MASK:
					m_mapMaskVec[vecGameObject[j]->GetName()].push_back(vecGameObject[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_TRANSPARENT:
					m_mapTransparentVec[vecGameObject[j]->GetName()].push_back(vecGameObject[j]);
					break;
				}
			}
		}
	}
}

//void CCamera::update_render(Ptr<CMesh> p)
//{
//	m_pObjectRenderBuffer->SetData(m_vecInfoObject.data(), m_vecInfoObject.size());
//	m_pObjectRenderBuffer->UpdateData(16, PIPELINE_STAGE::VS | PIPELINE_STAGE::PS);
//	p->render_particle(m_vecInfoObject.size());
//	m_pObjectRenderBuffer->Clear();
//	p = nullptr;
//}
