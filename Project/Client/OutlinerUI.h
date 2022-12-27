#pragma once
#include "UI.h"

class TreeUI;
class TreeNode;
class CGameObject;
class PopupUI;
class OutlinerUI :
	public UI
{
private:
	CGameObject* m_pGameObject;
	TreeUI*      m_Tree;
private:
	virtual void update() override;
	virtual void render_update() override;

public:
	void ResetLevel();

private:
	void SetObject(DWORD_PTR _res);
	void SetObjectToInspector(DWORD_PTR _res);
	void AddGameObjectToTree(TreeNode* _ParentNode, CGameObject* _Object);

	void AddChildObject(DWORD_PTR _ChildObject, DWORD_PTR _ParentObject);

public :
	OutlinerUI();
	~OutlinerUI();
};