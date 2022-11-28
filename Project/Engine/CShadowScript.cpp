#include "pch.h"
#include "CShadowScript.h"

#include "CDevice.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CUIMgr.h"

#include "CTransform.h"
#include "CMaterial.h"
#include "CMeshRender.h"

#include "CPrefab.h"

CShadowScript::CShadowScript()
	: m_fSpeed{ 100.f }
	, m_vColor{}
{
	SetName(L"ShadowScript");
}

CShadowScript::~CShadowScript()
{
}


void CShadowScript::begin()
{
}

void CShadowScript::tick()
{
}

void CShadowScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CShadowScript::Overlap(CCollider2D* _pOther)
{
}

void CShadowScript::EndOverlap(CCollider2D* _pOther)
{
}

void CShadowScript::SetColor(Vec4 _vColor)
{
	m_vColor = _vColor;
}