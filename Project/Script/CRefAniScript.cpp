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
	* ���콺�� ������
	* ���� ��ġ(ù Ŭ����)�� �� ��ġ(���콺 ���� ��)�� �޾ƿ´�.
	* Rect �Ž��� �����Ѵ�. 
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