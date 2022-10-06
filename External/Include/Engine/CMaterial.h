#pragma once
#include "CRes.h"
#include "pch.h"
#include "CGraphicsShader.h"

class CMaterial :
	public CRes
{
private:
	tMtrlConst			m_tConst;
	Ptr<CGraphicsShader>	m_pShader;
public :
	void SetScalarParam(SCALAR_PARAM _eScalarType, void* _pData);
	void SetShader(Ptr<CGraphicsShader> _pShader) { m_pShader = _pShader; }
	
	void UpdateData();

	virtual int Load(const wstring& _strFilePath) { return S_OK; }

public :
	CMaterial();
	~CMaterial();
};