#pragma once
#include "CParticleSystem.h"
#include "CParticleUpdateShader.h"

class CStructuredBuffer;

/*
* ���� ������Ʈ(����, �޽�)
* ����->������ ���̴�
* ��ǻƮ ���̴�
*/
class CParticleExplosion :
	public CParticleSystem
{
public:
	virtual void finaltick()override;
	virtual void render()override;

	CLONE(CParticleExplosion);

public:
	CParticleExplosion();
	CParticleExplosion(const CParticleExplosion& _Rhs);
	virtual ~CParticleExplosion();
};