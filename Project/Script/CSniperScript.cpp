#include "pch.h"
#include "CSniperScript.h"

#include <Engine\CDevice.h>
#include <Engine\CLayer.h>
#include <Engine\CLevel.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CCollisionMgr.h>
#include <Engine\CScript.h>
#include <Engine\CCollider2D.h>
#include <Engine\CTransform.h>
#include <Engine\CMaterial.h>
#include <Engine\CMeshRender.h>
#include <Engine\CJpsMgr.h>

#include <Engine\CInterfaceMgr.h>
#include <Engine\CScript.h>
#include <Script\CInfectedGiantScript.h>
#include <Script\CInfectedMedium_A.h>
#include <Script\CInfectedMedium_B.h>
#include <Script\CInfectedStrong_A.h>
#include <Script\CInfectedVenom.h>

#include <Script\\CButtonScript.h>

CSniperScript::CSniperScript()
	:CScript{ SNIPERSCRIPT }
	, m_fSpeed{ 100.f }
	, m_eState{ UNIT_STATE::NORMAL }
{
	m_fHP = 100;
	m_fFullHp = 100;
	m_iAttack = 15;

	m_iArmor = 1;
	m_iSpeed = 4;
	m_iAttackSpeed = 4;
	m_iAttackRange = 6;
	m_iGold = 1;
	m_iWorker = 1;


	//Desc
	Ptr<CPrefab> prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"CImageSniperPrefab");

	m_pPortrait = prefab->Instantiate();
	m_pPortrait->Transform()->SetRelativePos(-220.f, 0.f, -550.f);
	m_pPortrait->MeshRender()->Deactivate();
	Instantiate(m_pPortrait, 31);

	prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"CDescAttackDamagePrefab");
	m_vecIcon.push_back(prefab->Instantiate());
	prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"CDescArmorPrefab");
	m_vecIcon.push_back(prefab->Instantiate());
	prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"CDescSpeedPrefab");
	m_vecIcon.push_back(prefab->Instantiate());
	prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"CDescAttackSpeedPrefab");
	m_vecIcon.push_back(prefab->Instantiate());
	prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"CDescAttackSpeedPrefab");
	m_vecIcon.push_back(prefab->Instantiate());
	prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"CDescAttackRangePrefab");
	m_vecIcon.push_back(prefab->Instantiate());
	prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"CDescGoldPrefab");
	m_vecIcon.push_back(prefab->Instantiate());
	prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"CDescWorkerPrefab");
	m_vecIcon.push_back(prefab->Instantiate());

	m_vecIcon[0]->Transform()->SetRelativePos(50.f, 0.f, -500.f);
	m_vecIcon[1]->Transform()->SetRelativePos(120.f, 0.f, -500.f);
	m_vecIcon[2]->Transform()->SetRelativePos(190.f, 0.f, -500.f);
	m_vecIcon[3]->Transform()->SetRelativePos(260.f, 0.f, -500.f);
	m_vecIcon[4]->Transform()->SetRelativePos(50.f, 0.f, -580.f);
	m_vecIcon[5]->Transform()->SetRelativePos(120.f, 0.f, -580.f);
	m_vecIcon[6]->Transform()->SetRelativePos(190.f, 0.f, -580.f);
	m_vecIcon[7]->Transform()->SetRelativePos(260.f, 0.f, -580.f);

	for (size_t i{}; i < m_vecIcon.size(); ++i)
		Instantiate(m_vecIcon[i], 31);

	for (size_t i{}; i < m_vecIcon.size(); ++i)
		m_vecIcon[i]->MeshRender()->Deactivate();

	SetName(L"CSniperScript");
}

CSniperScript::~CSniperScript()
{
}


void CSniperScript::begin()
{
	CAnimator2D* Animator = GetOwner()->Animator2D();

	if (nullptr == Animator)
		GetOwner()->AddComponent(new CAnimator2D());

	Animator = GetOwner()->Animator2D();

	Animator->ActiveRepeat();
	Animator->CloneAnimation(L"Sniper", *Animator);

	m_vSource = Transform()->GetRelativePos();

	m_vSource = Transform()->GetRelativePos();
	m_vSource.x += 1.f;
	m_vSource.z -= 1.f;
	SetDestPos(m_vSource);

	GetOwner()->GetChilds()[0]->GetRenderComponent()->Deactivate();
}

#define OFFSET 22.5f

void CSniperScript::tick()
{
	__super::tick();

	if (0 > m_fHP)
		m_eState = UNIT_STATE::DEAD;

	if (m_pTargetObject && m_pTargetObject->IsDead())
		m_pTargetObject = nullptr;

	m_fDeltaTime += DT;

	if (UNIT_STATE::NORMAL == m_eState)
	{
		SetDestPos(m_vDest);
		ProcessEnemy();
	}
	else if (UNIT_STATE::RUN == m_eState)
	{
		//적을 쫒는 Run과 마우스 이동 Run 구분
		//밖에서 알고리즘 돌리면 타겟 nullptr
		Vec3 vDir{};

		auto iter = m_vecJps.begin();

		if (iter == m_vecJps.end())
		{
			m_eState = UNIT_STATE::NORMAL;
			return;
		}

		Vec3 vDest = *iter;
		vDir = *iter - Transform()->GetRelativePos();
		vDir.Normalize();
		m_vDest = DT * vDir * m_fSpeed + Transform()->GetRelativePos();

		Transform()->SetRelativePos(m_vDest);
		SetDestPos(vDest);
		m_vSource = Transform()->GetRelativePos();

		Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();

		if (m_vSource.Distance(m_vSource, *iter) < vScale.Length() * 0.5f)
			m_vecJps.erase(iter);

		if (m_vecJps.empty())
		{
			m_bActiveJps = false;

			if (m_pTargetObject)
				ChaseEnemy();
			else
			{
				m_eState = UNIT_STATE::NORMAL;
				SetDestPos(vDest);
			}
		}

		if (m_bAttack)
			ProcessEnemy();
	}
	else if (UNIT_STATE::ATTACK == m_eState)
	{

		if (!m_pTargetObject)
		{
			if (m_bActiveJps)
				m_eState = UNIT_STATE::RUN;
			else
			{
				m_eState = UNIT_STATE::NORMAL;
				SetDestPos(m_vDest);
				m_bAttack = false;
			}

			m_pTargetObject = nullptr;
		}

		if (m_fDeltaTime >= 1.5f)
		{
			//피깍기
			if (m_pTargetObject)
			{
				SetMonsterHP();
			}
			m_fDeltaTime -= 1.5f;
		}

		if (m_pTargetObject)
		{
			Vec3 vDest = m_pTargetObject->Transform()->GetRelativePos();
			SetDestPos(vDest);
		}

		ChaseEnemy();
	}
	else if (UNIT_STATE::LOADATTACK == m_eState)
	{
		if (m_fDeltaTime >= 1.5f)
		{
			if (m_pTargetObject)
			{
				Vec3 vPos = m_pTargetObject->Transform()->GetRelativePos();
				m_eState = UNIT_STATE::ATTACK;
				SetDestPos(vPos);
			}
			m_fDeltaTime -= 1.5f;
		}

	}
	else if (UNIT_STATE::DEAD == m_eState)
	{
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += 1;
		vPos.z += 1;

		SetDestPos(vPos);

		if (Animator2D()->IsEnd())
		{
			Destroy();
		}
	}


	//Set Animation according to radian

}

void CSniperScript::SetDestPos(Vec3 _vPos)
{
	m_vTarget = _vPos;
	/*
	* 방향 벡터
	* x축 기저 벡터
	* 내적
	*/

	Vec3 vDir = m_vTarget - GetOwner()->Transform()->GetRelativePos();
	vDir = vDir.Normalize();

	Vec3 vAxis{ 1.f, 0.f, 0.f };

	if (0 <= vAxis.Dot(vDir))
	{
		GetOwner()->Transform()->SetRelativeRotationX(XM_PI * -0.25f);
		GetOwner()->Transform()->SetRelativeRotationY(0.f);
		GetOwner()->Transform()->SetRelativeRotationZ(0.f);
	}
	else if (0 > vAxis.Dot(vDir))
	{
		GetOwner()->Transform()->SetRelativeRotationX(XM_PI * 0.25f);
		GetOwner()->Transform()->SetRelativeRotationY(0.f);
		GetOwner()->Transform()->SetRelativeRotationZ(XM_PI);
	}

	float fAngle = acosf(vAxis.Dot(vDir));

	if (m_vTarget.z < GetOwner()->Transform()->GetRelativePos().z)
		fAngle = 2 * XM_PI - fAngle;

	if (XM_PI * 0.5f <= fAngle && XM_PI > fAngle)
		fAngle = XM_PI - fAngle;
	//3사분면
	if (XM_PI <= fAngle && XM_PI * 1.5f > fAngle)
	{
		fAngle = fAngle - XM_PI;
		fAngle = 2.f * XM_PI - fAngle;
	}

	fAngle = fAngle / XM_PI * 180;

	wstring str{};

	if (UNIT_STATE::NORMAL == m_eState)
		str = L"Sniper_Normal";
	else if (UNIT_STATE::LOADATTACK == m_eState)
		str = L"Sniper_LoadAttack";
	else if (UNIT_STATE::ATTACK == m_eState)
		str = L"Sniper_Attack";
	else if (UNIT_STATE::RUN == m_eState)
		str = L"Sniper_Run";
	else if (UNIT_STATE::DEAD == m_eState)
	{
		str = L"Sniper_Die_000";
		if (m_strBackUp != str)
			GetOwner()->Animator2D()->Play(str);

		m_strBackUp = str;
		return;
	}

	if (360.f <= fAngle)
		fAngle -= 360.f;

	if (fAngle <= 90 + OFFSET && 90 - OFFSET < fAngle)
	{
		str += L"_234";
	}
	else if (fAngle <= 67.5 + OFFSET && 67.5 - OFFSET < fAngle)
	{
		str += L"_252";
	}
	else if (fAngle <= 45.f + OFFSET && 45.f - OFFSET < fAngle)
	{
		str += L"_270";
	}
	else if (fAngle <= 22.5f + OFFSET && 22.5f - OFFSET < fAngle)
	{
		str += L"_288";
	}
	else if (fAngle <= 0.f + OFFSET && 0.f - OFFSET < fAngle)
	{
		str += L"_306";
	}
	else if (fAngle <= 360.f - 22.5f + OFFSET && 360.f - 22.5f - OFFSET < fAngle)
	{
		str += L"_000";
	}
	else if (fAngle <= 360.f - 45.f + OFFSET && 360.f - 45.f - OFFSET < fAngle)
	{
		str += L"_018";
	}
	else if (fAngle <= 360.f - 67.5f + OFFSET && 360.f - 67.5f - OFFSET < fAngle)
		str += L"_036";
	else if (fAngle <= 360.f - 90.f + OFFSET && 360.f - 90.f - OFFSET < fAngle)
		str += L"_036";

	if (m_strBackUp != str)
		GetOwner()->Animator2D()->Play(str);

	m_strBackUp = str;
}

void CSniperScript::JpsAlgorithm(Int32 x, Int32 z)
{
	Vec3 vPos = Transform()->GetRelativePos();
	tTile tTile = CJpsMgr::GetInst()->GetTileObj()->TileMap()->GetInfo(vPos);
	Int32 x1 = tTile.iIndex % TILEX;
	Int32 z1 = tTile.iIndex / TILEX;
	m_vecJps.clear();

	//cout << "시작 타일 인덱스 " << x1 << " " << z1 << endl;
	//cout << "종료 타일 인덱스 " << x << " " << z << endl;
	if (x1 != x || z1 != z)
	{
		m_vecJps = CJpsMgr::GetInst()->Update(x1, z1, x, z);

		m_bActiveJps = true;

		m_eState = UNIT_STATE::RUN;
	}
}

void CSniperScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CSniperScript::Overlap(CCollider2D* _pOther)
{
	if (UNIT_STATE::RUN == m_eState)
		return;

	Vec2 vRelativePos = GetOwner()->Collider2D()->GetFinalPos() - _pOther->GetFinalPos();
	Vec2 vScale = (GetOwner()->Collider2D()->GetScale() + _pOther->GetScale()) * 0.5f;
	Vec3 vDiff{};
	vDiff.x = fabsf(vScale.x - fabsf(vRelativePos.x));
	vDiff.z = fabsf(vScale.y - fabsf(vRelativePos.y));
	Vec3 vDir = { vRelativePos.x , 0.f, vRelativePos.y };
	vDir = vDir.Normalize();

	Vec3 vPos = Transform()->GetRelativePos();

	Transform()->SetRelativePos(vPos + vDiff * vDir * DT * 8);
}

void CSniperScript::EndOverlap(CCollider2D* _pOther)
{
}

void CSniperScript::SaveToFile(FILE* _File)
{
	CScript::SaveToFile(_File);
	fwrite(&m_fSpeed, sizeof(float), 1, _File);

	SaveResourceRef(m_Prefab, _File);
}

void CSniperScript::LoadFromFile(FILE* _File)
{
	CScript::LoadFromFile(_File);
	fread(&m_fSpeed, sizeof(float), 1, _File);

	LoadResourceRef(m_Prefab, _File);
}

void CSniperScript::ProcessEnemy()
{
	vector<CGameObject*> vecEnemy = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(2)->GetParentObjects();

	if (!vecEnemy.empty())
	{
		sort(vecEnemy.begin(), vecEnemy.end(), [&](CGameObject* lhs, CGameObject* rhs)
			{
				Vec3 vRhs = rhs->Transform()->GetRelativePos();
				Vec3 vLhs = lhs->Transform()->GetRelativePos();

				if (m_vSource.Distance(m_vSource, vRhs) > m_vSource.Distance(m_vSource, vLhs))
					return true;
				else
					return false;
			});

		Vec3 vEnemyPos = vecEnemy[0]->Transform()->GetRelativePos();
		Vec3 vScale1 = vecEnemy[0]->Transform()->GetRelativeScale() * 0.5f;
		Vec3 vScale2 = Transform()->GetRelativeScale() * 0.5f;

		if (m_vSource.Distance(m_vSource, vEnemyPos) <= vScale1.Distance(vScale1, vScale2) + 500.f)
		{
			m_pTargetObject = vecEnemy[0];
			m_eState = UNIT_STATE::LOADATTACK;
			SetDestPos(vEnemyPos);
		}
	}
}

void CSniperScript::ChaseEnemy()
{
	vector<CGameObject*> vecEnemy = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(2)->GetParentObjects();

	if (!vecEnemy.empty())
	{
		sort(vecEnemy.begin(), vecEnemy.end(), [&](CGameObject* lhs, CGameObject* rhs)
			{
				Vec3 vRhs = rhs->Transform()->GetRelativePos();
				Vec3 vLhs = lhs->Transform()->GetRelativePos();

				if (m_vSource.Distance(m_vSource, vRhs) > m_vSource.Distance(m_vSource, vLhs))
					return true;
				else
					return false;
			});

		Vec3 vEnemyPos = vecEnemy[0]->Transform()->GetRelativePos();
		Vec3 vScale1 = vecEnemy[0]->Transform()->GetRelativeScale() * 0.5f;
		Vec3 vScale2 = Transform()->GetRelativeScale() * 0.5f;

		if (m_vSource.Distance(m_vSource, vEnemyPos) > vScale1.Distance(vScale1, vScale2) + 500.f)
		{
			tTile tTile = CJpsMgr::GetInst()->GetTileObj()->TileMap()->GetInfo(vEnemyPos);
			Int32 x = tTile.iIndex % TILEX;
			Int32 z = tTile.iIndex / TILEX;

			m_eState = UNIT_STATE::RUN;
			SetDestPos(vEnemyPos);

			JpsAlgorithm(x, z);
		}
	}

	//if (m_pTargetObject)
	//{
	//	Vec3 vEnemyPos = m_pTargetObject->Transform()->GetRelativePos();
	//	Vec3 vScale1 = m_pTargetObject->Transform()->GetRelativeScale() * 0.5f;
	//	Vec3 vScale2 = Transform()->GetRelativeScale() * 0.5f;

	//	if (m_vSource.Distance(m_vSource, vEnemyPos) > vScale1.Distance(vScale1, vScale2) + 250.f)
	//	{

	//		tTile tTile = CJpsMgr::GetInst()->GetTileObj()->TileMap()->GetInfo(vEnemyPos);
	//		Int32 x = tTile.iIndex % TILEX;
	//		Int32 z = tTile.iIndex / TILEX;

	//		m_eState = UNIT_STATE::RUN;
	//		SetDestPos(vEnemyPos);

	//		JpsAlgorithm(x, z);
	//	}
	//}
}

void CSniperScript::Move(Int32 _x, Int32 _z)
{
	m_pTargetObject = nullptr;
	m_bAttack = false;
	JpsAlgorithm(_x, _z);
}

void CSniperScript::SetMonsterHP()
{
	wstring wstrName = m_pTargetObject->GetName();

	float fHp{};

	fHp = m_pTargetObject->GetScripts()[0]->GetHp();
	fHp -= m_iAttack;
	m_pTargetObject->GetScripts()[0]->SetHp(fHp);
}
#include <Engine\CEngine.h>
#include <Engine\CFontMgr.h>
#include <Script\\CButtonScript.h>

void CSniperScript::PhaseEventOn()
{
	__super::PhaseEventOn();

	lstrcpy(CEngine::g_szFullName, L"Soldier");

	//lstrcpy(g_szHp, to_wstring(m_fHP));
	wchar_t sz[200];

	lstrcpy(sz, to_wstring(m_fHP).c_str());
	lstrcat(sz, L"/");
	lstrcat(sz, to_wstring(m_fFullHp).c_str());

	lstrcpy(CEngine::g_szHp, sz);

	m_pPortrait->MeshRender()->Activate();

	CEngine::g_IconText.clear();

	SetIconUI(m_iAttack, 0);
	SetIconUI(m_iArmor, 1);
	SetIconUI(m_iSpeed, 2);
	SetIconUI(m_iAttackSpeed, 3);
	SetIconUI(m_iAttackRange, 4);
	SetIconUI(m_iGold, 5);
	SetIconUI(m_iWorker, 6);

	const vector<CGameObject*> vec = CInterfaceMgr::GetInst()->GetTapButtons();
	for (size_t i{}; i < 6; ++i)
		vec[i]->GetScript<CButtonScript>()->SetColumn((UINT)UNIT);

	GetOwner()->GetChilds()[0]->GetRenderComponent()->Activate();
}

void CSniperScript::PhaseEventOff()
{
	__super::PhaseEventOff();

	m_pPortrait->MeshRender()->Deactivate();

	for (size_t i{}; i < m_vecIcon.size(); ++i)
		m_vecIcon[i]->MeshRender()->Deactivate();

	GetOwner()->GetChilds()[0]->GetRenderComponent()->Deactivate();
}
