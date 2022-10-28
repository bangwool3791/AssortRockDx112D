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
			vector<CGameObject*> vecChilds = pGameObeject->GetChilds();
			vecChilds.push_back(pGameObeject);
		}
			break;
		case EVENT_TYPE::DELETE_OBJECT:
		{
			static queue<CGameObject*> que;

			que.push((CGameObject*)iter->wParam);

			if (!iter->wParam)
			{
				while (!que.empty())
				{
					CGameObject* pObj = (CGameObject*)que.front();
					que.pop();
					vector<CGameObject*> vecChild = pObj->GetChilds();

					for (auto iter{ vecChild.begin() }; iter != vecChild.end(); ++iter)
					{
						que.push(*iter);
					}

					pObj->SetDead(true);
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

