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
#define KEY_DIR	            CKeyMgr::GetInst()->GetMouseDir(key)

#define CLONE(type) public: type* Clone() { return new type(*this); }
#define CLONE_DUMMY(type) public : type* Clone(){return nullptr;}
#define CLONE_ASSERT(type) public : type* Clone(){assert(false); return nullptr;}

#define MAX_LAYER 32

#define ALPHABLEND_Z_POS 0.9f

#define TILEX 100
#define TILEY 100

#define TILECX 64.f
#define TILECY 64.f

#define OBJECTPOOL_SIZE 1000
enum class SHADER_DOMAIN
{
	DOMAIN_OPAQUE,		 // 불투명 단계
	DOMAIN_MASK,		 // 출력, 비출력
	DOMAIN_TRANSPARENT,  // 반투명
	DOMAIN_POST_PROCESS, // 후처리	    
	NONE,
};

enum class SAMPLER_TYPE
{
	ANISOTROPIC, // 이방성
	POINT,

	END,
};

enum class CB_TYPE
{
	TRANSFORM,	 // b0
	MATERIAL,	 // b1
	GLOBAL,	     // b2
	ANIMATION2D, // b3
	END,
};

// Rasterizer State
enum class RS_TYPE
{
	CULL_BACK,
	CULL_FRONT,
	CULL_NONE,
	WIRE_FRAME,
	END,
};

// BlendState Type
enum class BS_TYPE
{
	DEFAULT,
	ALPHABLEND,
	ONE_ONE,

	END,
};

enum class DS_TYPE
{
	LESS,
	GREATER,
	NO_WRITE,
	NO_TEST_NO_WRITE,
	END,
};
enum PIPELINE_STAGE
{
	VS = 0x01,
	HS = 0x02,
	DS = 0x04,
	GS = 0x08,
	PS = 0x10,
	ALL_STAGE = VS | HS | DS | GS | PS,
};

enum class RES_TYPE
{
	PREFAB,
	MESHDATA,
	COMPUTE_SHADER,

	MATERIAL,
	MESH,
	TEXTURE,
	SOUND,
	GRAPHICS_SHADER,
	END,
};

enum class COMPONENT_TYPE
{
	TRANSFORM,
	CAMERA,
	COLLIDER2D,
	COLLIDER3D,
	ANIMATOR2D,
	ANIMATOR3D,
	LIGHT2D,
	LIGHT3D,

	// render
	MESHRENDER,
	TILEMAP,
	PARTICLESYSTEM,
	SKYBOX,
	DECAL,
	LANDSCAPE,

	SCRIPT,
	END,
};

enum class OBJECT_TYPE
{
	LIGHT2D,
	LIGHT3D,
	END,
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

enum TEX_PARAM
{
	TEX_0,
	TEX_1,
	TEX_2,
	TEX_3,
	TEX_4,
	TEX_5,
	TEX_6,
	TEX_7,

	TEX_CUBE_0,
	TEX_CUBE_1,

	TEX_ARR_0,
	TEX_ARR_1,

	DUMMY0,
	DUMMY1,
	DUMMY2,
	DUMMY3,
	DUMMY4,
	DUMMY5,
	DUMMY6,

	TEX_8,
	TEX_9,
	TEX_10,
	TEX_11,
	TEX_12,
	TEX_13,
	TEX_14,
	TEX_15,
	TEX_16,
	TEX_17,
	TEX_18,
	TEX_19,
	TEX_20,
	TEX_21,
	TEX_22,
	TEX_23,
	TEX_24,
	TEX_25,
	TEX_26,
	TEX_27,
	TEX_28,
	TEX_29,
	TEX_30,
	TEX_31,
	TEX_32,

	TEX_END,
};

enum class DIR
{
	RIGHT,
	UP,
	FRONT,
	END,
};

enum class EVENT_TYPE
{
	CREATE_OBJECT,
	CREATE_CHILD_OBJECT,
	DELETE_OBJECT,
	CHANGE_LEVEL,

	END,
};

enum class DEBUG_SHAPE
{
	RECT,
	CIRCLE,

	CUBE,
	SPHERE,
	END,
};

enum class COLLIDER2D_TYPE
{
	COLLIDER2D_RECT,
	COLLIDER2D_CIRCLE,
};

enum class SB_TYPE
{
	SRV_ONLY,
	UAV_INC,
};

enum class LIGHT_TYPE
{
	DIRECTIONAL,
	POINT,
	SPOT,
};
enum class UI_TYPE
{
	GAMEOBJECT,
	DRAG,
	UNIT_UI,
	END,
};

enum class INSTANCING_TYPE
{
	NONE,
	USED,
};

extern const char* COMPONENT_TYPE_CHAR[(UINT)COMPONENT_TYPE::END];
extern const wchar_t* COMPONENT_TYPE_WCHAR[(UINT)COMPONENT_TYPE::END];

extern const char* OBJECT_TYPE_CHAR[(UINT)OBJECT_TYPE::END];
extern const wchar_t* OBJECT_TYPE_WCHAR[(UINT)OBJECT_TYPE::END];