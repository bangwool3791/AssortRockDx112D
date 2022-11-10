
#pragma once

class CCamera;
class CStructuredBuffer;

#include "CLight2D.h"

class CRenderMgr
	: public CSingletone<CRenderMgr>
{
public:
	CRenderMgr();
	virtual ~CRenderMgr();

private:
	vector<CCamera*>		m_vecCam;			// ���� ������ �ִ� ��� ī�޶�
	vector<tLightInfo>		m_vecLight2D;		// ���� ������ �ִ� ��� 2D ����
	CStructuredBuffer*		m_pLight2DBuffer;
	
	vector<tDebugShapeInfo>	m_DebugDrawInfo;	// ���� ������ �ִ� ��� ī�޶�
public:
	void RegisterCamera(auto _pCam) { m_vecCam.push_back(_pCam); }
	void RegisterLight2D(auto _pLight) { m_vecLight2D.push_back(_pLight->GetLightInfo()); }

	CCamera* GetMainCam()
	{
		if (m_vecCam.empty())
			return nullptr;

		return m_vecCam[0];
	}

	void DebugDraw(DEBUG_SHAPE _eShape, Vec4 _vColor, Vec3 _vPosition, Vec3 _vScale, Vec3 _vRotation, float _fRadius, float _fDuration)
	{
#ifdef _DEBUG
		m_DebugDrawInfo.push_back(tDebugShapeInfo{_eShape, _vColor, _vPosition, _vScale, _vRotation, _fRadius, _fDuration, 0.f} );
#endif
	}

	vector<tDebugShapeInfo>& GetDebugDrawInfo() { return m_DebugDrawInfo; }

private:
	void UpdateLight2D();
public:
	void init();
	void tick();
	void render();

};