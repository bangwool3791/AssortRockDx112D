#pragma once
#include "CRes.h"

class CGameObject;

class CPrefab :
	public CRes
{
private :
	CGameObject* m_pProtoObj;

public :
	CGameObject* Instantiate();

	virtual int Load(const wstring& _strFilePath) { return E_FAIL; }

	CLONE_DUMMY(CPrefab);

public:
	CPrefab();
	CPrefab(CGameObject* _pProto);
	~CPrefab();
};