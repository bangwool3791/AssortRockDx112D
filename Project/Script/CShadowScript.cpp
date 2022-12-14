#include "pch.h"
#include "CShadowScript.h"

CShadowScript::CShadowScript()
	: CScript{SHADOWSCRIPT}
	, m_fSpeed{ 100.f }
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