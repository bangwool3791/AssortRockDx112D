#pragma once
#include "CRes.h"
#include "pch.h"
#include "CTexture.h"
#include "CGraphicsShader.h"

class CMaterial :
	public CRes
{
private:
	tMtrlConst						m_tConst;
	Ptr<CTexture>					m_arrTex[TEX_END];
	Ptr<CGraphicsShader>			m_pShader;

	Ptr<CMaterial>					m_pMasterMtrl;
public :
	void SetScalarParam(SCALAR_PARAM _eScalarType, void* _pData);
	void SetShader(Ptr<CGraphicsShader> _pShader) 
	{ 
		m_pShader = _pShader; 
	}
	void SetTexParam(TEX_PARAM _eTex, Ptr<CTexture> _pTex);

	void UpdateData();

	virtual int Load(const wstring& _strFilePath) { return S_OK; }

public :
	CMaterial();
	CMaterial(const CMaterial& _other)
		: CRes(RES_TYPE::MATERIAL)
		, m_tConst(_other.m_tConst)
		, m_pShader(_other.m_pShader)
	{
		m_pMasterMtrl = const_cast<CMaterial*>(&_other);
	}

	~CMaterial();
};