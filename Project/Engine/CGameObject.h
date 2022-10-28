#pragma once
#include "CEntity.h"

class CComponent;
class CCamera;
class CTransform;
class CRenderComponent;
class CMeshRender;
class CScript;
class CCollider2D;

#define GET_COMPONENT(classname, CLASSNAME ) C##classname* classname() { return ( C##classname*)GetComponent(COMPONENT_TYPE::CLASSNAME); }


class CGameObject
	:public CEntity
{
private:
	array<CComponent*, (UINT)COMPONENT_TYPE::END> m_arrCom;
	vector<CScript*>							  m_vecScripts;
	CRenderComponent* m_pRenderComponent{};
protected:
	//부모 오브젝트
	CGameObject*								  m_pParent;
	vector<CGameObject*>						  m_vecChild;
	int											  m_iLayerIdx;
	bool										  m_bDead;
public:
	CGameObject* Get_Parent(){return m_pParent;}
	const vector<CGameObject*>& GetChilds() const {return m_vecChild;}
	const vector<CScript*>& GetScripts() const {return m_vecScripts;}
	int GetLayerIndex(){return m_iLayerIdx;}

	bool IsDead() { return m_bDead; }
	void SetDead(bool _bChecked) { m_bDead = _bChecked; }
public :
	virtual void begin();
	virtual void tick();
	virtual void finaltick();
	virtual void render();
public :
	CRenderComponent* GetRenderComponent() {return m_pRenderComponent;}

public :
	friend class CLayer;
public :
	void AddComponent(CComponent* _pComponent);
	void AddChild(CGameObject* _pGameObejct)
	{
		_pGameObejct->m_pParent = this;
		m_vecChild.push_back(_pGameObejct);
	}
	CComponent* GetComponent(COMPONENT_TYPE _eComType);

	GET_COMPONENT(Transform,  TRANSFORM);
	GET_COMPONENT(MeshRender, MESHRENDER);
	GET_COMPONENT(Camera,	  CAMERA);
	GET_COMPONENT(Collider2D, COLLIDER2D);
	template<typename T>
	T* GetScript();

	CLONE(CGameObject);
public :
	CGameObject();
	CGameObject(const CGameObject& rhs);
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