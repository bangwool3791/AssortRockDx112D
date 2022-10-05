#pragma once

#include "CComponent.h"

class CTransform
	: public CComponent 
{
public:
	CTransform();
	~CTransform();

private :
	Vec3		m_vRelativePos;
	Vec3		m_vRelativeScale;
	Vec3		m_vRelativeRotation;

public :
	void SetRelativePos(Vec3 _vPos) { m_vRelativePos = _vPos; }
	void SetRelativeScale(Vec3 _vPos) { m_vRelativeScale = _vPos; }
	void SetRelativeRotation(Vec3 _vPos) { m_vRelativeRotation = _vPos; }
	void SetRelativePos(float _x, float _y, float _z ) { m_vRelativePos = Vec3{_x,_y,_z}; }
	void SetRelativeScale(float _x, float _y, float _z) { m_vRelativeScale = Vec3{ _x,_y,_z }; }
	void SetRelativeRotation(float _x, float _y, float _z) { m_vRelativeRotation = Vec3{ _x,_y,_z }; }

	Vec3 GetRelativePos() { return	m_vRelativePos; }
	Vec3 GetRelativeScale() { return m_vRelativeScale; }
	Vec3 GetRelativeRotation() { return m_vRelativeRotation; }
public :
	void tick();
	void finaltick();
	void UpdateData();
};