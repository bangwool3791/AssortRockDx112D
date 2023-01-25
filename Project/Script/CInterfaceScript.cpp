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
					for (size_t i{}; i < 6; ++i)
						m_arrTapButton[i]->GetScript<CButtonScript>()->SetColumn((UINT)COMMAND_CENTER);
				}				
				else if (!lstrcmp(L"SC", wstrName.c_str()))
				{
					for (size_t i{}; i < 6; ++i)
						m_arrTapButton[i]->GetScript<CButtonScript>()->SetColumn((UINT)SOLDIER_CMD);
				}
			}
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
							Instantiate(pObj, 0);
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
							Instantiate(pObj, 0);
						}
						break;
						case 1:
						{
							Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"SawMillPrefab");
							CGameObject* pObj = pUIPrefab->Instantiate();
							CInterfaceMgr::GetInst()->SetBuildObj(pObj);
							Instantiate(pObj, 0);
						}
						break;
						case 2:
						{
							Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"QuarryPrefab");
							CGameObject* pObj = pUIPrefab->Instantiate();
							CInterfaceMgr::GetInst()->SetBuildObj(pObj);
							Instantiate(pObj, 0);
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
							Instantiate(pObj, 0);
						}
						break;
						case 1:
						{
							Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"POSPrefab");
							CGameObject* pObj = pUIPrefab->Instantiate();
							CInterfaceMgr::GetInst()->SetBuildObj(pObj);
							Instantiate(pObj, 0);
						}
						break;
						case 2:
						{
							Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"WoodWorkshopPrefab");
							CGameObject* pObj = pUIPrefab->Instantiate();
							CInterfaceMgr::GetInst()->SetBuildObj(pObj);
							Instantiate(pObj, 0);
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
							Instantiate(pObj, 0);
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
				Vec2 p = CKeyMgr::GetInst()->GetMousePos();
				Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();

				p.x = (2.0f * p.x) / vResolution.x - 1.0f;
				p.y = 1.0f - (2.0f * p.y) / vResolution.y;

				XMVECTOR det; //Determinant, needed for matrix inverse function call
				Vector3 origin = Vector3(p.x, p.y, 0);
				Vector3 faraway = Vector3(p.x, p.y, 1);

				XMMATRIX invViewProj = XMMatrixInverse(&det, g_transform.matView * g_transform.matProj);
				Vector3 rayorigin = XMVector3Transform(origin, invViewProj);
				Vector3 rayend = XMVector3Transform(faraway, invViewProj);
				Vector3 raydirection = rayend - rayorigin;
				raydirection.Normalize();
				Ray ray;
				ray.position = rayorigin;
				ray.direction = raydirection;
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

void CInterfaceScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CInterfaceScript::Overlap(CCollider2D* _pOther)
{
}

void CInterfaceScript::EndOverlap(CCollider2D* _pOther)
{
}
