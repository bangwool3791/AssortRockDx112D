#include "pch.h"
#include "CParticleSystem.h"
#include "CStructuredBuffer.h"

#include "CResMgr.h"
#include "CTimeMgr.h"

#include "CTransform.h"

CParticleSystem::CParticleSystem()
	:CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_iMaxCount(10)
	, m_ParticleBuffer{}
	, m_ParticleShare{}
	, m_arrParticle{}
	, m_fAccTime{}

{
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));

	m_ParticleBuffer = new CStructuredBuffer;

	tParticle arr[10] = {};
	for (UINT i{ 0 }; i < 10; ++i)
	{
		arr[i].fSpeed = 200.f;
		arr[i].vDir = Vec2(cosf(XM_PI * i / 5), sinf(XM_PI * i / 5));
	}

	m_ParticleBuffer->Create(sizeof(tParticle), m_iMaxCount, SB_TYPE::UAV_INC, &arr);

	m_ParticleShare = new CStructuredBuffer;
	m_ParticleShare->Create(sizeof(tParticleShare), 1, SB_TYPE::UAV_INC, nullptr, true);

}

CParticleSystem::CParticleSystem(const CParticleSystem& _Rhs)
	:CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_iMaxCount(_Rhs.m_iMaxCount)
	, m_fMinLifeTime(_Rhs.m_fMinLifeTime)
	, m_fMaxLifeTime(_Rhs.m_fMaxLifeTime)
	, m_fAccTime{_Rhs.m_fAccTime}
	, m_ParticleBuffer{}
	, m_ParticleShare{}
{
	memcpy(&m_arrParticle, _Rhs.m_arrParticle, sizeof(_Rhs.m_arrParticle) / sizeof(tParticle));

	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));
	SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleRenderMtrl"));

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

	if (fAliveTime < m_fAccTime)
	{
		float f = m_fAccTime / fAliveTime;
		UINT iAliveCount = (UINT)f;
		m_fAccTime = f - floor(f);
		m_share.iAliveCount = 1;
		m_ParticleShare->SetData(&m_share, 1);
	}
	else
	{
		m_share.iAliveCount = 0;
		m_ParticleShare->SetData(&m_share, 1);
	}


	m_UpdateCS->SetWorldSpawn(m_WorldSpawn);
	m_UpdateCS->SetObjectWorldPos(Transform()->GetWorldPos());
	m_UpdateCS->SetSpawnRange(m_fSpawnRange);
	m_UpdateCS->SetMinMaxSpeed(m_vMinMaxSpeed);
	m_UpdateCS->SetMinMaxLifeTime(m_vMinMaxLifeTime);
	m_UpdateCS->SetParticleType(m_iType);
	m_UpdateCS->Excute();
}

void CParticleSystem::render()
{
}

