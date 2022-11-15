#pragma once
#include "CComputeShader.h"

class CStructuredBuffer;

class CParticleUpdateShader :
	public CComputeShader
{
public :
	CStructuredBuffer* m_ParticleBuffer;
	/*
	* 셰이더 공유 데이터
	* alive count
	*/
	CStructuredBuffer* m_ParticleShare;
public:
	void SetParticleBuffer(CStructuredBuffer* _Buffer);
	void SetParticleShareData(CStructuredBuffer* _Buffer) { assert(_Buffer); m_ParticleShare = _Buffer; }

public :
	virtual void UpdateData()override;
	virtual void Clear() override;

public:
	CParticleUpdateShader();
	~CParticleUpdateShader();
};