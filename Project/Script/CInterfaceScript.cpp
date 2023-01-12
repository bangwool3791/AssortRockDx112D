#include "pch.h"
#include "CInterfaceScript.h"

#include <Engine\CLevel.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CInterfaceMgr.h>

#include "CButtonScript.h"

CInterfaceScript::CInterfaceScript()
	:CScript{ SCRIPT_TYPE::INTERFACESCRIPT }
	, m_id{}
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
				for (size_t i{}; i < 6; ++i)
					m_arrTapButton[i]->GetScript<CButtonScript>()->SetColumn(0);
			}
		}
		
		m_pTarget = pGameObject;

		if (!lstrcmp(L"CommandCenter", wstrName.c_str()))
		{
			for (size_t i{}; i < 6; ++i)
			{
				if (nullptr == m_arrTapButton[i])
					continue;

				if (m_arrTapButton[i]->GetScript<CButtonScript>()->IsClicked())
				{
					if (0 == m_arrTapButton[i]->GetScript<CButtonScript>()->GetColumn())
					{
						for (size_t j{}; j < 6; ++j)
							m_arrTapButton[j]->GetScript<CButtonScript>()->SetColumn(1);
					}
					else if (1 == m_arrTapButton[i]->GetScript<CButtonScript>()->GetColumn())
					{
						//°´Ã¼ ²ø¾î¿À±â
					}
				}
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
