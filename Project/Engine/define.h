#pragma once

typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;


#define SINGLE(type) public:\
						static type* GetInst()\
						{\
							static type mgr;\
							return &mgr;\
						}\
						private:\
							type();\
							~type();

#define DT CTimeMgr::GetInst()->GetDeltaTime()

#define DEVICE CDevice::GetInst()->GetDevice()
#define CONTEXT CDevice::GetInst()->GetContext()

#define KEY_PRESSED(key)	CKeyMgr::GetInst()->GetKeyState(key) == KEY_STATE::PRESS
#define KEY_TAP(key)		CKeyMgr::GetInst()->GetKeyState(key) == KEY_STATE::TAP
#define KEY_RELEASE(key)	CKeyMgr::GetInst()->GetKeyState(key) == KEY_STATE::RELEASE

#define MAX_LAYER 32

enum class SAMPLER_TYPE
{
	ANISOTROPIC, // �̹漺
	POINT,

	END,
};

enum class CB_TYPE
{
	TRANSFORM,	// b0
	MATERIAL,	// b1

	END,
};


enum PIPELINE_STAGE
{
	VS = 0x01,
	HS = 0x02,
	DS = 0x04,
	GS = 0x08,
	PS = 0x10
};

enum class RES_TYPE
{
	MESH,
	TEXTURE,
	MATERIAL,
	SOUND,
	PREFAB,
	MESHDATA,
	GRAPHICS_SHADER,
	COMPUTE_SHADER,
	END,
};

enum COMPONENT_TYPE
{
	TRANSFORM,
	MESHRENDER,
	END,
	SCRIPT,
};


enum SCALAR_PARAM
{
	INT_0,
	INT_1,
	INT_2,
	INT_3,

	FLOAT_0,
	FLOAT_1,
	FLOAT_2,
	FLOAT_3,

	VEC2_0,
	VEC2_1,
	VEC2_2,
	VEC2_3,

	VEC4_0,
	VEC4_1,
	VEC4_2,
	VEC4_3,

	MAT_0,
	MAT_1,
	MAT_2,
	MAT_3,
};