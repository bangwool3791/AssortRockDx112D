#pragma once
#include "CEntity.h"

class CComponent;
class CCamera;
class CTransform;
class CRenderComponent;
class CMeshRender;
class CScript;

#define GET_COMPONENT(classname, CLASSNAME ) C##classname* classname() { return ( C##classname*)GetComponent(COMPONENT_TYPE::CLASSNAME); }


class CGameObject
	:public CEntity
{
private:
	array<CComponent*, (UINT)COMPONENT_TYPE::END> m_arrCom;
	vector<CScript*>	m_vecScripts;
	CRenderComponent* m_pRenderComponent{};
public :
	void begin();
	void tick();
	void finaltick();
	void render();

public :
	void AddComponent(CComponent* _pComponent);
	CComponent* GetComponent(COMPONENT_TYPE _eComType);
	GET_COMPONENT(Transform,  TRANSFORM);
	GET_COMPONENT(MeshRender, MESHRENDER);
	GET_COMPONENT(Camera,	  CAMERA);
	template<typename T>
	T* GetScript();
public :
	CGameObject();
	virtual ~CGameObject();
};

template<typename T>
T* CGameObject::GetScript()
{
	for (size_t i{ 0 }; i < m_vecScripts.size(); ++i)
	{
		T* pScript = dynamic_cast<T*>(m_vecScripts[i]);
		if (nullptr != pScript)
			return pScript;
	}
	return nullptr;
}