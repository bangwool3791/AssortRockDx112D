#ifndef _STRUCT
#define _STRUCT

//LightInfo

struct tLightColor
{
	float4 vDiff;
	float4 vSpec;
	float4 vEmb;
};

struct tLightInfo
{
	tLightColor color;
	float4		vWorldPos; //광원 월드 위치
	float4		vWorldDir; //광원 월드 방향
	int			iLightType;//광원 타입
	float		fRadius; //광원 반경 (POINT, SPOT)
	float		fAngle;
	int			pad;
};

/*
* Particle System Texture 버퍼 사용 구조체
*/
struct tParticle
{
	float4 vRelativePos;
	float4 vDir;

	float fMaxTime;
	float fCurTime;
	float fSpeed;
	uint  iActive;
};
#endif