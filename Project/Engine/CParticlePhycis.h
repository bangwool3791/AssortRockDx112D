#pragma once
#include "CParticleSystem.h"
#include "CParticleUpdateShader.h"

class CStructuredBuffer;

/*
* ���� ������Ʈ(����, �޽�)
* ����->������ ���̴�
* ��ǻƮ ���̴�
*/
class CParticlePhycis :
	public CParticleSystem
{
public:
	virtual void finaltick()override;
	virtual void render()override;

	CLONE(CParticlePhycis);

public:
	CParticlePhycis();
	CParticlePhycis(const CParticlePhycis& _Rhs);
	virtual ~CParticlePhycis();
};