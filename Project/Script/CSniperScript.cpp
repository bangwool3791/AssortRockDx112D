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

CSniperScript::CSniperScript()
	:CScript{ SNIPERSCRIPT }
	, m_fSpeed{ 100.f }
	, m_eState{ UNIT_STATE::NORMAL }
	, m_iHp{ 100 }
	, m_iAttack{ 15 }
{
	SetName(L"CSniperScript");

	//AddScriptParam(SCRIPT_PARAM::FLOAT, "Player MoveSpeed", &m_fSpeed);
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
}

#define OFFSET 22.5f

void CSniperScript::tick()
{
	if (0 >= m_iHp)
		m_eState = UNIT_STATE::DEAD;

	if (m_pTargetObject && m_pTargetObject->IsDead())
		m_pTargetObject = nullptr;

	m_fDeltaTime += DT;

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

		if (GetOwner()->Transform()->Picking(ray, m_vDest))
		{
			CInterfaceMgr::GetInst()->SetTarget(GetOwner());
		}
	}

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

		if (m_vDest.Distance(m_vSource, *iter) < 10)
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

	cout << "시작 타일 인덱스 " << x1 << " " << z1 << endl;
	cout << "종료 타일 인덱스 " << x << " " << z << endl;
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
	//Vec3 vRelativePos = Transform()->GetRelativePos() - _pOther->Transform()->GetRelativePos();
	//Vec3 vScale = (Transform()->GetRelativeScale() + _pOther->Transform()->GetRelativeScale()) * 0.5f;
	//Vec3 vDiff{};
	//vDiff.x = fabsf(vScale.x - fabsf(vRelativePos.x));
	//vDiff.y = fabsf(vScale.y - fabsf(vRelativePos.y));
	//vDiff.z = fabsf(vScale.z - fabsf(vRelativePos.z));
	//vRelativePos = vRelativePos.Normalize();
	//
	//Vec3 vPos = Transform()->GetRelativePos();
	//
	//Transform()->SetRelativePos(vPos + vDiff * vRelativePos);
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

	UINT iHp{};

	if (!lstrcmp(L"CInfectedGiant", wstrName.c_str()))
	{
		iHp = m_pTargetObject->GetScript< CInfectedGiantScript>()->GetHp();
		iHp -= m_iAttack;
		m_pTargetObject->GetScript< CInfectedGiantScript>()->SetHp(iHp);
	}
	else if (!lstrcmp(L"CInfectedMedium_A_", wstrName.c_str()))
	{
		iHp = m_pTargetObject->GetScript< CInfectedMedium_A>()->GetHp();
		iHp -= m_iAttack;
		m_pTargetObject->GetScript< CInfectedMedium_A>()->SetHp(iHp);
	}
	else if (!lstrcmp(L"CInfectedMedium_B_", wstrName.c_str()))
	{
		iHp = m_pTargetObject->GetScript< CInfectedMedium_B>()->GetHp();
		iHp -= m_iAttack;
		m_pTargetObject->GetScript< CInfectedMedium_B>()->SetHp(iHp);
	}
	else if (!lstrcmp(L"CInfectedStrong_A_", wstrName.c_str()))
	{
		iHp = m_pTargetObject->GetScript< CInfectedStrong_A>()->GetHp();
		iHp -= m_iAttack;
		m_pTargetObject->GetScript< CInfectedStrong_A>()->SetHp(iHp);
	}
	else if (!lstrcmp(L"CInfectedVenom", wstrName.c_str()))
	{
		iHp = m_pTargetObject->GetScript< CInfectedVenom>()->GetHp();
		iHp -= m_iAttack;
		m_pTargetObject->GetScript< CInfectedVenom>()->SetHp(iHp);
	}
}