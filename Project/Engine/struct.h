#pragma once

struct Vtx 
{
	Vec3 vPos;
	Vec4 vColor;
	Vec2 vUV;
};

// ================
// 상수버퍼용 구조체
// ================
struct tTransform
{
	Vec4 vPos;
	Matrix matWorld;
	Matrix matWV;
	Matrix matWVP;
};

struct tMtrlConst
{
	int iArr[4];
	float fArr[4];
	Vec2 v2Arr[4];
	Vec4 v4Arr[4];
	Matrix matArr[4];
};