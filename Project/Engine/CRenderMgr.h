
#pragma once

class CCamera;
class CStructuredBuffer;

#include "CLight2D.h"
#include "CTexture.h"

class CRenderMgr
	: public CSingleton<CRenderMgr>
{
private:
	CRenderMgr();
	virtual ~CRenderMgr();
	friend class CSingleton<CRenderMgr>;
private:
	CCamera*					m_EditorCam;
	vector<CCamera*>			m_vecCam;			// 현재 레벨에 있는 모든 카메라
	vector<tLightInfo>			m_vecLight2D;		// 현재 레벨에 있는 모든 2D 광원
	CStructuredBuffer*			m_pLight2DBuffer;
	vector<tDebugShapeInfo>		m_DebugDrawInfo;	// 현재 레벨에 있는 모든 카메라
	Ptr<CTexture>				m_RTCopyTex;
public:
	void RegisterCamera(auto _pCam) { m_vecCam.push_back(_pCam); }
	void RegisterLight2D(auto _pLight) { m_vecLight2D.push_back(_pLight->GetLightInfo()); }
	void RegisterEditCam(auto _pCam) { m_EditorCam = _pCam; }
	// 렌더타겟을 카피텍스쳐로 복사
	void CopyRenderTarget();

	CCamera* GetMainCam();
	void DebugDraw(DEBUG_SHAPE _eShape, Vec4 _vColor, Vec3 _vPosition, Vec3 _vScale, Vec3 _vRotation, float _fRadius, float _fDuration)
	{
#ifdef _DEBUG
		m_DebugDrawInfo.push_back(tDebugShapeInfo{_eShape, _vColor, _vPosition, _vScale, _vRotation, _fRadius, _fDuration, 0.f} );
#endif
	}

	vector<tDebugShapeInfo>& GetDebugDrawInfo() { return m_DebugDrawInfo; }

private:
	void UpdateNoiseTexture();
	void UpdateLight2D();
public:
	void init();
	void tick();
	void render();

	void render_game();
	void render_editor();

};