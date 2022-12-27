#include "pch.h"
#include "CRefAniScript.h"

#include <Engine\CUIMgr.h>
#include <Engine\CScript.h>
#include <Engine\CTransform.h>
#include <Engine\CMaterial.h>
#include <Engine\CMeshRender.h>

CRefAniScript::CRefAniScript()
	:CScript{ REFANISCRIPT}
{
	SetName(L"CRefAniScript");
}

CRefAniScript::~CRefAniScript()
{
}


void CRefAniScript::begin()
{
}

void CRefAniScript::tick()
{
	/*
	* 마우스가 있으면
	* 시작 위치(첫 클릭시)와 끝 위치(마우스 땟을 때)를 받아온다.
	* Rect 매쉬를 생성한다. 
	* 
	*/
}

void CRefAniScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CRefAniScript::Overlap(CCollider2D* _pOther)
{
}

void CRefAniScript::EndOverlap(CCollider2D* _pOther)
{
}