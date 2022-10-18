#pragma once

#include "CComponent.h"

class CTransform
	: public CComponent 
{
public:
	CTransform();
	virtual ~CTransform();

private :
	Vec3		m_vRelativePos;
	Vec3		m_vRelativeScale;
	Vec3		m_vRelativeRotation;
	
	Vec3		m_vRelativeDir[(UINT)DIR::END];
	Matrix		m_matWorld;

public :
	void SetRelativePos(Vec3 _vPos) { m_vRelativePos = _vPos; }
	void SetRelativeScale(Vec3 _vPos) { m_vRelativeScale = _vPos; }
	void SetRelativeRotation(Vec3 _vPos) { m_vRelativeRotation = _vPos; }
	void SetRelativePos(float _x, float _y, float _z ) { m_vRelativePos = Vec3{_x,_y,_z}; }
	void SetRelativeScale(float _x, float _y, float _z) { m_vRelativeScale = Vec3{ _x,_y,_z }; }
	void SetRelativeRotation(float _x, float _y, float _z) { m_vRelativeRotation = Vec3{ _x,_y,_z }; }

	Vec3& GetRelativePos_() { return	m_vRelativePos; }
	Vec3 GetRelativePos() { return	m_vRelativePos; }
	Vec3 GetRelativeScale() { return m_vRelativeScale; }
	Vec3 GetRelativeRotation() { return m_vRelativeRotation; }
	Vec3& GetRelativeRotation_() { return m_vRelativeRotation; }
	Vec3 GetRelativeDir(DIR _eType) { return m_vRelativeDir[(UINT)_eType]; }
	Vec3& GetRelativeDir_(DIR _eType) { return m_vRelativeDir[(UINT)_eType]; }
public :
	void tick();
	void finaltick();
	void UpdateData();
};