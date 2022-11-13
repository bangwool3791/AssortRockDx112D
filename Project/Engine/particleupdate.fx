#ifndef _PARTICLEUPDATE
#define _PARTICLEUPDATE

#include "register.fx"

RWStructuredBuffer<tParticle> ParticleBuffer : register(u0);
#define PARTICLE_MAX g_int_0

[numthreads(128, 1, 1)]
void CS_ParticleUpdate(uint _id : SV_DispatchThreadID)
{
	if (PARTICLE_MAX <= _id.x)
		return;

	ParticleBuffer[_id.x].vRelativePos += ParticleBuffer[_id.x].vDir * ParticleBuffer[_id.x].fSpeed * g_fDT;
}

#endif