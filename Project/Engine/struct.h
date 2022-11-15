#pragma once

struct Vtx 
{
	Vec3 vPos;
	Vec4 vColor;
	Vec2 vUV;
};

struct tEvent
{
	EVENT_TYPE eType;
	DWORD_PTR  wParam;
	DWORD_PTR  lParam;
	DWORD_PTR  oParam;
};

struct tAnim2DFrm
{
	Vec2 vLeftTop;
	Vec2 vSlice;
	Vec2 vOffset;
	Vec2 vFullSize;
	float fDuration;
};

struct tAnim2DInfo
{
	Vec2 vLeftTop;
	Vec2 vSlice;
	Vec2 vOffset;
	Vec2 vFullSize;

	int iAnim2DUse;
	/*
	* .fx 파일에서 16바이트 단위로 데이터 전달 필요
	*/
	int iPaading[3];
};
struct tDebugShapeInfo
{
	DEBUG_SHAPE eShape;
	Vec4		vColor;
	Vec3		vPosition;
	Vec3		vScale;
	Vec3		vRot;
	float		fRadius;
	float		fDuration;
	float		fCurTime;
};

// ================
// 상수버퍼용 구조체
// ================
struct tTransform
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProj;

	Matrix matWV;
	Matrix matWVP;
};

extern tTransform g_transform;

struct tMtrlConst
{
	int iArr[4];
	float fArr[4];
	Vec2 v2Arr[4];
	Vec4 v4Arr[4];
	Matrix matArr[4];
};

struct tObjectRender
{
	tTransform transform;
	tMtrlConst mtrl;
	tAnim2DInfo animation;
};

extern tObjectRender g_objectInfo;

struct tLightInfo
{
	Vec4		vDiff;
	Vec4		vSpec;
	Vec4		vEmv;

	Vec4		vWorldPos;
	Vec4		vWorldDir;
	LIGHT_TYPE  iLightType;
	float		fRadius;
	float		fAngle;
	int			pad;
};

struct tGlobalData
{
	Vec2	vRenderResolution;
	int		iLight2DCount;
	int		iLight3DCount;

	float	fAccTime;
	float	fDT;
	int		ipad[2];
};

struct tParticle
{
	Vec4 vRelativePos;
	Vec4 vDir;
	
	float fMaxTime;
	float fCurTime;
	float fSpeed;
	UINT iActive;
};

struct tParticleShare
{
	UINT iAliveCount;
	UINT iPadding[3];
};
extern tGlobalData g_global;