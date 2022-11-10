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
#endif