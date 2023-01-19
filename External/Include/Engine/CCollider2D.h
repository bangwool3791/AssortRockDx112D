#pragma once
#include "CComponent.h"


class CCollider2D :
    public CComponent
{
private:
    Vec2            m_vOffsetPos;
    Vec2            m_vScale;
    Vec3            m_vRot;

    Vec2            m_vFinalPos;
    Vec2            m_vFinalScale;
    Matrix          m_matWorld;

    COLLIDER2D_TYPE m_eType;
    bool            m_bIgnorObjectScale;
    bool            m_bPause;
    int             m_iOverlapCount;

public:
    bool IsPause() { return m_bPause; }
    void SetPause() { m_bPause = true; }
    void ReleasePause() { m_bPause = false; }
public:
    void BeginOverlap(CCollider2D* _pOther);
    void Overlap(CCollider2D* _pOther);
    void EndOverlap(CCollider2D* _pOther);
public:
    void SetOffsetPos(Vec2 _vOffset) { m_vOffsetPos = _vOffset; }
    void SetScale(Vec2 _vScale) { m_vScale = _vScale; }

    void SetCollider2DType(COLLIDER2D_TYPE _type) { m_eType = _type; }
    COLLIDER2D_TYPE GetCollider2DType() { return m_eType; }

    float GetRotationX() { return m_vRot.x; }
    float GetRotationY() { return m_vRot.y; }
    float GetRotationZ() { return m_vRot.z; }
    void  SetRotationX(float _fDegree);
    void  SetRotationY(float _fDegree);
    void  SetRotationZ(float _fDegree);

    Vec3 GetRotation() { return m_vRot; }

    Vec2 GetOffsetPos() { return m_vOffsetPos; }
    Vec2 GetScale() { return m_vScale; }

    Vec2 GetFinalPos() { return m_vFinalPos; }
    const Matrix& GetWorldMat() { return m_matWorld; }

    void SetIgnoreObjectScale(bool _bSet) { m_bIgnorObjectScale = _bSet; }
    bool GetIgnoreObjectScale() { return m_bIgnorObjectScale; }
    int GetOverlapCount() { return m_iOverlapCount; }
    void ResetOverlapCount() { m_iOverlapCount = 0; }
public:
    virtual void finaltick() override;

public:
    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);
    CLONE(CCollider2D);
public:
    CCollider2D();
    virtual ~CCollider2D();
};

