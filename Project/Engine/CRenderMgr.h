
#pragma once

class CCamera;

class CRenderMgr
	: public CSingletone<CRenderMgr>
{
public:
	CRenderMgr();
	virtual ~CRenderMgr();

private:
	vector<CCamera*>		m_vecCam;	// 현재 레벨에 있는 모든 카메라
	vector<tDebugShapeInfo>	m_DebugDrawInfo;	// 현재 레벨에 있는 모든 카메라
public:
	void RegisterCamera(auto _pCam) { m_vecCam.push_back(_pCam); }

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
public:
	void init();
	void tick();
	void render();

};