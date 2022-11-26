#pragma once
#include "CParticleSystem.h"
#include "CParticleUpdateShader.h"

class CStructuredBuffer;

/*
* ���� ������Ʈ(����, �޽�)
* ����->������ ���̴�
* ��ǻƮ ���̴�
*/
class CExplosion :
	public CParticleSystem
{
public:
	virtual void finaltick()override;
	virtual void render()override;

	CLONE(CExplosion);

public:
	CExplosion();
	CExplosion(const CExplosion& _Rhs);
	virtual ~CExplosion();
};