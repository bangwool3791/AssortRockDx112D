#pragma once

#include "CLayer.h"

class CLevel 
	: public CEntity
{
private:
	array<CLayer, MAX_LAYER>		m_arrLayer;

public :
	void begin();
	void tick();
	void finaltick();
	void render();

public:
	CLayer* GetLayer(UINT _iIdx) { return &m_arrLayer[_iIdx]; }
	CLayer* GetLayer(const wstring& _strLayerName);

	void AddGameObject(CGameObject* _pObject, UINT _iLayerIdx);
	void AddGameObject(CGameObject* _pObject, const wstring& _strLayerName);

	CLONE_DUMMY(CLevel);
public:
	CLevel();
	virtual ~CLevel();
};