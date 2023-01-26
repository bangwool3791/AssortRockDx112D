#include "pch.h"
#include "CInterfaceScript.h"

#include <Engine\CDevice.h>
#include <Engine\CLevel.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CInterfaceMgr.h>

#include "CSoldierScript.h"
#include "CSCScript.h"
#include "CInfectedGiantScript.h"
#include "CButtonScript.h"

#include "CCommandScript.h"
#include "CTentScript.h"
#include "CHuntScript.h"
#include "CSawScript.h"
#include "CQuarryScript.h"
#include "CSCScript.h"
#include "CWWSScript.h"
#include "CWWCScript.h"
#include "CWWallScript.h"
#include "CPOSScript.h"

CInterfaceScript::CInterfaceScript()
	:CScript{ SCRIPT_TYPE::INTERFACESCRIPT }
	, m_id{}
	, m_pTile{}
	, m_pTarget{}
	, m_pMouseObject{}
	, m_pCameraObject{}
	, m_arrTapButton{}
{
	SetName(L"CInterfaceScript");
}

CInterfaceScript::~CInterfaceScript()
{
}


void CInterfaceScript::begin()
{
	Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Interface\\Atlas1_LQ.dds");
	float width = pTex->GetWidth();
	float height = pTex->GetHeight();
	GetOwner()->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);
	int a = 1;
	GetOwner()->GetRenderComponent()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, &a);
	Vec2 vLeftTop = { 0.f, 0.f };
	GetOwner()->GetRenderComponent()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_0, &vLeftTop);
	Vec2 vSlice = { 1350.f / width, 213.f / height };
	GetOwner()->GetRenderComponent()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_1, &vSlice);

	for (size_t i{}; i < 2; ++i)
	{
		for (size_t j{}; j < 3; ++j)
		{
			wstring strName = L"UIButton";
			strName += std::to_wstring(i * 3 + j);

			m_arrTapButton[i * 3 + j] = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(strName);
		}
	}

	m_pTile = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"LevelTile");
}

void CInterfaceScript::tick()
{
	/*
	* 
	*/

	if (CInterfaceMgr::GetInst()->GetTarget())
	{
		CGameObject* pGameObject =  CInterfaceMgr::GetInst()->GetTarget();
		wstring wstrName = pGameObject->GetName();

		if (m_pTarget)
		{
			if (lstrcmp(m_pTarget->GetName().c_str(), wstrName.c_str()))
			{
				if (!lstrcmp(L"CmdCenter", wstrName.c_str()))
				{
					m_pTile->TileMap()->On();
					for (size_t i{}; i < 6; ++i)
						m_arrTapButton[i]->GetScript<CButtonScript>()->SetColumn((UINT)COMMAND_CENTER);
				}				
				else if (!lstrcmp(L"SC", wstrName.c_str()))
				{
					m_pTile->TileMap()->Off();
					for (size_t i{}; i < 6; ++i)
						m_arrTapButton[i]->GetScript<CButtonScript>()->SetColumn((UINT)SOLDIER_CMD);
				}else
					m_pTile->TileMap()->Off();
			}
		}
		
		if (!m_pTarget)
		{
			if (!lstrcmp(L"CmdCenter", wstrName.c_str()))
			{
				m_pTile->TileMap()->On();
				for (size_t i{}; i < 6; ++i)
					m_arrTapButton[i]->GetScript<CButtonScript>()->SetColumn((UINT)COMMAND_CENTER);
			}
			else if (!lstrcmp(L"SC", wstrName.c_str()))
			{
				m_pTile->TileMap()->Off();
				for (size_t i{}; i < 6; ++i)
					m_arrTapButton[i]->GetScript<CButtonScript>()->SetColumn((UINT)SOLDIER_CMD);
			}
			else
				m_pTile->TileMap()->Off();
		}

		m_pTarget = pGameObject;

		if (!lstrcmp(L"CmdCenter", wstrName.c_str()))
		{
			for (size_t i{}; i < 6; ++i)
			{
				if (nullptr == m_arrTapButton[i])
					continue;

				if (m_arrTapButton[i]->GetScript<CButtonScript>()->IsClicked())
				{
					if (COMMAND_CENTER == m_arrTapButton[i]->GetScript<CButtonScript>()->GetColumn())
					{
						switch (i)
						{
						case 0:
							for (size_t j{}; j < 6; ++j)
								m_arrTapButton[j]->GetScript<CButtonScript>()->SetColumn(COLONISTS);
							break;
						case 1:
							for (size_t j{}; j < 6; ++j)
								m_arrTapButton[j]->GetScript<CButtonScript>()->SetColumn(RESOURCE);
							break;
						case 2:
							for (size_t j{}; j < 6; ++j)
								m_arrTapButton[j]->GetScript<CButtonScript>()->SetColumn(INDUSTRY);
							break;
						case 3:
							for (size_t j{}; j < 6; ++j)
								m_arrTapButton[j]->GetScript<CButtonScript>()->SetColumn(DEFENSE);
							break;
						}
					}
					else if (COLONISTS == m_arrTapButton[i]->GetScript<CButtonScript>()->GetColumn())
					{
						switch (i)
						{
						case 0:
						{
							Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"TentPrefab");
							CGameObject* pObj = pUIPrefab->Instantiate();
							CInterfaceMgr::GetInst()->SetBuildObj(pObj);
							Instantiate(pObj, 1);
						}
							break;
						case 5:
							for (size_t j{}; j < 6; ++j)
								m_arrTapButton[j]->GetScript<CButtonScript>()->SetColumn(COMMAND_CENTER);

							if (CInterfaceMgr::GetInst()->GetBuildObj())
							{
								CInterfaceMgr::GetInst()->GetBuildObj()->Destroy();
								CInterfaceMgr::GetInst()->SetBuildObj(nullptr);
							}
							break;
						}
					}
					else if (RESOURCE == m_arrTapButton[i]->GetScript<CButtonScript>()->GetColumn())
					{
						switch (i)
						{
						case 0:
						{
							Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"HuntHousePrefab");
							CGameObject* pObj = pUIPrefab->Instantiate();
							CInterfaceMgr::GetInst()->SetBuildObj(pObj);
							Instantiate(pObj, 1);
						}
						break;
						case 1:
						{
							Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"SawMillPrefab");
							CGameObject* pObj = pUIPrefab->Instantiate();
							CInterfaceMgr::GetInst()->SetBuildObj(pObj);
							Instantiate(pObj, 1);
						}
						break;
						case 2:
						{
							Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"QuarryPrefab");
							CGameObject* pObj = pUIPrefab->Instantiate();
							CInterfaceMgr::GetInst()->SetBuildObj(pObj);
							Instantiate(pObj, 1);
						}
						break;
						case 5:
							for (size_t j{}; j < 6; ++j)
								m_arrTapButton[j]->GetScript<CButtonScript>()->SetColumn(COMMAND_CENTER);

							if (CInterfaceMgr::GetInst()->GetBuildObj())
							{
								CInterfaceMgr::GetInst()->GetBuildObj()->Destroy();
								CInterfaceMgr::GetInst()->SetBuildObj(nullptr);
							}
							break;
						}
					}
					else if (INDUSTRY == m_arrTapButton[i]->GetScript<CButtonScript>()->GetColumn())
					{
						switch (i)
						{
						case 0:
						{
							Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"SCPrefab");
							CGameObject* pObj = pUIPrefab->Instantiate();
							CInterfaceMgr::GetInst()->SetBuildObj(pObj);
							Instantiate(pObj, 1);
						}
						break;
						case 1:
						{
							Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"POSPrefab");
							CGameObject* pObj = pUIPrefab->Instantiate();
							CInterfaceMgr::GetInst()->SetBuildObj(pObj);
							Instantiate(pObj, 1);
						}
						break;
						case 2:
						{
							Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"WoodWorkshopPrefab");
							CGameObject* pObj = pUIPrefab->Instantiate();
							CInterfaceMgr::GetInst()->SetBuildObj(pObj);
							Instantiate(pObj, 1);
						}
						break;
						case 5:
							for (size_t j{}; j < 6; ++j)
								m_arrTapButton[j]->GetScript<CButtonScript>()->SetColumn(COMMAND_CENTER);

							if (CInterfaceMgr::GetInst()->GetBuildObj())
							{
								CInterfaceMgr::GetInst()->GetBuildObj()->Destroy();
								CInterfaceMgr::GetInst()->SetBuildObj(nullptr);
							}
							break;
						}
					}
					else if (DEFENSE == m_arrTapButton[i]->GetScript<CButtonScript>()->GetColumn())
					{
						switch (i)
						{
						case 0:
						{
							Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"WoodWallPrefab");
							CGameObject* pObj = pUIPrefab->Instantiate();
							CInterfaceMgr::GetInst()->SetBuildObj(pObj);
							Instantiate(pObj, 1);
						}
						break;
						case 5:
							for (size_t j{}; j < 6; ++j)
								m_arrTapButton[j]->GetScript<CButtonScript>()->SetColumn(COMMAND_CENTER);

							if (CInterfaceMgr::GetInst()->GetBuildObj())
							{
							CInterfaceMgr::GetInst()->GetBuildObj()->Destroy();
							CInterfaceMgr::GetInst()->SetBuildObj(nullptr);
							}
							break;
						}
					}
				}
				//버튼 토글
			}
		}
		/*
		* 마우스 이동
		* UI Description 표기
		*/
		else if (!lstrcmp(L"SC", wstrName.c_str()))
		{
		for (size_t i{}; i < 6; ++i)
		{
			if (nullptr == m_arrTapButton[i])
				continue;

			if (m_arrTapButton[i]->GetScript<CButtonScript>()->IsClicked())
			{
				if (SOLDIER_CMD == m_arrTapButton[i]->GetScript<CButtonScript>()->GetColumn())
				{
					switch (i)
					{
					case 0:
						m_pTarget->GetScript<CSCScript>()->CreateUnit(L"CRangerPrefab");
						break;
					case 1:
						m_pTarget->GetScript<CSCScript>()->CreateUnit(L"SoldierPrefab");
						break;
					case 2:
						m_pTarget->GetScript<CSCScript>()->CreateUnit(L"CSniperPrefab");
						break;
						//끝
					case 3:

						break;
					}
				}
			}
			//버튼 토글
		}
		}
		else if (!lstrcmp(L"Soldier", wstrName.c_str()) || !lstrcmp(L"CInfectedGiant", wstrName.c_str()))
		{
		if (KEY_PRESSED(KEY::LBTN))
		{
			const Ray& ray = GetRay();
			Vec3 vPos{};

			if (m_pTile->TileMap()->GetMesh()->GetPosition(ray, vPos))
			{
				tTile tTile = m_pTile->TileMap()->GetInfo(vPos);
				Int32 x = tTile.iIndex % TILEX;
				Int32 y = tTile.iIndex / TILEX;

				if (!lstrcmp(L"Soldier", wstrName.c_str()))
				{
					m_pTarget->GetScript<CSoldierScript>()->Move(x, y);
				}
				else if (!lstrcmp(L"CInfectedGiant", wstrName.c_str()))
				{
					m_pTarget->GetScript<CInfectedGiantScript>()->JpsAlgorithm(x, y);
				}
			}
		}

		}
	}
	else
	{
	m_pTarget = nullptr;
	}

	//타겟 미 선택 버튼 선택 일 경우 해지
	for (size_t i{}; i < 6; ++i)
		m_arrTapButton[i]->GetScript<CButtonScript>()->Release();
}

void CInterfaceScript::finaltick()
{
	if (CInterfaceMgr::GetInst()->GetBuildObj())
	{
		CGameObject* pObj = CInterfaceMgr::GetInst()->GetBuildObj();
		if (KEY_PRESSED(KEY::RBTN))
		{
			if (!lstrcmp(pObj->GetName().data(), L"Tent"))
				pObj->GetScript<CTentScript>()->clear();
			else if (!lstrcmp(pObj->GetName().data(), L"HuntHouse"))
				pObj->GetScript<CHuntScript>()->clear();
			else if (!lstrcmp(pObj->GetName().data(), L"SawMill"))
				pObj->GetScript<CSawScript>()->clear();
			else if (!lstrcmp(pObj->GetName().data(), L"Quarry"))
				pObj->GetScript<CQuarryScript>()->clear();
			else if (!lstrcmp(pObj->GetName().data(), L"SC"))
				pObj->GetScript<CSCScript>()->clear();
			else if (!lstrcmp(pObj->GetName().data(), L"WoodWorkshop"))
				pObj->GetScript<CWWSScript>()->clear();
			else if (!lstrcmp(pObj->GetName().data(), L"POS"))
				pObj->GetScript<CPOSScript>()->clear();

			for (size_t j{}; j < 6; ++j)
				m_arrTapButton[j]->GetScript<CButtonScript>()->SetColumn(COMMAND_CENTER);

			pObj->Destroy();
			CInterfaceMgr::GetInst()->SetBuildObj(nullptr);
		}
	}

	if (KEY_PRESSED(KEY::LBTN))
	{
		static int iChecked = 0;
		float fDist{};
		const Ray& ray = GetRay();
		Vec3 vPos{};

		vector<pair<CGameObject*, float>> vecPair{};

		const vector<CGameObject*>& vecObj = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(1)->GetParentObjects();

		for (size_t i{}; i < vecObj.size(); ++i)
		{
			if (vecObj[i]->Transform()->Picking(ray, fDist))
			{
				BUILD_STATE eState{BUILD_STATE::END};

				if (!lstrcmp(vecObj[i]->GetName().data(), L"CmdCenter"))
					eState = vecObj[i]->GetScript<CCommandScript>()->GetState();
				else if (!lstrcmp(vecObj[i]->GetName().data(), L"Tent"))
					eState = vecObj[i]->GetScript<CTentScript>()->GetState();
				else if (!lstrcmp(vecObj[i]->GetName().data(), L"HuntHouse"))
					eState = vecObj[i]->GetScript<CHuntScript>()->GetState();
				else if (!lstrcmp(vecObj[i]->GetName().data(), L"SawMill"))
					eState = vecObj[i]->GetScript<CSawScript>()->GetState();
				else if (!lstrcmp(vecObj[i]->GetName().data(), L"Quarry"))
					eState = vecObj[i]->GetScript<CQuarryScript>()->GetState();
				else if (!lstrcmp(vecObj[i]->GetName().data(), L"SC"))
					eState = vecObj[i]->GetScript<CSCScript>()->GetState();
				else if (!lstrcmp(vecObj[i]->GetName().data(), L"WoodWorkshop"))
					eState = vecObj[i]->GetScript<CWWSScript>()->GetState();
				else if (!lstrcmp(vecObj[i]->GetName().data(), L"POS"))
					eState = vecObj[i]->GetScript<CPOSScript>()->GetState();

				if ((BUILD_STATE::COMPLETE == eState) || (BUILD_STATE::CREATE_UNIT == eState))
				{
					vecPair.push_back(make_pair(vecObj[i], fDist));
				}
			}
		}

		const vector<CGameObject*>& vecUnit = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(1)->GetParentObjects();

		for (size_t i{}; i < vecUnit.size(); ++i)
		{
			if (!lstrcmp(vecObj[i]->GetName().data(), L"CmdCenter"))
				continue;
			else if (!lstrcmp(vecObj[i]->GetName().data(), L"Tent"))
				continue;
			else if (!lstrcmp(vecObj[i]->GetName().data(), L"HuntHouse"))
				continue;
			else if (!lstrcmp(vecObj[i]->GetName().data(), L"SawMill"))
				continue;
			else if (!lstrcmp(vecObj[i]->GetName().data(), L"Quarry"))
				continue;
			else if (!lstrcmp(vecObj[i]->GetName().data(), L"SC"))
				continue;
			else if (!lstrcmp(vecObj[i]->GetName().data(), L"WoodWorkshop"))
				continue;
			else if (!lstrcmp(vecObj[i]->GetName().data(), L"POS"))
				continue;

			if (vecUnit[i]->Transform()->Picking(ray, fDist))
				vecPair.push_back(make_pair(vecUnit[i], fDist));
		}

		sort(vecPair.begin(), vecPair.end(), [&](pair<CGameObject*, float> lhs, pair<CGameObject*, float> rhs)
		{
			if (lhs.second < rhs.second)
				return true;
			else
				return false;
		});

		if(!vecPair.empty())
			CInterfaceMgr::GetInst()->SetTarget(vecPair[0].first);
	}
}

void CInterfaceScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CInterfaceScript::Overlap(CCollider2D* _pOther)
{
}

void CInterfaceScript::EndOverlap(CCollider2D* _pOther)
{
}
