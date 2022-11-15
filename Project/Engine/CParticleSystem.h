#pragma once
#include "CRenderComponent.h"
#include "CParticleUpdateShader.h"

class CStructuredBuffer;

/*
* ���� ������Ʈ(����, �޽�)
* ����->������ ���̴�
* ��ǻƮ ���̴�
*/
class CParticleSystem :
	public CRenderComponent
{
private:
	UINT			m_iMaxCount;

	Vec2			m_vStartScale;
	Vec2			m_vEndScale;

	Vec4			m_vStartColor;
	Vec4			m_vEndColor;

	float			m_fMinLifeTime;
	float			m_fMaxLifeTime;

	float			m_fAccTime;
	float			m_fFrequency;
	tParticle		m_arrParticle[1000];
	tParticleShare  m_share;
	/*
	* ���� �� ��ü�� effect�� �ٸ� �� �����Ƿ� 
	* ���� �����ڿ��� ���� ���� �Ҵ��ؾ��Ѵ�.
	*/
	CStructuredBuffer* m_ParticleBuffer;
	CStructuredBuffer* m_ParticleShare;
	Ptr<CParticleUpdateShader>  m_UpdateCS;

public :
	virtual void finaltick()override;
	virtual void render()override;

	CLONE(CParticleSystem);
	
public :
	CParticleSystem();
	CParticleSystem(const CParticleSystem& _Rhs);
	~CParticleSystem();
};