#include "pch.h"
#include "CEventMgr.h"

#include "CGameObject.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CLevelMgr.h"

CEventMgr::CEventMgr()
{

}

CEventMgr::~CEventMgr()
{

}

void CEventMgr::tick()
{
	m_bLevelChanged = false;

	for (UINT i{}; i < m_vecGarbage.size(); ++i)
	{
		delete m_vecGarbage[i];
		m_bLevelChanged = true;
	}
	m_vecGarbage.clear();

	for (auto iter{ m_vecEvent.begin()}; iter != m_vecEvent.end(); ++iter)
	{
		switch (iter->eType)
		{
		case EVENT_TYPE::CREATE_OBJECT:
		{
			CGameObject* pGameObeject = (CGameObject*)iter->wParam;
			int	iLayer = (int)iter->lParam;
			CLevel* level = CLevelMgr::GetInst()->GetCurLevel();
			pGameObeject->begin();
			level->AddGameObject(pGameObeject, iLayer);
			m_bLevelChanged = true;
		}
			break;
		case EVENT_TYPE::ADD_CHILD:
		{
			// wParam : Child Adress, lParam : Parent Adress
			CGameObject* pParent = (CGameObject*)iter->lParam;
			CGameObject* pChild = (CGameObject*)iter->wParam;
			pChild->begin();
			pParent->AddChild(pChild);
			m_bLevelChanged = true;
		}
			break;
		case EVENT_TYPE::DELETE_OBJECT:
		{
			static queue<CGameObject*> que;
			CGameObject* pGameObj = (CGameObject*)iter->wParam;
			if (!pGameObj->IsDead())
			{
				que.push(pGameObj);

				while (!que.empty())
				{
					CGameObject* pObj = (CGameObject*)que.front();
					que.pop();
					m_vecGarbage.push_back(pObj);

					vector<CGameObject*> vecChild = pObj->GetChilds();

					for (auto iter{ vecChild.begin() }; iter != vecChild.end(); ++iter)
					{
						que.push(*iter);
					}

					pObj->m_bDead = true;
				}
			}
		}
			break;
		case EVENT_TYPE::CHANGE_LEVEL:
		{
			m_bLevelChanged = true;
		}
			break;
		case EVENT_TYPE::CHANGE_LEVEL_STATE:
		{
			m_bLevelChanged = true;
			CLevelMgr::GetInst()->ChangeLevelState((LEVEL_STATE)iter->wParam);
		}
			break;

		case EVENT_TYPE::EDIT_RES:
		{
			switch ((RES_TYPE)iter->wParam)
			{
			case RES_TYPE::PREFAB:
			{
				if (((CRes*)iter->lParam)->GetName().empty())
					break;

				wchar_t sz_data[255] = {};
				lstrcpy(sz_data, ((CRes*)iter->lParam)->GetName().c_str());
				wstring wstrRelativePath = lstrcat(sz_data, L"Prefab");
				CResMgr::GetInst()->AddRes(wstrRelativePath, new CPrefab(((CGameObject*)iter->lParam)->Clone(), false));
			}
				break;
			case RES_TYPE::COMPUTE_SHADER:
				CResMgr::GetInst()->AddRes(((CRes*)iter->lParam)->GetName(), (CComputeShader*)iter->lParam);
				break;
			case RES_TYPE::MATERIAL:
				CResMgr::GetInst()->AddRes<CMaterial>(((CRes*)iter->lParam)->GetName(), (CMaterial*)(iter->lParam));
				break;
			case RES_TYPE::MESH:
				CResMgr::GetInst()->AddRes(((CRes*)iter->lParam)->GetName(), (CMesh*)iter->lParam);
				break;
			case RES_TYPE::TEXTURE:
				CResMgr::GetInst()->AddRes(((CRes*)iter->lParam)->GetName(), (CTexture*)iter->lParam);
				break;
			default:
				break;
			}
		}
			break;
		case EVENT_TYPE::DELETE_RES:
		{
			// wParam : RES_TYPE, lParam : Resource Adress
			if (!CResMgr::GetInst()->DeleteRes((RES_TYPE)iter->wParam, ((CRes*)iter->lParam)->GetKey()))
			{
				MessageBox(nullptr, L"리소스 삭제 실패", L"에러", MB_OK);
			}
			break;
		}
		}
	}

	m_vecEvent.clear();
}

