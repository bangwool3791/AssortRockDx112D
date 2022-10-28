#include "pch.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CEventMgr.h"
#include "CRenderMgr.h"

void Instantiate(CGameObject* _pNewObj, Vec3 _vWorldPos, int _iLayerIdx)
{
	_pNewObj->Transform()->SetRelativePos(_vWorldPos);

	tEvent evn{};

	evn.eType = EVENT_TYPE::CREATE_OBJECT;
	evn.wParam = (DWORD_PTR)_pNewObj;
	evn.lParam = (DWORD_PTR)_iLayerIdx;

	CEventMgr::GetInst()->AddEvent(evn);

}

void Instantiate(CGameObject* _pNewObj, const Matrix& _matrix)
{
	_pNewObj->Transform()->SetRelativePos(Vec3{ _matrix.m[3][0],_matrix.m[3][1],_matrix.m[3][2] });
	
	Vec3 Right	= Vec3{ _matrix.m[0][0],_matrix.m[0][1],_matrix.m[0][2] };
	Vec3 Up		= Vec3{ _matrix.m[1][0],_matrix.m[1][1],_matrix.m[1][2] };
	Vec3 Look	= Vec3{ _matrix.m[2][0],_matrix.m[2][1],_matrix.m[2][2] };

	_pNewObj->Transform()->SetRelativeScale(Right.Length(), Right.Length(), 0.f);
	
	tEvent evn{};

	evn.eType = EVENT_TYPE::CREATE_CHILD_OBJECT;
	evn.wParam = (DWORD_PTR)_pNewObj;

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
