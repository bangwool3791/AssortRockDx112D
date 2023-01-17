#include "pch.h"
#include "CWWallScript.h"

#include <Engine\CDevice.h>
#include <Engine\CLevel.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CTransform.h>

#include <Engine\CInterfaceMgr.h>
#include <Script\CMouseScript.h>

array<tWoodWall, 100 * 100> CWWallScript::m_arr{};

CWWallScript::CWWallScript()
	:CScript{ SCRIPT_TYPE::WWALLSCRIPT}
	, m_vMousePos{}
	, m_pTileObject{}
	, m_eBuildState{ BUILD_STATE::READY }
{
	SetName(L"CWWallScript");
}

CWWallScript::~CWWallScript()
{
}

void CWWallScript::begin()
{
	m_pLevelMouseObject = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"MouseObject");
	m_pTileObject = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"LevelTile");

	GetOwner()->GetRenderComponent()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"WWallMesh"));
	GetOwner()->GetRenderComponent()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"BuildMtrl"));
	GetOwner()->GetRenderComponent()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\buildings\\Atlas1_LQ.dds"));
	GetOwner()->GetRenderComponent()->SetInstancingType(INSTANCING_TYPE::NONE);

	m_pTileObject->TileMap()->On();
}

void CWWallScript::tick()
{

}

void CWWallScript::finaltick()
{

	m_fDt += DT;
	m_fDt2 += DT;

	if (BUILD_STATE::READY == m_eBuildState)
	{
		if (m_fDt > 0.15f)
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

			Vec3 vPos = m_pTileObject->GetRenderComponent()->GetMesh()->GetPosition(ray);

			tTile tTile = m_pTileObject->TileMap()->GetInfo(vPos);

			if (-1 != m_iIndex)
			{
				SetTileInfo(m_iIndex);
			}

			SetTileInfo(tTile.iIndex);

			m_iIndex = tTile.iIndex;
			GetOwner()->Transform()->SetRelativePos(tTile.vPos);

			int a = 0;

			if (IsBlocked(m_iIndex))
			{
				a = 1;
				GetOwner()->GetRenderComponent()->GetDynamicMaterial()->SetScalarParam(INT_0, &a);
			}
			else
				GetOwner()->GetRenderComponent()->GetDynamicMaterial()->SetScalarParam(INT_0, &a);

			m_fDt -= 0.25f;
		}

		if (m_fDt2 > 0.5f)
		{
			if (KEY_PRESSED(KEY::LBTN) && !IsBlocked(m_iIndex))
			{
				SetTileInfo(m_iIndex);
				m_pTileObject->TileMap()->Off();
				m_eBuildState = BUILD_STATE::BUILD;
				m_fDt = 0.f;
				m_fDt2 = 0.f;

				m_arr[m_iIndex].bChecked = true;

				//even
				if ((m_iIndex / TILEX) % 2 == 0)
				{
					if (0 <= m_iIndex + TILEX - 1 && m_iIndex + TILEX - 1 < 100 * 100)
					{
						if (Node(m_iIndex + TILEX - 1, 1))
						{
							Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"WoodWallChildPrefab");
							CGameObject* pObj{};
							Instantiate(pObj = pUIPrefab->Instantiate(), 1);

							tTile tTile1 = m_pTileObject->TileMap()->GetInfo(m_iIndex);
							tTile tTile2 = m_pTileObject->TileMap()->GetInfo(m_iIndex + TILEX - 1);
							Vec3 vPos = (tTile1.vPos + tTile2.vPos) * 0.5f;
							Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();
							pObj->Transform()->SetRelativePos(vPos);
							pObj->Transform()->SetRelativeScale(vScale);
							pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"WWallLeftSideMesh"));
							pObj->begin();
						}
					}
					if (0 <= m_iIndex + TILEX && m_iIndex + TILEX < 100 * 100)
					{
						if (Node(m_iIndex + TILEX, 2))
						{
							Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"WoodWallChildPrefab");
							CGameObject* pObj{};
							Instantiate(pObj = pUIPrefab->Instantiate(), 1);

							tTile tTile1 = m_pTileObject->TileMap()->GetInfo(m_iIndex);
							tTile tTile2 = m_pTileObject->TileMap()->GetInfo(m_iIndex + TILEX);
							Vec3 vPos = (tTile1.vPos + tTile2.vPos) * 0.5f;
							Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();
							pObj->Transform()->SetRelativePos(vPos);
							pObj->Transform()->SetRelativeScale(vScale);
							pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"WWallRightSideMesh"));
							pObj->begin();
						}
					}
					if (0 <= m_iIndex - TILEX && m_iIndex - TILEX < 100 * 100)
					{
						if (Node(m_iIndex - TILEX, 3))
						{
							Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"WoodWallChildPrefab");
							CGameObject* pObj{};
							Instantiate(pObj = pUIPrefab->Instantiate(), 1);

							tTile tTile1 = m_pTileObject->TileMap()->GetInfo(m_iIndex);
							tTile tTile2 = m_pTileObject->TileMap()->GetInfo(m_iIndex - TILEX);
							Vec3 vPos = (tTile1.vPos + tTile2.vPos) * 0.5f;
							Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();
							pObj->Transform()->SetRelativePos(vPos);
							pObj->Transform()->SetRelativeScale(vScale);
							pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"WWallLeftSideMesh"));
							pObj->begin();
						}
					}
					if (0 <= m_iIndex - TILEX - 1 && m_iIndex - TILEX - 1 < 100 * 100)
					{
						if (Node(m_iIndex - TILEX - 1, 4))
						{
							Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"WoodWallChildPrefab");
							CGameObject* pObj{};
							Instantiate(pObj = pUIPrefab->Instantiate(), 1);

							tTile tTile1 = m_pTileObject->TileMap()->GetInfo(m_iIndex);
							tTile tTile2 = m_pTileObject->TileMap()->GetInfo(m_iIndex - TILEX - 1);
							Vec3 vPos = (tTile1.vPos + tTile2.vPos) * 0.5f;
							Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();
							pObj->Transform()->SetRelativePos(vPos);
							pObj->Transform()->SetRelativeScale(vScale);
							pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"WWallRightSideMesh"));
							pObj->begin();
						}
					}

					if (0 <= m_iIndex + TILEX * 2 && m_iIndex + TILEX * 2 < 100 * 100)
					{
						if (Node(m_iIndex + TILEX * 2, 1))
						{
							Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"WoodWallChildPrefab");
							CGameObject* pObj{};
							Instantiate(pObj = pUIPrefab->Instantiate(), 1);

							tTile tTile1 = m_pTileObject->TileMap()->GetInfo(m_iIndex);
							tTile tTile2 = m_pTileObject->TileMap()->GetInfo(m_iIndex + TILEX * 2);
							Vec3 vPos = (tTile1.vPos + tTile2.vPos) * 0.5f;
							Vec3 vScale = tTile1.vPos - tTile2.vPos;
							vScale.x = GetOwner()->Transform()->GetRelativeScale().x;
							vScale.y = abs(vScale.y);
							vScale.z = abs(vScale.z);
							pObj->Transform()->SetRelativePos(vPos);
							pObj->Transform()->SetRelativeScale(vScale);
							pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"WWallHightMesh"));
							pObj->begin();
						}
					}
					if (0 <= m_iIndex + 1 && m_iIndex + 1 < 100 * 100)
					{
						if (Node(m_iIndex + 1, 2))
						{
							Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"WoodWallChildPrefab");
							CGameObject* pObj{};
							Instantiate(pObj = pUIPrefab->Instantiate(), 1);

							tTile tTile1 = m_pTileObject->TileMap()->GetInfo(m_iIndex);
							tTile tTile2 = m_pTileObject->TileMap()->GetInfo(m_iIndex + 1);
							Vec3 vPos = (tTile1.vPos + tTile2.vPos) * 0.5f;
							Vec3 vScale = tTile1.vPos - tTile2.vPos;
							vScale.x = abs(vScale.x);
							vScale.y = GetOwner()->Transform()->GetRelativeScale().y;
							vScale.z = GetOwner()->Transform()->GetRelativeScale().z;
							pObj->Transform()->SetRelativePos(vPos);
							pObj->Transform()->SetRelativeScale(vScale);
							pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"WWallWidthMesh"));
							pObj->begin();
						}
					}
					if (0 <= m_iIndex - TILEX * 2 && m_iIndex - TILEX * 2 < 100 * 100)
					{
						if (Node(m_iIndex - TILEX * 2, 3))
						{
							Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"WoodWallChildPrefab");
							CGameObject* pObj{};
							Instantiate(pObj = pUIPrefab->Instantiate(), 1);

							tTile tTile1 = m_pTileObject->TileMap()->GetInfo(m_iIndex);
							tTile tTile2 = m_pTileObject->TileMap()->GetInfo(m_iIndex - TILEX * 2);
							Vec3 vPos = (tTile1.vPos + tTile2.vPos) * 0.5f;
							Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();
							pObj->Transform()->SetRelativePos(vPos);
							pObj->Transform()->SetRelativeScale(vScale);
							pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"WWallHightMesh"));
							pObj->begin();
						}
					}
					if (0 <= m_iIndex - 1 && m_iIndex - 1 < 100 * 100)
					{
						if (Node(m_iIndex - 1, 4))
						{
							Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"WoodWallChildPrefab");
							CGameObject* pObj{};
							Instantiate(pObj = pUIPrefab->Instantiate(), 1);

							tTile tTile1 = m_pTileObject->TileMap()->GetInfo(m_iIndex);
							tTile tTile2 = m_pTileObject->TileMap()->GetInfo(m_iIndex - 1);
							Vec3 vPos = (tTile1.vPos + tTile2.vPos) * 0.5f;
							Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();
							pObj->Transform()->SetRelativePos(vPos);
							pObj->Transform()->SetRelativeScale(vScale);
							pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"WWallWidthMesh"));
							pObj->begin();
						}
					}
				}
				//odd
				else if ((m_iIndex / TILEX) % 2 == 1)
				{
					if (0 <= m_iIndex + TILEX + 1 && m_iIndex + TILEX + 1 < 100 * 100)
					{
						if (Node(m_iIndex + TILEX + 1, 1))
						{
							Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"WoodWallChildPrefab");
							CGameObject* pObj{};
							Instantiate(pObj = pUIPrefab->Instantiate(), 1);

							tTile tTile1 = m_pTileObject->TileMap()->GetInfo(m_iIndex);
							tTile tTile2 = m_pTileObject->TileMap()->GetInfo(m_iIndex + TILEX + 1);
							Vec3 vPos = (tTile1.vPos + tTile2.vPos) * 0.5f;
							Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();
							pObj->Transform()->SetRelativePos(vPos);
							pObj->Transform()->SetRelativeScale(vScale);
							pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"WWallRightSideMesh"));
							pObj->begin();
						}
					}
					if (0 <= m_iIndex + TILEX && m_iIndex + TILEX < 100 * 100)
					{
						if (Node(m_iIndex + TILEX, 2))
						{
							Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"WoodWallChildPrefab");
							CGameObject* pObj{};
							Instantiate(pObj = pUIPrefab->Instantiate(), 1);

							tTile tTile1 = m_pTileObject->TileMap()->GetInfo(m_iIndex);
							tTile tTile2 = m_pTileObject->TileMap()->GetInfo(m_iIndex + TILEX);
							Vec3 vPos = (tTile1.vPos + tTile2.vPos) * 0.5f;
							Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();
							pObj->Transform()->SetRelativePos(vPos);
							pObj->Transform()->SetRelativeScale(vScale);
							pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"WWallLeftSideMesh"));
							pObj->begin();
						}
					}
					if (0 <= m_iIndex - TILEX && m_iIndex - TILEX < 100 * 100)
					{
						if (Node(m_iIndex - TILEX, 3))
						{
							Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"WoodWallChildPrefab");
							CGameObject* pObj{};
							Instantiate(pObj = pUIPrefab->Instantiate(), 1);

							tTile tTile1 = m_pTileObject->TileMap()->GetInfo(m_iIndex);
							tTile tTile2 = m_pTileObject->TileMap()->GetInfo(m_iIndex - TILEX);
							Vec3 vPos = (tTile1.vPos + tTile2.vPos) * 0.5f;
							Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();
							pObj->Transform()->SetRelativePos(vPos);
							pObj->Transform()->SetRelativeScale(vScale);
							pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"WWallRightSideMesh"));
							pObj->begin();
						}
					}
					if (0 <= m_iIndex - TILEX + 1 && m_iIndex - TILEX + 1 < 100 * 100)
					{
						if (Node(m_iIndex - TILEX + 1, 4))
						{
							Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"WoodWallChildPrefab");
							CGameObject* pObj{};
							Instantiate(pObj = pUIPrefab->Instantiate(), 1);

							tTile tTile1 = m_pTileObject->TileMap()->GetInfo(m_iIndex);
							tTile tTile2 = m_pTileObject->TileMap()->GetInfo(m_iIndex - TILEX + 1);
							Vec3 vPos = (tTile1.vPos + tTile2.vPos) * 0.5f;
							Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();
							pObj->Transform()->SetRelativePos(vPos);
							pObj->Transform()->SetRelativeScale(vScale);
							pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"WWallLeftSideMesh"));
							pObj->begin();
						}
					}

					if (0 <= m_iIndex + TILEX * 2 && m_iIndex + TILEX * 2 < 100 * 100)
					{
						if (Node(m_iIndex + TILEX * 2, 1))
						{
							Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"WoodWallChildPrefab");
							CGameObject* pObj{};
							Instantiate(pObj = pUIPrefab->Instantiate(), 1);

							tTile tTile1 = m_pTileObject->TileMap()->GetInfo(m_iIndex);
							tTile tTile2 = m_pTileObject->TileMap()->GetInfo(m_iIndex + TILEX * 2);
							Vec3 vPos = (tTile1.vPos + tTile2.vPos) * 0.5f;
							Vec3 vScale = tTile1.vPos - tTile2.vPos;
							vScale.x = GetOwner()->Transform()->GetRelativeScale().x;
							vScale.y = abs(vScale.y);
							vScale.z = abs(vScale.z);
							pObj->Transform()->SetRelativePos(vPos);
							pObj->Transform()->SetRelativeScale(vScale);
							pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"WWallHightMesh"));
							pObj->begin();
						}
					}

					if (0 <= m_iIndex + 1 && m_iIndex + 1 < 100 * 100)
					{
						if (Node(m_iIndex + 1, 2))
						{
							Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"WoodWallChildPrefab");
							CGameObject* pObj{};
							Instantiate(pObj = pUIPrefab->Instantiate(), 1);

							tTile tTile1 = m_pTileObject->TileMap()->GetInfo(m_iIndex);
							tTile tTile2 = m_pTileObject->TileMap()->GetInfo(m_iIndex + 1);
							Vec3 vPos = (tTile1.vPos + tTile2.vPos) * 0.5f;
							Vec3 vScale = tTile1.vPos - tTile2.vPos;
							vScale.x = abs(vScale.x);
							vScale.y = GetOwner()->Transform()->GetRelativeScale().y;
							vScale.z = GetOwner()->Transform()->GetRelativeScale().z;
							pObj->Transform()->SetRelativePos(vPos);
							pObj->Transform()->SetRelativeScale(vScale);
							pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"WWallWidthMesh"));
							pObj->begin();
						}
					}
					if (0 <= m_iIndex - TILEX * 2 && m_iIndex - TILEX * 2 < 100 * 100)
					{
						if (Node(m_iIndex - TILEX * 2, 3))
						{
							Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"WoodWallChildPrefab");
							CGameObject* pObj{};
							Instantiate(pObj = pUIPrefab->Instantiate(), 1);

							tTile tTile1 = m_pTileObject->TileMap()->GetInfo(m_iIndex);
							tTile tTile2 = m_pTileObject->TileMap()->GetInfo(m_iIndex - TILEX * 2);
							Vec3 vPos = (tTile1.vPos + tTile2.vPos) * 0.5f;
							Vec3 vScale = tTile1.vPos - tTile2.vPos;
							vScale.x = GetOwner()->Transform()->GetRelativeScale().x;
							vScale.y = abs(vScale.y);
							vScale.z = abs(vScale.z);
							pObj->Transform()->SetRelativePos(vPos);
							pObj->Transform()->SetRelativeScale(vScale);
							pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"WWallHightMesh"));
							pObj->begin();
						}
					}
					if (0 <= m_iIndex - 1 && m_iIndex - 1 < 100 * 100)
					{
						if (Node(m_iIndex - 1, 4))
						{
							Ptr<CPrefab> pUIPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"WoodWallChildPrefab");
							CGameObject* pObj{};
							Instantiate(pObj = pUIPrefab->Instantiate(), 1);

							tTile tTile1 = m_pTileObject->TileMap()->GetInfo(m_iIndex);
							tTile tTile2 = m_pTileObject->TileMap()->GetInfo(m_iIndex - 1);
							Vec3 vPos = (tTile1.vPos + tTile2.vPos) * 0.5f;
							Vec3 vScale = tTile1.vPos - tTile2.vPos;
							vScale.x = abs(vScale.x);
							vScale.y = GetOwner()->Transform()->GetRelativeScale().y;
							vScale.z = GetOwner()->Transform()->GetRelativeScale().z;
							pObj->Transform()->SetRelativePos(vPos);
							pObj->Transform()->SetRelativeScale(vScale);
							pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"WWallWidthMesh"));
							pObj->begin();
						}
					}
				}
			}
			m_fDt2 = 0.5f;
		}
	}
	else if (m_eBuildState == BUILD_STATE::BUILD)
	{
		if (m_fDt > 5.f)
		{
			GetOwner()->GetRenderComponent()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ObjectMtrl"));
			GetOwner()->GetRenderComponent()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\buildings\\Atlas1_LQ.dds"));
			GetOwner()->GetRenderComponent()->SetInstancingType(INSTANCING_TYPE::USED);
			m_eBuildState = BUILD_STATE::COMPLETE;
			m_fDt = 0.f;
		}
	}
	else if (m_eBuildState == BUILD_STATE::COMPLETE)
	{
	}
}

void CWWallScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CWWallScript::Overlap(CCollider2D* _pOther)
{
}

void CWWallScript::EndOverlap(CCollider2D* _pOther)
{
}
void CWWallScript::SaveToFile(FILE* _File)
{
	CScript::SaveToFile(_File);
}

void CWWallScript::LoadFromFile(FILE* _File)
{
	CScript::LoadFromFile(_File);
}

void CWWallScript::SetTileInfo(UINT _iTile)
{
	tTile tTile = m_pTileObject->TileMap()->GetInfo(_iTile);

	if ((UINT)TILE_TYPE::EMPTY == tTile.iInfo)
	{
		m_pTileObject->TileMap()->SetInfo(_iTile, (UINT)TILE_TYPE::BUILD);
	}
	else if ((UINT)TILE_TYPE::BUILD == tTile.iInfo)
	{
		m_pTileObject->TileMap()->SetInfo(_iTile, (UINT)TILE_TYPE::EMPTY);
	}
	else if ((UINT)TILE_TYPE::USED == tTile.iInfo)
	{
		m_pTileObject->TileMap()->SetInfo(_iTile, (UINT)TILE_TYPE::COLLISION);
	}
	else if ((UINT)TILE_TYPE::COLLISION == tTile.iInfo)
	{
		m_pTileObject->TileMap()->SetInfo(_iTile, (UINT)TILE_TYPE::USED);
	}
}

bool  CWWallScript::IsBlocked(UINT _iTile)
{
	static vector<tTile> vec(1, tTile{});

	vec[0] = m_pTileObject->TileMap()->GetInfo(_iTile);

	for (size_t i{}; i < vec.size(); ++i)
	{
		if ((UINT)TILE_TYPE::BUILD != vec[i].iInfo)
			return true;
	}
	return false;
}

bool  CWWallScript::Node(UINT _iTile, UINT _iIndex)
{
	if (m_arr[_iTile].bChecked && false == (bool)(m_arr[_iTile].arr[_iIndex - 1]))
	{
		m_arr[_iTile].arr[_iIndex] = true;
		return true;
	}
	return false;
}