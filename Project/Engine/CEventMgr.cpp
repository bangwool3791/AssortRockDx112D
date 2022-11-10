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
	Safe_Del_Vec(m_vecGarbage);

	for (auto iter{ m_vecEvent.begin()}; iter != m_vecEvent.end(); ++iter)
	{
		switch (iter->eType)
		{
		case EVENT_TYPE::CREATE_OBJECT:
		{
			CGameObject* pGameObeject = (CGameObject*)iter->wParam;
			int	iLayer = (int)iter->lParam;
			CLevel* level = CLevelMgr::GetInst()->GetCurLevel();
			level->AddGameObject(pGameObeject, iLayer);
		}
			break;
		case EVENT_TYPE::CREATE_CHILD_OBJECT:
		{
			CGameObject* pGameObeject = (CGameObject*)iter->wParam;
			CGameObject* pOwner      =	(CGameObject*)iter->oParam;
			pOwner->AddChild(pGameObeject);
			pGameObeject->SetLayerIndex((UINT)iter->lParam);
		}
			break;
		case EVENT_TYPE::DELETE_OBJECT:
		{
			static queue<CGameObject*> que;
			CGameObject* pGameObj = (CGameObject*)iter->wParam;
			if (!pGameObj->IsDead())
			{
				que.push(pGameObj);

				if (!lstrcmp(L"UnitSelectUI", pGameObj->GetName().c_str()))
				{
					int a = 0;
				}
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
			break;
		}
	}

	m_vecEvent.clear();
}

