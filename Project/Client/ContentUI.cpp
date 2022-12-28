#include "pch.h"
#include "ContentUI.h"

#include "CImGuiMgr.h"

#include "TreeUI.h"
#include "InspectorUI.h"
#include "CEditorCam.h"
#include "CGameObjectEx.h"

#include "CEditor.h"

#include <Engine\CRes.h>
#include <Engine/CResMgr.h>
#include <Engine\CDevice.h>
#include <Engine\CKeyMgr.h>
#include <Engine\CTransform.h>
#include <Engine/CCamera.h>

ContentUI::ContentUI()
	:UI("Content")
	, m_Tree{}
	, m_bDragEvent{false}
{
	m_Tree = new TreeUI("##ContentTree");
	AddChild(m_Tree);

	m_Tree->ShowSeperator(false);
	m_Tree->SetDummyRoot(true);

	m_Tree->AddDynamic_LBtn_Selected(this, (FUNC_1)&ContentUI::SetResourceToInspector);
	m_Tree->AddDynamic_DragDrop_World(this, (FUNC_1)&ContentUI::SetDragObject);
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

	if (m_bDragEvent && nullptr != m_pTargetPrefab)
	{
		if (KEY_RELEASE(KEY::LBTN))
		{
			CGameObjectEx* pCameraObject = CEditor::GetInst()->FindByName(L"Editor Camera");
			Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();

			Vec2 Resolution = CDevice::GetInst()->GetRenderResolution();
			float fScale = pCameraObject->Camera()->GetOrthographicScale();
			Resolution *= fScale;
			Vec3  vCameraPos = pCameraObject->Transform()->GetRelativePos();

			Vec3 vPos{ vMousePos.x, vMousePos.y, 1.f };
			vPos.x -= Resolution.x * 0.5f;
			vPos.y = vPos.y * -1.f + Resolution.y * 0.5f;
			vPos *= fScale;
			vPos.x += vCameraPos.x;
			vPos.y += vCameraPos.y;

			if (Resolution.x * -0.5f <= vPos.x && Resolution.y * -0.5f <= vPos.y &&
				Resolution.x * 0.5f >= vPos.x && Resolution.y * 0.5f >= vPos.y)
			{
				CGameObject* pGameObject = m_pTargetPrefab->Instantiate();

				Instantiate(pGameObject, vPos, 1);
			}
		}
	}

	if (KEY_PRESSED(KEY::LCTRL) && KEY_PRESSED(KEY::S))
	{
		TreeNode* pSelectedNode = m_Tree->GetSelectedNode();

		if (nullptr != pSelectedNode)
		{
			if (pSelectedNode->GetData())
			{
				RES_TYPE type = ((CRes*)pSelectedNode->GetData())->GetResType();

				wstring strRelativePath;

				switch (type)
				{
				case RES_TYPE::PREFAB:
					break;
				case RES_TYPE::MESHDATA:
					break;
				case RES_TYPE::COMPUTE_SHADER:
					break;
				case RES_TYPE::MATERIAL:
				{
					Ptr<CMaterial> pRes = (CMaterial*)pSelectedNode->GetData();
					strRelativePath = L"\\material\\";
					strRelativePath += pRes->GetKey();
					pRes->Save(strRelativePath);
				}
				break;
				case RES_TYPE::MESH:
					break;
				case RES_TYPE::TEXTURE:
					break;
				case RES_TYPE::SOUND:
					break;
				case RES_TYPE::GRAPHICS_SHADER:
					break;
				case RES_TYPE::END:
					break;
				default:
					break;
				}
			}
		}
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


void ContentUI::SetDragObject(DWORD_PTR _res)
{
	// _res : 클릭한 노드
	TreeNode* pSelectedNode = (TreeNode*)_res;
	m_pTargetPrefab = (CPrefab*)pSelectedNode->GetData();
	m_bDragEvent = true;
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