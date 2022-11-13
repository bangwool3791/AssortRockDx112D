#pragma once
#include "CRenderComponent.h"
#include "CParticleUpdateShader.h"

class CStructuredBuffer;

/*
* 랜더 컴포넌트(재질, 메쉬)
* 재질->랜더용 셰이더
* 컴퓨트 셰이더
*/
class CParticleSystem :
	public CRenderComponent
{
private:
	UINT	m_iMaxCount;

	Vec2	m_vStartScale;
	Vec2	m_vEndScale;

	Vec4	m_vStartColor;
	Vec4	m_vEndColor;

	float	m_fMinLifeTime;
	float	m_fMaxLifeTime;

	float   m_fFrequency;

	CStructuredBuffer* m_ParticleBuffer;
	Ptr<CParticleUpdateShader>  m_UpdateCS;

public :
	virtual void finaltick()override;
	virtual void render()override;

	CLONE(CParticleSystem);
	
public :
	CParticleSystem();
	~CParticleSystem();
};