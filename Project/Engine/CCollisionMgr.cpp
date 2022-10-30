#include "pch.h"
#include "CCollisionMgr.h"

#include "CTransform.h"
#include "CCollider2D.h"
#include "CGameObject.h"
#include "CLevel.h"
#include "CLevelMgr.h"

CCollisionMgr::CCollisionMgr()
{
}

CCollisionMgr::~CCollisionMgr()
{
}

/*
* 행렬의 절반 만큼 검사
* 0, 0 의경우, 
*/
void CCollisionMgr::tick()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();

	for (int iRow{ 0 }; iRow < MAX_LAYER; ++iRow)
	{
		for (int iCol{iRow}; iCol < MAX_LAYER; ++iCol)
		{
			if (m_matrix[iRow] & (1 << iCol))
			{
				CollisionBtwLayer(pCurLevel, iRow, iCol);
			}
		}
	}
}

void CCollisionMgr::CollisionBtwLayer(CLevel* _pCurScene, int _iLeft, int _iRight)
{
	const vector<CGameObject*>& vecLeft = _pCurScene->GetLayer(_iLeft)->GetObjects();
	const vector<CGameObject*>& vecRight = _pCurScene->GetLayer(_iRight)->GetObjects();

	for (int i{ 0 }; i < vecLeft.size(); ++i)
	{
		if (nullptr == vecLeft[i]->Collider2D())
			continue;

		int j{ 0 };
		
		if (_iLeft == _iRight)
			j = i + 1;

		for (; j < vecRight.size(); ++j)
		{
			if (nullptr == vecRight[j]->Collider2D())
				continue;

			CollisionBtwCollider(vecLeft[i]->Collider2D(), vecRight[j]->Collider2D());
		}
	}
}

void CCollisionMgr::CollisionBtwCollider(CCollider2D* _pLeft, CCollider2D* _pRight)
{
	COLLIDER_ID id;
	id.LEFT_ID = _pLeft->GetID();
	id.RIGHT_ID = _pRight->GetID();

	auto iter = m_mapColInfo.find(id.ID);

	if (iter == m_mapColInfo.end())
	{
		m_mapColInfo.insert(make_pair(id.ID, false));
		iter = m_mapColInfo.find(id.ID);
	}

	bool IsDead = _pLeft->GetOwner()->IsDead() || _pRight->GetOwner()->IsDead();

	if (IsCollision(_pLeft, _pRight))
	{
		if (false == iter->second)
		{
			if (!IsDead)
			{
				_pLeft->BeginOverlap(_pRight);
				_pRight->BeginOverlap(_pLeft);
				iter->second = true;
			}
		}
		else
		{
			if (IsDead)
			{
				_pLeft->EndOverlap(_pRight);
				_pRight->EndOverlap(_pLeft);
				iter->second = false;
			}
			else
			{
				_pLeft->Overlap(_pRight);
				_pRight->Overlap(_pLeft);
			}
		}
	}
	else
	{
		if (iter->second)
		{
			_pLeft->EndOverlap(_pRight);
			_pRight->EndOverlap(_pLeft);
			iter->second = false;
		}
	}
}

bool CCollisionMgr::IsCollision(CCollider2D* _pLeft, CCollider2D* _pRight)
{
	// 0 --- 1
	// |  \  | 
	// 3 --- 2	

//#ifdef true
	static const Vec3 arrLocalPos[4]
	{
		Vec3{-0.5f,  0.5f, 0.f},
		Vec3{ 0.5f,  0.5f, 0.f},
		Vec3{ 0.5f, -0.5f, 0.f},
		Vec3{ -0.5f, -0.5f, 0.f},
	};

	Vec3 vAxis[4]{};

	const Matrix& matLeft	= _pLeft->GetWorldMat();
	const Matrix& matRight	= _pRight->GetWorldMat();

	/*
	* 마우스 드래그의 경우 월드 행렬 스케일이 없지만, 초기 위치가 플레이어 위치와 동일하여
	* 충돌 판정이라고 코드가 판단 하므로 예외 처리 조건 추가
	*/
	Vec3 vecRight = matLeft.Right();
	Vec3 vecUp = matLeft.Up();
	
	if (!vecRight.Length() && !vecUp.Length())
		return false;

	vecRight = matRight.Right();
	vecUp = matRight.Up();

	if (!vecRight.Length() && !vecUp.Length())
		return false;

	vAxis[0] = XMVector3TransformCoord(arrLocalPos[1], matLeft) - XMVector3TransformCoord(arrLocalPos[0], matLeft);
	vAxis[1] = XMVector3TransformCoord(arrLocalPos[3], matLeft) - XMVector3TransformCoord(arrLocalPos[0], matLeft);

	vAxis[2] = XMVector3TransformCoord(arrLocalPos[1], matRight) - XMVector3TransformCoord(arrLocalPos[0], matRight);
	vAxis[3] = XMVector3TransformCoord(arrLocalPos[3], matRight) - XMVector3TransformCoord(arrLocalPos[0], matRight);

	for (int i{ 0 }; i < 4; ++i)
		vAxis[i].z = 0.f;

	Vec2 vC = _pLeft->GetFinalPos() - _pRight->GetFinalPos() ;
	Vec3 vCenterDir = Vec3{ vC.x,vC.y, 0.f };
	
	float fSum{ 0.f };

	for (int i{ 0 }; i < 4; ++i)
	{
		Vec3 vA = vAxis[i];
		vA.Normalize();

		float fProjDist{ 0.f };

		for (int j{ 0 }; j < 4; ++j)
		{
			fProjDist += fabsf(vAxis[j].Dot(vA)) /2.f;
		}

		if (fProjDist < fabsf(vCenterDir.Dot(vA)))
		{
			return false;
		}
	}
	return true;
}

void CCollisionMgr::CollisionLayerCheck(int _iLeft, int _iRight)
{
	int iRow{ 0 }, iCol{ 0 };

	//iLeft 0, iRight 32
	if (_iLeft <= _iRight)
	{
		iRow = _iLeft;
		iCol = _iRight;
	}
	//iLeft 32, iRight 0
	else
	{
		iRow = _iRight;
		iCol = _iLeft;
	}

	m_matrix[iRow] |= (1 << iCol);
}

void CCollisionMgr::CollisionLayerRelease(int _iLeft, int _iRight)
{
	int iRow{ 0 }, iCol{ 0 };

	//iLeft 0, iRight 32
	if (_iLeft <= _iRight)
	{
		iRow = _iLeft;
		iCol = _iRight;
	}
	//iLeft 32, iRight 0
	else
	{
		iRow = _iRight;
		iCol = _iLeft;
	}

	m_matrix[iRow] &= ~(1 << iCol);
}

