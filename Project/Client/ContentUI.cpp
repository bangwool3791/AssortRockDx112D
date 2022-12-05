#include "pch.h"
#include "ContentUI.h"

#include <Engine\CRes.h>
#include <Engine/CResMgr.h>
#include "TreeUI.h"
#include "CImGuiMgr.h"
#include "InspectorUI.h"

ContentUI::ContentUI()
	:UI("Content")
	, m_Tree{}
{
	m_Tree = new TreeUI("##ContentTree");
	AddChild(m_Tree);

	m_Tree->ShowSeprator(false);
	m_Tree->SetDummyRoot(true);

	m_Tree->AddDynamic_Selected(this, (FUNC_1)&ContentUI::SetResourceToInspector);

	ResetContent();
}

ContentUI::~ContentUI()
{

}

void ContentUI::update()
{
	if (CResMgr::GetInst()->IsChanged())
	{
		ResetContent();
	}

	UI::update();
}

void ContentUI::render_update()
{

}

void ContentUI::ResetContent()
{
	m_Tree->Clear();

	TreeNode* pRootNode = m_Tree->AddItem(nullptr, "Content", 0);

	for (UINT i{}; i < (UINT)RES_TYPE::END; ++i)
	{
		TreeNode* pResNode = m_Tree->AddItem(pRootNode, ToString((RES_TYPE)i), 0, true);

		const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResource((RES_TYPE(i)));

		auto iter = mapRes.begin();

		for (; iter != mapRes.end(); ++iter)
		{
			m_Tree->AddItem(pResNode, string(iter->first.begin(), iter->first.end()), (DWORD_PTR)iter->second.Get());
		}
	}
}

void ContentUI::SetResourceToInspector(DWORD_PTR _res)
{
	// _res : 클릭한 노드
	TreeNode* pSelectedNode = (TreeNode*)_res;
	CRes* pRes = (CRes*)pSelectedNode->GetData();
	// InspectorUI 에 클릭된 Resouce 를 알려준다.
	InspectorUI* Inspector = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
	Inspector->SetTargetResource(pRes);
}