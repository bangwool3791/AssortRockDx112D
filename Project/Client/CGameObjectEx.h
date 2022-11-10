#pragma once
#include <Engine/CGameObject.h>
#include <Engine/smallobjallocator.h>
class CGameObjectEx
	:public CGameObject
{
public :
	virtual void finaltick();

public :
	CGameObjectEx();
	virtual ~CGameObjectEx();
};