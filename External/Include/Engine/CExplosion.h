#pragma once
#include "CParticleSystem.h"
#include "CParticleUpdateShader.h"

class CStructuredBuffer;

/*
* 랜더 컴포넌트(재질, 메쉬)
* 재질->랜더용 셰이더
* 컴퓨트 셰이더
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