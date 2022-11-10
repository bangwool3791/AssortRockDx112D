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
	float4		vWorldPos; //���� ���� ��ġ
	float4		vWorldDir; //���� ���� ����
	int			iLightType;//���� Ÿ��
	float		fRadius; //���� �ݰ� (POINT, SPOT)
	float		fAngle;
	int			pad;
};
#endif