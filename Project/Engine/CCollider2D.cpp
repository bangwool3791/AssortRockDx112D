#include "pch.h"
#include "CCollider2D.h"

#include "CTransform.h"

CCollider2D::CCollider2D()
	: CComponent(COMPONENT_TYPE::COLLIDER2D)
	, m_vScale(Vec2(1.f, 1.f))
	, m_eType(COLLIDER2D_TYPE::COLLIDER2D_RECT)
	, m_bIgnorObjectScale(false)
{
}

CCollider2D::~CCollider2D()
{
}

void CCollider2D::finaltick()
{
	// FinalPos ���
	Vec3 vObjectPos = Transform()->GetWorldPos();
	m_vFinalPos = m_vOffsetPos + Vec2(vObjectPos.x, vObjectPos.y);

	// ��� ����
	// ũ ȸ ��
	m_vFinalScale = Vec2(m_vScale.x, m_vScale.y);
	if (!m_bIgnorObjectScale)
	{
		Vec3 vWorldScale = Transform()->GetWorldScale();
		m_vFinalScale *= Vec2(vWorldScale.x, vWorldScale.y);
	}

	Matrix matScale = XMMatrixScaling(m_vFinalScale.x, m_vFinalScale.y, 1.f);

	Matrix matRot = XMMatrixRotationX(m_vRot.x);
	matRot *= XMMatrixRotationY(m_vRot.y);
	matRot *= XMMatrixRotationZ(m_vRot.z);

	Matrix matTrans = XMMatrixTranslation(m_vFinalPos.x, m_vFinalPos.y, 0.f);

	// �浹ü ���� ���
	m_matWorld = matScale * matRot * matTrans;

	// DebugDraw ��û
#ifdef _DEBUG
	if (COLLIDER2D_TYPE::COLLIDER2D_RECT == m_eType)
	{
		DebugDrawRect(Vec4(0.f, 1.f, 0.f, 1.f), Vec3(m_vFinalPos.x, m_vFinalPos.y, 0.f), Vec3(m_vFinalScale.x, m_vFinalScale.y, 1.f), m_vRot);
	}
	else
	{
		DebugDrawCircle(Vec4(0.f, 1.f, 0.f, 1.f), Vec3(m_vFinalPos.x, m_vFinalPos.y, 0.f), m_vFinalScale.x);
	}
#endif
}
