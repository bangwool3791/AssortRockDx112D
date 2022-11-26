#pragma once
#include "CParticleSystem.h"
#include "CParticleUpdateShader.h"

class CStructuredBuffer;

/*
* ���� ������Ʈ(����, �޽�)
* ����->������ ���̴�
* ��ǻƮ ���̴�
*/
class CParticleWood :
	public CParticleSystem
{
public:
	virtual void finaltick()override;
	virtual void render()override;

	CLONE(CParticleWood);

public:
	CParticleWood();
	CParticleWood(const CParticleWood& _Rhs);
	virtual ~CParticleWood();
};