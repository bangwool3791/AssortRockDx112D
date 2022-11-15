#include "pch.h"
#include "CParticleSystem.h"
#include "CStructuredBuffer.h"

#include "CResMgr.h"
#include "CTimeMgr.h"

#include "CTransform.h"

CParticleSystem::CParticleSystem()
	:CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_iMaxCount(144)
	, m_fMinLifeTime(0.f)
	, m_fMaxLifeTime(0.f)
	, m_ParticleBuffer{}
	, m_ParticleShare{}
	, m_arrParticle{}
	, m_fAccTime{}
{
	SetName(L"CParticleSystem");
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));
	SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleRenderMtrl"));
	m_UpdateCS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"ParticleUpdateShader").Get();

	GetCurMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"SmokeParticle"));
	
	Vec4 vStartPos = Vec4(-800, -450, 0.f, 0.f);
	for (int i = 0; i < m_iMaxCount; ++i)
	{
		m_arrParticle[i].vRelativePos = Vec3(0.f, 0.f, 100.f);
		m_arrParticle[i].vDir = Vec3(cosf((float)i * (XM_2PI / (float)m_iMaxCount)), sinf((float)i * (XM_2PI / m_iMaxCount)), 0.f);
		m_arrParticle[i].iActive = 0;
		m_arrParticle[i].fSpeed = 200.f;
	}

	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_iMaxCount, SB_TYPE::UAV_INC, m_arrParticle);
	
	m_ParticleShare = new CStructuredBuffer;
	m_ParticleShare->Create(sizeof(tParticleShare), 1, SB_TYPE::UAV_INC, nullptr, true);

}

CParticleSystem::CParticleSystem(const CParticleSystem& _Rhs)
	:CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_iMaxCount(_Rhs.m_iMaxCount)
	, m_fMinLifeTime(_Rhs.m_iMaxCount)
	, m_fMaxLifeTime(_Rhs.m_iMaxCount)
	, m_fAccTime{_Rhs.m_fAccTime}
	, m_ParticleBuffer{}
	, m_ParticleShare{}
{
	memcpy(&m_arrParticle, _Rhs.m_arrParticle, sizeof(_Rhs.m_arrParticle) / sizeof(tParticle));

	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_iMaxCount, SB_TYPE::UAV_INC, const_cast<tParticle*>(m_arrParticle));

	m_ParticleShare = new CStructuredBuffer;
	m_ParticleShare->Create(sizeof(tParticleShare), 1, SB_TYPE::UAV_INC, &m_share, true);
}

CParticleSystem::~CParticleSystem()
{
	Safe_Delete(m_ParticleBuffer);
	Safe_Delete(m_ParticleShare);
}

void CParticleSystem::finaltick()
{
	float fAliveTime = 1.f / m_fFrequency;

	m_fAccTime += DT;

	if (fAliveTime <= m_fAccTime)
	{
		float f = m_fAccTime / fAliveTime;
		UINT iAliveCount = (UINT)f;
		m_fAccTime = f - floor(f);
		m_share.iAliveCount = 10;
		m_ParticleShare->SetData(&m_share, 1);
	}
	else
	{
		m_share.iAliveCount = 0;
		m_ParticleShare->SetData(&m_share, 1);
	}
	
	m_UpdateCS->SetParticleBuffer(m_ParticleBuffer);
	m_UpdateCS->SetParticleShareData(m_ParticleShare);
	m_UpdateCS->Excute();
}

void CParticleSystem::render()
{
	Transform()->UpdateData();
	m_ParticleBuffer->UpdateData(17, PIPELINE_STAGE::VS | PIPELINE_STAGE::GS | PIPELINE_STAGE::PS);

	GetCurMaterial()->UpdateData();
	GetMesh()->render_particle(m_iMaxCount);

	m_ParticleBuffer->Clear();
}

