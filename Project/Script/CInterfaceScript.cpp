#include "pch.h"
#include "CInterfaceScript.h"

#include <Engine\CEngine.h>
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
#include "CCircleArrowScript.h"

CInterfaceScript::CInterfaceScript()
	:CScript{ SCRIPT_TYPE::INTERFACESCRIPT }
	, m_id{}
	, m_pTile{}
	, m_pTarget{}
	, m_pMouseObject{}
	, m_pCameraObject{}
	, m_arrTapButton{}
	, m_bActiveMouse{false}
{
	SetName(L"CInterfaceScript");

	m_pCameraObject = CLevelMgr::GetInst()->GetCurLevel()->FindParentObjectByName(L"MainCamera");
	m_pDragObj = CResMgr::GetInst()->FindRes<CPrefab>(L"CDragObjectPrefab")->Instantiate();

	Instantiate(m_pDragObj, 0);
	
	m_pRallyPoint = CResMgr::GetInst()->FindRes<CPrefab>(L"CRallypointPrefab")->Instantiate();
	m_pRallyPoint->GetRenderComponent()->Deactivate();
	Instantiate(m_pRallyPoint, 0);
}

CInterfaceScript::~CInterfaceScript()
{
}


void CInterfaceScript::begin()
{
	Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Interface\\Atlas1_LQ.dds");
	float width = (float)pTex->GetWidth();
	float height = (float)pTex->GetHeight();
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

	m_pGoldBar = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"CProgressGold");
	m_pWoodBar = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"CProgressWood");
	m_pIronBar = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"CProgressIron");

}

void CInterfaceScript::tick()
{
	static bool bCheck = false;

	if (!bCheck)
	{
		const vector<CGameObject*> vec = GetOwner()->GetChilds();

		for (size_t i{}; i < vec.size(); ++i)
		{
			if (vec[i]->GetName().find(L"CDesc") != std::wstring::npos)
			{
				if (vec[i]->GetName().find(L"Detail") == std::wstring::npos)
				{
					vec[i]->GetRenderComponent()->Activate();
				}
			}
		}
		bCheck = true;
	}
	Resource();

	if (m_pTarget && m_pTarget->IsDead())
	{
		CInterfaceMgr::GetInst()->SetTarget(nullptr);
		m_pTarget = nullptr;
	}
	
	if (!m_vecDragObj.empty())
	{
		for (auto iter{ m_vecDragObj.begin() }; iter != m_vecDragObj.end();)
		{
			if ((*iter)->IsDead())
			{
				iter = m_vecDragObj.erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}

	if (CInterfaceMgr::GetInst()->GetBuildObj())
	{
		CGameObject* pObj = CInterfaceMgr::GetInst()->GetBuildObj();

		m_bChecked = false;

		if (KEY_PRESSED(KEY::RBTN))
		{
			pObj->GetScripts()[0]->clear();

			for (size_t j{}; j < 6; ++j)
				m_arrTapButton[j]->GetScript<CButtonScript>()->SetColumn(COMMAND_CENTER);

			m_bChecked = true;

			pObj->Destroy();
			CInterfaceMgr::GetInst()->SetBuildObj(nullptr);
		}
	}
	else
	{
		if (m_bUiClicked)
		{
		}
		else if (KEY_PRESSED(KEY::LBTN))
		{
			const Ray& ray = GetRay();
			Vec3 vPos{};

			if (m_pTile->TileMap()->GetMesh()->GetPosition(ray, vPos))
			{
				if (m_bActiveMouse)
				{
					Vec3 vPos2 = (m_pairPoint.first + vPos) * 0.5f;
					Vec3 vScale = Vec3(fabsf(m_pairPoint.first.x - vPos.x), 
						fabsf(m_pairPoint.first.y - vPos.y),
						fabsf(m_pairPoint.first.z - vPos.z));
					m_pDragObj->Transform()->SetRelativePos(vPos2);
					m_pDragObj->Transform()->SetRelativeScale(vScale);
				}

				if (!m_bActiveMouse)
				{
					m_pDragObj->GetRenderComponent()->Activate();
					m_pairPoint.first = vPos;
					m_bActiveMouse = true;

					for (size_t i{}; i < m_vecDragObj.size(); ++i)
						m_vecDragObj[i]->GetChilds()[0]->GetRenderComponent()->Deactivate();

					m_vecDragObj.clear();
					m_vecDragObj.shrink_to_fit();
				}
			}
		}
		else if (KEY_RELEASE(KEY::LBTN))
		{
			const Ray& ray = GetRay();
			Vec3 vPos{};

			if (m_pTile->TileMap()->GetMesh()->GetPosition(ray, vPos))
			{
				if (m_bActiveMouse)
				{
					m_pDragObj->GetRenderComponent()->Deactivate();
					Vec3 vScale{ 0.f, 0.f, 0.f };
					m_pDragObj->Transform()->SetRelativeScale(vScale);
					m_bActiveDrag = true;
					m_pairPoint.second = vPos;
					m_bActiveMouse = false;
				}
			}
			else
			{
				if (m_bActiveMouse)
				{
					m_pDragObj->GetRenderComponent()->Deactivate();
					Vec3 vScale{ 0.f, 0.f, 0.f };
					m_pDragObj->Transform()->SetRelativeScale(vScale);
					m_bActiveDrag = false;
					m_bActiveMouse = false;
				}
			}
		}

	}
}

void CInterfaceScript::finaltick()
{
	if (m_bActiveDrag)
	{
		const vector<CGameObject*>& vecObj = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(1)->GetParentObjects();

		Vec3 vPos{};

		for (size_t i{}; i < vecObj.size(); ++i)
		{
			const wstring& wstrName = vecObj[i]->GetName();
			if (!lstrcmp(L"CTitan", wstrName.c_str()) || !lstrcmp(L"Soldier", wstrName.c_str()) || !lstrcmp(L"CSniper", wstrName.c_str()) || !lstrcmp(L"CRanger", wstrName.c_str()))
			{
				vPos = vecObj[i]->Transform()->GetRelativePos();

				if ((m_pairPoint.first.x <= vPos.x
					&& vPos.x <= m_pairPoint.second.x
					&& vPos.z <= m_pairPoint.first.z
					&& m_pairPoint.second.z <= vPos.z))
				{
					vecObj[i]->GetScripts()[0]->ActiveIcon();
					m_vecDragObj.push_back(vecObj[i]);
				}
				else if ((m_pairPoint.second.x <= vPos.x
					&& vPos.x <= m_pairPoint.first.x
					&& vPos.z <= m_pairPoint.second.z
					&& m_pairPoint.first.z <= vPos.z))
				{
					vecObj[i]->GetScripts()[0]->ActiveIcon();
					m_vecDragObj.push_back(vecObj[i]);
				}
			}
		}
		if (!m_vecDragObj.empty())
		{
			if (m_pTarget)
			{
				//CSelectCirclePrefab
				for (size_t i{}; i < m_pTarget->GetChilds().size(); ++i)
				{
					const wstring& str = m_pTarget->GetChilds()[i]->GetName();

					if (str == L"CSelectCircle")
					{
						m_pTarget->GetChilds()[i]->GetRenderComponent()->Deactivate();
						break;
					}
				}
			}

			for (size_t i{}; i < m_vecDragObj.size(); ++i)
				m_vecDragObj[i]->GetChilds()[0]->GetRenderComponent()->Activate();
		}
		m_bActiveDrag = false;
	}
	else if (!m_vecDragObj.empty())
	{

		if (KEY_TAP(KEY::RBTN))
		{
			const Ray& ray = GetRay();
			Vec3 vPos{};

			if (m_pTile->TileMap()->GetMesh()->GetPosition(ray, vPos))
			{
				tTile tTile = m_pTile->TileMap()->GetInfo(vPos);
				Int32 x = tTile.iIndex % TILEX;
				Int32 y = tTile.iIndex / TILEX;

				for (size_t i{}; i < m_vecDragObj.size(); ++i)
				{
					m_vecDragObj[i]->GetScripts()[0]->Move(x, y);
				}
				m_pRallyPoint->Transform()->SetRelativePos(vPos);
				m_pRallyPoint->GetRenderComponent()->Activate();
			}
		}
	}
	 else if (CInterfaceMgr::GetInst()->GetTarget())
	{
		static wstring wstrName{};

		if (m_pTarget != CInterfaceMgr::GetInst()->GetTarget() && m_bChecked)
		{
			CGameObject* pObj = m_pTarget;

			if (pObj)
				pObj->GetScripts()[0]->PhaseEventOff();

			m_pTarget = CInterfaceMgr::GetInst()->GetTarget();
			m_pTarget->GetScripts()[0]->PhaseEventOn();

			wstrName = m_pTarget->GetName();
		}

		if (!lstrcmp(L"CmdCenter", wstrName.c_str()))
		{
			for (size_t i{}; i < 6; ++i)
			{
				if (nullptr == m_arrTapButton[i])
					continue;

				if (m_arrTapButton[i]->GetScript<CButtonScript>()->IsClicked())
				{
					m_bUiClicked = true;

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
							Create(L"TentPrefab");
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
							Create(L"HuntHousePrefab");
						}
						break;
						case 1:
						{
							Create(L"SawMillPrefab");
						}
						break;
						case 2:
						{
							Create(L"QuarryPrefab");
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
							Create(L"SCPrefab");
						}
						break;
						case 1:
						{
							Create(L"POSPrefab");
						}
						break;
						case 2:
						{
							Create(L"WoodWorkshopPrefab");
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
							Create(L"WoodWallPrefab");
						}
						break;
						case 1:
						{
							Create(L"CTeslaTowerPrefab");
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
		else if (!lstrcmp(L"Tent", wstrName.c_str()))
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
							//Upgrade
							break;;
						case 6:
							m_pTarget->Destroy();
							break;
							//끝
						}
					}
				}
				//버튼 토글
			}
		}
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
							/*
							* SCScript 랠리 포인트 객체 생성
							* 이벤트 발생 시 해당 위치로 이동
							*/
							m_pTarget->GetScript<CSCScript>()->RallyEvent();
							break;
						}
					}
				}
				//버튼 토글
			}
		}
		else if (!lstrcmp(L"CTitan", wstrName.c_str()) || !lstrcmp(L"Soldier", wstrName.c_str()) || !lstrcmp(L"CSniper", wstrName.c_str()) || !lstrcmp(L"CRanger", wstrName.c_str()))
		{
			if (KEY_TAP(KEY::RBTN))
			{
				const Ray& ray = GetRay();
				Vec3 vPos{};

				if (m_pTile->TileMap()->GetMesh()->GetPosition(ray, vPos))
				{
					tTile tTile = m_pTile->TileMap()->GetInfo(vPos);
					Int32 x = tTile.iIndex % TILEX;
					Int32 y = tTile.iIndex / TILEX;

					m_pTarget->GetScripts()[0]->Move(x, y);
					m_pRallyPoint->Transform()->SetRelativePos(vPos);
					m_pRallyPoint->GetRenderComponent()->Activate();
				}
			}

		}
	}
	else
	{
		m_pTarget = nullptr;
	}

	if (m_bUiClicked)
	{
		m_bUiClicked = false;
	}
	else if (KEY_TAP(KEY::LBTN))
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
				vecPair.push_back(make_pair(vecObj[i], fDist));
			}
		}

		const vector<CGameObject*>& vecUnit = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(5)->GetParentObjects();

		for (size_t i{}; i < vecUnit.size(); ++i)
		{
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

		bool bClick = true;
		for (size_t i{}; i < 6; ++i)
		{
			if (m_arrTapButton[i]->GetScript<CButtonScript>()->IsClicked())
			{
				bClick = false;
			}
		}

		if (!vecPair.empty() && bClick)
		{
			CInterfaceMgr::GetInst()->SetTarget(vecPair[0].first);

			for (size_t i{}; i < m_vecDragObj.size(); ++i)
				m_vecDragObj[i]->GetChilds()[0]->GetRenderComponent()->Deactivate();

			m_vecDragObj.clear();
			m_vecDragObj.shrink_to_fit();

			m_bChecked = true;
		}
	}

	////타겟 미 선택 버튼 선택 일 경우 해지
	for (size_t i{}; i < 6; ++i)
	{
		if (m_arrTapButton[i]->GetScript<CButtonScript>()->IsClicked())
		{
			if (m_bActiveMouse)
			{
				m_pDragObj->GetRenderComponent()->Deactivate();
				Vec3 vScale{ 0.f, 0.f, 0.f };
				m_pDragObj->Transform()->SetRelativeScale(vScale);
				m_bActiveDrag = false;
				m_bActiveMouse = false;
			}
		}
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

void CInterfaceScript::Resource()
{
	m_fDeltaTime += DT;

	if (g_iGold > m_iGoldTotal)
		g_iGold = m_iGoldTotal;

	if (g_iWood > m_iWoodTotal)
		g_iWood = m_iWoodTotal;

	if (g_iIron > m_iIronTotal)
		g_iIron = m_iIronTotal;


	if (m_fDeltaTime >= 10.f)
	{
		if (g_iGold < m_iGoldTotal)
			g_iGold += g_iGoldInc;
		else
			g_iGold = m_iGoldTotal;

		if (g_iWood < m_iWoodTotal)
			g_iWood += g_iWoodInc;
		else
			g_iWood = m_iWoodTotal;

		if (g_iIron < m_iIronTotal)
			g_iIron += g_iIronInc;
		else
			g_iIron = m_iIronTotal;

		m_fDeltaTime = 0;
	}

	CEngine::g_vecUiText.clear();
	CEngine::g_vecUiText.shrink_to_fit();

	tTextInfo tInfo{};

	lstrcat(tInfo.sz, std::to_wstring(g_iColony).c_str());
	tInfo.vPos = Vec2{ 1392.f, 813.f };
	tInfo.vColor = Vec4{ 20.f, 200.f, 20.f, 200.f };
	tInfo.fSize = 14.f;
	CEngine::g_vecUiText.push_back(tInfo);

	memset(&tInfo, 0, sizeof(tTextInfo));

	lstrcat(tInfo.sz, std::to_wstring(g_iWorker).c_str());
	tInfo.vPos = Vec2{ 1392.f, 813.f + 29.f };
	tInfo.vColor = Vec4{ 20.f, 200.f, 20.f, 200.f };
	tInfo.fSize = 14.f;
	CEngine::g_vecUiText.push_back(tInfo);

	memset(&tInfo, 0, sizeof(tTextInfo));

	lstrcat(tInfo.sz, std::to_wstring(g_iFood).c_str());
	tInfo.vPos = Vec2{ 1392.f, 813.f + 29.f * 2.f };
	tInfo.vColor = Vec4{ 20.f, 200.f, 20.f, 200.f };
	tInfo.fSize = 14.f;
	CEngine::g_vecUiText.push_back(tInfo);

	BarText();

	IncText();
	//Progress bar

	ProgressBar(g_iGold, m_iGoldTotal, m_pGoldBar);
	ProgressBar(g_iIron, m_iIronTotal, m_pIronBar);
	ProgressBar(g_iWood, m_iWoodTotal, m_pWoodBar);
}

void CInterfaceScript::ProgressBar(int _iValue, int iTotal, CGameObject* pProObj)
{
	float x{}, fCX{};
	CGameObject* pObj{};
	Vec3 vScale{};

	pObj = pProObj->GetChilds()[0];
	vScale = pObj->Transform()->GetRelativeScale();

	x = (float)_iValue / iTotal * pProObj->Transform()->GetRelativeScale().x;

	vScale.x = x;

	fCX = pProObj->Transform()->GetRelativeScale().x;

	pObj->Transform()->SetRelativePos(Vec3(-0.5f * fCX + x * 0.5f, 0.f, 0.f));
	pObj->Transform()->SetRelativeScale(vScale);
}

void CInterfaceScript::BarText()
{
	tTextInfo tInfo{};

	lstrcat(tInfo.sz, std::to_wstring(g_iGold).c_str());
	tInfo.vPos = Vec2{ 1592.f - 100.f, 813.f - 2.f };
	tInfo.vColor = Vec4{ 255.f, 255.f, 255.f, 255.f };
	tInfo.fSize = 15.f;
	CEngine::g_vecUiText.push_back(tInfo);

	memset(&tInfo, 0, sizeof(tTextInfo));

	lstrcat(tInfo.sz, std::to_wstring(g_iIron).c_str());
	tInfo.vPos = Vec2{ 1592.f - 100.f, 813.f + 29.f - 2.f };
	tInfo.vColor = Vec4{ 255.f, 255.f, 255.f, 255.f };
	tInfo.fSize = 15.f;
	CEngine::g_vecUiText.push_back(tInfo);

	memset(&tInfo, 0, sizeof(tTextInfo));

	lstrcat(tInfo.sz, std::to_wstring(g_iWood).c_str());
	tInfo.vPos = Vec2{ 1592.f - 100.f, 813.f + 29.f * 2.f - 2.f };
	tInfo.vColor = Vec4{ 255.f, 255.f, 255.f, 255.f };
	tInfo.fSize = 15.f;
	CEngine::g_vecUiText.push_back(tInfo);
}

void CInterfaceScript::IncText()
{
	tTextInfo tInfo{};

	lstrcpy(tInfo.sz, L"+");
	lstrcat(tInfo.sz, std::to_wstring(g_iGoldInc).c_str());
	tInfo.vPos = Vec2{ 1550.f, 813.f - 2.f };
	tInfo.vColor = Vec4{ 0.f, 180.f, 0.f, 255.f};
	tInfo.fSize = 14.f;
	CEngine::g_vecUiText.push_back(tInfo);

	memset(&tInfo, 0, sizeof(tTextInfo));

	lstrcpy(tInfo.sz, L"+");
	lstrcat(tInfo.sz, std::to_wstring(g_iIronInc).c_str());
	tInfo.vPos = Vec2{ 1550.f, 813.f + 29.f - 2.f };
	tInfo.vColor = Vec4{ 0.f, 180.f, 0.f, 255.f };
	tInfo.fSize = 14.f;
	CEngine::g_vecUiText.push_back(tInfo);

	memset(&tInfo, 0, sizeof(tTextInfo));

	lstrcpy(tInfo.sz, L"+");
	lstrcat(tInfo.sz, std::to_wstring(g_iWoodInc).c_str());
	tInfo.vPos = Vec2{ 1550.f, 813.f + 29.f * 2.f - 2.f };
	tInfo.vColor = Vec4{ 0.f, 180.f, 0.f, 255.f };
	tInfo.fSize = 14.f;
	CEngine::g_vecUiText.push_back(tInfo);
}

bool CInterfaceScript::Create(const wstring& _str)
{
	if (!__super::Create(_str))
	{
		for (size_t j{}; j < 6; ++j)
			m_arrTapButton[j]->GetScript<CButtonScript>()->SetColumn(COMMAND_CENTER);
	}
	return true;
}