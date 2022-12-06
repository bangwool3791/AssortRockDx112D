#include "pch.h"
#include "OutlinerUI.h"

#include <Engine\CGameObject.h>
#include <Engine\CLevel.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CEventMgr.h>

#include "TreeUI.h"
#include "InspectorUI.h"
#include "CImGuiMgr.h"

OutlinerUI::OutlinerUI()
	: UI("Outliner")
	, m_Tree{}
{
	m_Tree = new TreeUI("##OutlinerTree");
	AddChild(m_Tree);

	m_Tree->ShowSeperator(false);
	m_Tree->SetDummyRoot(true);

	m_Tree->AddDynamic_Selected(this, (FUNC_1)&OutlinerUI::SetObjectToInspector);
	m_Tree->AddDynamic_DragDrop(this, (FUNC_2)&OutlinerUI::AddChildObject);

	// 레벨 오브젝트 갱신
	ResetLevel();
}

OutlinerUI::~OutlinerUI()
{
}

void OutlinerUI::update()
{
	if (CEventMgr::GetInst()->IsLevelChanged())
	{
		ResetLevel();
	}
}

void OutlinerUI::render_update()
{
}

void OutlinerUI::ResetLevel()
{
	m_Tree->Clear();

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	string strLevelName = string(pCurLevel->GetName().begin(), pCurLevel->GetName().end());

	TreeNode* pRootNode = m_Tree->AddItem(nullptr, strLevelName, 0, true);

	for (UINT i{}; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecObejct = pCurLevel->GetLayer(i)->GetParentObjects();

		for (size_t j{}; j < vecObejct.size(); ++j)
		{
			AddGameObjectToTree(pRootNode, vecObejct[j]);
		}
	}

}

void OutlinerUI::SetObjectToInspector(DWORD_PTR _res)
{
	TreeNode* pSelectedNode = (TreeNode*)_res;
	CGameObject* pObject = (CGameObject*)pSelectedNode->GetData();

	InspectorUI* Inspector = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");

	Inspector->SetTargetObject(pObject);
}

void OutlinerUI::AddGameObjectToTree(TreeNode* _ParentNode, CGameObject* _Object)
{
	string strObjectName = string(_Object->GetName().begin(), _Object->GetName().end());
	TreeNode* pCurNode = m_Tree->AddItem(_ParentNode, strObjectName.c_str(), (DWORD_PTR)_Object);

	const vector<CGameObject*>& vecChild = _Object->GetChilds();

	for (size_t i{}; i < vecChild.size(); ++i)
	{
		AddGameObjectToTree(pCurNode, vecChild[i]);
	}
}

void OutlinerUI::AddChildObject(DWORD_PTR _ChildObject, DWORD_PTR _ParentObject)
{
	TreeNode* pChildNode = (TreeNode*)_ChildObject;
	TreeNode* pParentNode = (TreeNode*)_ParentObject;

	tEvent evt = {};
	evt.eType = EVENT_TYPE::ADD_CHILD;
	evt.wParam = pChildNode->GetData();
	evt.lParam = pParentNode->GetData();
	CEventMgr::GetInst()->AddEvent(evt);
}
