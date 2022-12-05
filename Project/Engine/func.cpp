#include "pch.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CEventMgr.h"
#include "CRenderMgr.h"

void Instantiate(CGameObject* _pNewObj, int _iLayerIdx)
{
	tEvent evn{};

	evn.eType = EVENT_TYPE::CREATE_OBJECT;
	evn.wParam = (DWORD_PTR)_pNewObj;
	evn.lParam = (DWORD_PTR)_iLayerIdx;

	CEventMgr::GetInst()->AddEvent(evn);
}

void Instantiate(CGameObject* _pNewObj, Vec3 _vWorldPos, int _iLayerIdx)
{
	_pNewObj->Transform()->SetRelativePos(_vWorldPos);

	tEvent evn{};

	evn.eType = EVENT_TYPE::CREATE_OBJECT;
	evn.wParam = (DWORD_PTR)_pNewObj;
	evn.lParam = (DWORD_PTR)_iLayerIdx;

	CEventMgr::GetInst()->AddEvent(evn);
}

void Instantiate(CGameObject* _pNewObj, CGameObject* _pOwner, int _iLayerIdx)
{
	tEvent evn{};

	evn.eType = EVENT_TYPE::ADD_CHILD;
	evn.wParam = (DWORD_PTR)_pNewObj;
	evn.lParam = (DWORD_PTR)_pOwner;

	CEventMgr::GetInst()->AddEvent(evn);
}

void DebugDrawRect(Vec4 _vColor, Vec3 _vPosition, Vec3 _vScale, Vec3 _vRotation, float _fDuration)
{
	CRenderMgr::GetInst()->DebugDraw(DEBUG_SHAPE::RECT, _vColor, _vPosition, _vScale, _vRotation, 0.f, _fDuration);
}

void DebugDrawCircle(Vec4 _vColor, Vec3 _vPosition, float _fRadius, float _fDuration)
{
	CRenderMgr::GetInst()->DebugDraw(DEBUG_SHAPE::CIRCLE, _vColor, _vPosition, Vec3::Zero, Vec3::Zero, _fRadius, _fDuration);
}

const char* ToString(COMPONENT_TYPE _type)
{
	return COMPONENT_TYPE_CHAR[(UINT)_type];
}

const wchar_t* ToWString(COMPONENT_TYPE _type)
{
	return COMPONENT_TYPE_WCHAR[(UINT)_type];
}

const char* ToString(OBJECT_TYPE _type)
{
	return OBJECT_TYPE_CHAR[(UINT)_type];
}

const wchar_t* ToWString(OBJECT_TYPE _type)
{
	return OBJECT_TYPE_WCHAR[(UINT)_type];
}

const char* ToString(RES_TYPE _type)
{
	return RES_TYPE_CHAR[(UINT)_type];
}

const wchar_t* ToWString(RES_TYPE _type)
{
	return RES_TYPE_WCHAR[(UINT)_type];
}