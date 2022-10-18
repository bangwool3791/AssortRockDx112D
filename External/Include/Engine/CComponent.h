#pragma once

#include "CEntity.h"
#include "CGameObject.h"

#define GET_OTHER_COMPONENT(ComponentName) C##ComponentName* ComponentName() { return m_pOwnerObject->ComponentName(); }

class CComponent
	:public CEntity
{
private:
	CGameObject* m_pOwnerObject;
	const COMPONENT_TYPE m_eType;
public:
	virtual void begin() {}
	virtual void tick() {};
	virtual void finaltick()= 0;

public :
	COMPONENT_TYPE GetType() { return m_eType; }
	CGameObject* GetOwner() { return m_pOwnerObject; }

	GET_OTHER_COMPONENT(Transform);
	GET_OTHER_COMPONENT(MeshRender);
	GET_OTHER_COMPONENT(Camera);

public :
	CComponent(COMPONENT_TYPE _eType);
	virtual ~CComponent();

	friend class CGameObject;

};