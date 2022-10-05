#pragma once

#include "CEntity.h"

class CGameObject;

class CLayer
	:public CEntity
	
{
public :
	CLayer();
	~CLayer();

public :
	void tick();
	void finaltick();
	void render();

private :
	vector<CGameObject*> m_vecGameObject;
public :
	void AddGameObject(auto _pObject)
	{
		back_insert_iterator iter{ m_vecGameObject };
		*iter = _pObject;
	}

};