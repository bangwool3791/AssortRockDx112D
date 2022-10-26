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