#pragma once
#include "CEntity.h"

class CComponent;
class CTransform;
class CMeshRender;

#define GET_COMPONENT(classname, CLASSNAME ) C##classname* classname() { return ( C##classname*)GetComponent(COMPONENT_TYPE::CLASSNAME); }


class CGameObject
	:public CEntity
{
private:
	array<CComponent*, (UINT)COMPONENT_TYPE::END> m_arrCom;

public :
	void tick();
	void finaltick();
	void render();

public :
	void AddComponent(CComponent* _pComponent);
	CComponent* GetComponent(COMPONENT_TYPE _eComType);
	GET_COMPONENT(Transform, TRANSFORM);
	GET_COMPONENT(MeshRender, MESHRENDER);

public :
	CGameObject();
	virtual ~CGameObject();
};