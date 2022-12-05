#pragma once

#include "CEntity.h"
#include "CTexture.h"

#include "pch.h"

class CAnimator2D;

class CAnimation2D :
	public CEntity
{
private:
	vector<tAnim2DFrm>	m_vecFrm;
	int					m_iCurIdx;

	CAnimator2D*		m_pOwner;
	Ptr<CTexture>		m_AtlasTex;

	float				m_fAccTime;
	bool				m_bFinish;

	tAnim2DInfo			m_info;
public :
	void finaltick();
public:
	const vector<tAnim2DFrm>& GetFames() { return m_vecFrm; }
	const tAnim2DInfo& GetAniInfo() { return m_info; }
	const tAnim2DFrm& GetAniFrame() { return m_vecFrm[m_iCurIdx]; }
	tAnim2DFrm&	GetAniFrameCopy() { return m_vecFrm[m_iCurIdx]; }

	void SetLeftTop(Vec2 _vLeftTop, int _index);
	void SetSlice(Vec2 _vSlice, int _index);
	void SetOffset(Vec2 _vOffset, int _index);
	void SetFullSize(Vec2 _vFullSize, int _index);
	void SetDuration(float _fDuration, int _index);
public :
	void Create(const wstring& _strKey, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, float _fStep, int _iMaxFrm, float _FPS);
	void Reset()
	{
		m_fAccTime = 0.f;
		m_iCurIdx = 0;
		m_bFinish = false;
	}

	void UpdateData();
	void Clear();
	void SetOwner(CAnimator2D* _pOwner) {
		m_pOwner = _pOwner;
	}
	CLONE(CAnimation2D);

public:
	CAnimation2D();
	virtual ~CAnimation2D();

	friend class CAnimator2D;
};