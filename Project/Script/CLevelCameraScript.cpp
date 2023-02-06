#include "pch.h"
#include "CLevelCameraScript.h"


CLevelCameraScript::CLevelCameraScript()
	:CScript(SCRIPT_TYPE::LEVELCAMERASCRIPT)
	, m_fSpeed(300.f)
	, m_vPos{}
{
}

CLevelCameraScript::~CLevelCameraScript()
{
}

void CLevelCameraScript::begin()
{
	m_fScale = Camera()->GetOrthographicScale();
}

void CLevelCameraScript::tick()
{
	Move();
}

void CLevelCameraScript::Move()
{
	static float fdt = DT;
	fdt += DT;

	Vec3 vPos = Transform()->GetRelativePos();

	float fSpeed = m_fSpeed;

	if (KEY_PRESSED(KEY::LSHIFT))
	{
		fSpeed *= 5.f;
	}

	if (Camera()->GetProjType() == PROJ_TYPE::PERSPECTIVE)
	{
		Vec3 vFront = Transform()->GetRelativeDir(DIR::FRONT);
		Vec3 vRight = Transform()->GetRelativeDir(DIR::RIGHT);

		if (KEY_PRESSED(KEY::W))
			vPos += DT * vFront * fSpeed;
		if (KEY_PRESSED(KEY::S))
			vPos += DT * vFront * -fSpeed;
		if (KEY_PRESSED(KEY::A))
			vPos += DT * -vRight * fSpeed;
		if (KEY_PRESSED(KEY::D))
			vPos += DT * vRight * fSpeed;

		Vec2 vMouseDir = CKeyMgr::GetInst()->GetMouseDir();


		if (KEY_PRESSED(KEY::RBTN))
		{
			Vec3 vRot = Transform()->GetRelativeRotation();

			vRot.y += vMouseDir.x * DT * XM_PI;
			vRot.x -= vMouseDir.y * DT * XM_PI;
			Transform()->SetRelativeRotation(vRot);
		}
		Transform()->SetRelativePos(vPos);

		if (KEY_PRESSED(KEY::SPACE))
		{
			if (fdt > 0.5f)
			{
				Camera()->SetProjType(PROJ_TYPE::ORTHOGRAHPICS);
				Camera()->SetOrthographicScale(m_fScale);
				Transform()->SetRelativeRotation(Vec3(XM_PI * 0.25f, 0.f, 0.f));
				fdt = 0.f;
			}
		}
	}

	else
	{
		Vec3 vFront = Transform()->GetRelativeDir(DIR::UP);
		Vec3 vRight = Transform()->GetRelativeDir(DIR::RIGHT);

		if (KEY_PRESSED(KEY::W))
			vPos += DT * vFront * fSpeed;
		if (KEY_PRESSED(KEY::S))
			vPos += DT * vFront * -fSpeed;
		if (KEY_PRESSED(KEY::A))
			vPos += DT * -vRight * fSpeed;
		if (KEY_PRESSED(KEY::D))
			vPos += DT * vRight * fSpeed;

		Vec2 vMouseDir = CKeyMgr::GetInst()->GetMouseDir();

		Transform()->SetRelativePos(vPos);

		if (KEY_PRESSED(KEY::NUM_1))
		{
			m_fScale = Camera()->GetOrthographicScale();
			m_fScale += DT;
			Camera()->SetOrthographicScale(m_fScale);
		}
		else if (KEY_PRESSED(KEY::NUM_2))
		{
			m_fScale = Camera()->GetOrthographicScale();
			m_fScale -= DT;
			Camera()->SetOrthographicScale(m_fScale);
		}

		if (KEY_PRESSED(KEY::SPACE))
		{
			if (fdt > 0.5f)
			{
				m_vPos = Transform()->GetRelativeDir(DIR::FRONT);
				Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
				fdt = 0.f;
			}
		}
	}
}
