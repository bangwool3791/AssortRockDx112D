#include "pch.h"
#include "CCollider2D.h"
#include "CTransform.h"
#include "CScript.h"

CCollider2D::CCollider2D()
	: CComponent(COMPONENT_TYPE::COLLIDER2D)
	, m_vScale(Vec2(1.f, 1.f))
	, m_eType(COLLIDER2D_TYPE::COLLIDER2D_RECT)
	, m_bIgnorObjectScale(false)
	, m_bPause{false}
{
	SetName(L"CCollider2D");
}

CCollider2D::~CCollider2D()
{
}

void CCollider2D::finaltick()
{
	// FinalPos 계산
	if (nullptr == Transform())
		return;

	Vec3 vObjectPos = Transform()->GetWorldPos();
	m_vFinalPos = m_vOffsetPos + Vec2(vObjectPos.x, vObjectPos.z);

	// 행렬 연산
	// 크 회 이
	m_vFinalScale = Vec2(m_vScale.x, m_vScale.y);

	if (!m_bIgnorObjectScale)
	{
		Vec3 vWorldScale = Transform()->GetWorldScale();
		m_vFinalScale *= Vec2(vWorldScale.x, vWorldScale.z);
	}

	Matrix matScale = XMMatrixScaling(m_vFinalScale.x, m_vFinalScale.y, 1.f);

	m_vRot *= Transform()->GetRelativeRotation();
	Matrix matRot = XMMatrixRotationX(m_vRot.x);
	matRot *= XMMatrixRotationY(m_vRot.y);
	matRot *= XMMatrixRotationZ(m_vRot.z);

	Matrix matTrans = XMMatrixTranslation(m_vFinalPos.x, 0.f, m_vFinalPos.y);

	// 충돌체 월드 행렬
	m_matWorld = matScale * matRot * matTrans;

	// DebugDraw 요청
#ifdef _DEBUG
	Vec4 Color = { 0.f, 0.f, 1.f, 1.f };
	if(0 < m_iOverlapCount)
		Color = Vec4{ 1.f, 0.f, 0.f, 1.f };

	if (COLLIDER2D_TYPE::COLLIDER2D_RECT == m_eType)
	{
		DebugDrawRect(Color, Vec3(m_vFinalPos.x, 0.f, m_vFinalPos.y), Vec3(m_vFinalScale.x, 1.f, m_vFinalScale.y), m_vRot);
	}
	else
	{
		DebugDrawCircle(Color, Vec3(m_vFinalPos.x, 0.f, m_vFinalPos.y), m_vFinalScale.x);
	}
#endif
}

void CCollider2D::BeginOverlap(CCollider2D* _pOther)
{
	++m_iOverlapCount;
	const vector<CScript*>& vecScripts = GetOwner()->GetScripts();
	for (auto iter{ vecScripts.begin() }; iter != vecScripts.end(); ++iter)
	{
		(*iter)->BeginOverlap(_pOther);
	}
}

void CCollider2D::Overlap(CCollider2D* _pOther)
{
	const vector<CScript*>& vecScripts = GetOwner()->GetScripts();
	for (auto iter{ vecScripts.begin() }; iter != vecScripts.end(); ++iter)
	{
		(*iter)->Overlap(_pOther);
	}
}

void CCollider2D::EndOverlap(CCollider2D* _pOther)
{
	--m_iOverlapCount;
	const vector<CScript*>& vecScripts = GetOwner()->GetScripts();
	for (auto iter{ vecScripts.begin() }; iter != vecScripts.end(); ++iter)
	{
		(*iter)->EndOverlap(_pOther);
	}
}

void CCollider2D::SetRotationX(float _fDegree) 
{ 
	Transform()->SetRelativeRotationX(_fDegree);
}
void CCollider2D::SetRotationY(float _fDegree)
{
	Transform()->SetRelativeRotationY(_fDegree);
}
void CCollider2D::SetRotationZ(float _fDegree)
{ 
	Transform()->SetRelativeRotationZ(_fDegree);
}

void CCollider2D::SaveToFile(FILE* _File)
{
	COMPONENT_TYPE type = GetType();
	fwrite(&type, sizeof(UINT), 1, _File);

	fwrite(&m_vOffsetPos, sizeof(Vec3), 1, _File);
	fwrite(&m_vScale, sizeof(Vec3), 1, _File);
	fwrite(&m_vRot, sizeof(Vec3), 1, _File);
	fwrite(&m_eType, sizeof(UINT), 1, _File);
	fwrite(&m_bIgnorObjectScale, sizeof(bool), 1, _File);
}

void CCollider2D::LoadFromFile(FILE* _File)
{
	fread(&m_vOffsetPos, sizeof(Vec3), 1, _File);
	fread(&m_vScale, sizeof(Vec3), 1, _File);
	fread(&m_vRot, sizeof(Vec3), 1, _File);
	fread(&m_eType, sizeof(UINT), 1, _File);
	fread(&m_bIgnorObjectScale, sizeof(bool), 1, _File);
}