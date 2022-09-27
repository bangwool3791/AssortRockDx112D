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

#define KEY_PRESSED(key) CKeyMgr::GetInst()->GetKeyState(key) == KEY_STATE::PRESS
#define KEY_TAP(key) CKeyMgr::GetInst()->GetKeyState(key) == KEY_STATE::TAP
#define KEY_RELEASE(key) CKeyMgr::GetInst()->GetKeyState(key) == KEY_STATE::RELEASE
