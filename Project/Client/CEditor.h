#pragma once

class CComponent;
class CGameObjectEx;
class MemPool;

class CEditor
	: public CSingleton<CEditor>
{
private:
	vector<CGameObjectEx*>								m_vecEditorObj;
	vector<CGameObjectEx*>								m_vecDummyObj;
	list<tDebugShapeInfo>								m_DebugDrawList;
	array<CGameObjectEx*, (UINT)DEBUG_SHAPE::END>		m_DebugDrawObject;
	array<CComponent*, (UINT)COMPONENT_TYPE::END>		m_arrCom;
public:
	void init();
	void progress();

private:
	void tick();
	void render();
	void debug_render();
private:
	void CreateDebugDrawObject();
	void DebugDraw(tDebugShapeInfo& _info);;
public:
	void AddEditObject(CGameObjectEx* _pGameObject);
	CGameObjectEx* GetDummyObject(const wstring& _name);
	array<CComponent*, (UINT)COMPONENT_TYPE::END>& GetArrComponents() { return m_arrCom; }
	CComponent* GetArrComponent(COMPONENT_TYPE _eType) { return m_arrCom[(UINT)_eType]; }
	CGameObjectEx* FindByName(const wstring& _strky);
private:
	CEditor();
	virtual ~CEditor();
	friend class CSingleton<CEditor>;
};