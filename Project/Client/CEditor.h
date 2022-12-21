#pragma once

class CComponent;
class CGameObjectEx;
class MemPool;

/*
* 카메라 마우스 항상 동작
* Editor Object 변경
*/
enum class EDIT_MODE
{
	ANIMATOR,
	MAPTOOL,
	OBJECT,
	END,
};

class CEditor
	: public CSingleton<CEditor>
{
private:
	EDIT_MODE											m_editmode;
	/*
	* 객체 늘어나면 map<vector>처리
	*/
	CGameObjectEx*										m_pCameraObject;
	CGameObjectEx*										m_MouseObject;
	CGameObjectEx*										m_GirdObject;
	vector<map<const wchar_t*, CGameObjectEx*>>			m_EditorObj;

	vector<CGameObjectEx*>								m_vecDummyObj;
	list<tDebugShapeInfo>								m_DebugDrawList;
	array<CGameObjectEx*, (UINT)DEBUG_SHAPE::END>		m_DebugDrawObject;
	array<CComponent*, (UINT)COMPONENT_TYPE::END>		m_arrCom;
public:
	void init();
	void progress();
public :
	void SetEditmode(EDIT_MODE _editmode);
private:
	void tick();
	void render();
	void debug_render();
private:
	void CreateDebugDrawObject();
	void CreateTileMap(CGameObject* _pCamera, CGameObject* _pMouse);
	void CreateAnimatorTool(CGameObject* _pCamera, CGameObject* _pMouse);
	void DebugDraw(tDebugShapeInfo& _info);
public:
	void Add_Editobject(EDIT_MODE _emode, CGameObjectEx* _pGameObject);
	void Add_Editobject(EDIT_MODE _emode, const wchar_t* _pName, CGameObjectEx* _pGameObject);
	CGameObjectEx* GetDummyObject(const wstring& _name);
	array<CComponent*, (UINT)COMPONENT_TYPE::END>& GetArrComponents() { return m_arrCom; }
	CComponent* GetArrComponent(COMPONENT_TYPE _eType) { return m_arrCom[(UINT)_eType]; }
	CGameObjectEx* FindByName(const wstring& _strky);
	void PopByName(const wstring& _strky);
private:
	CEditor();
	virtual ~CEditor();
	friend class CSingleton<CEditor>;
};