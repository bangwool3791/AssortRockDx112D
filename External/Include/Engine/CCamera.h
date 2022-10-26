#pragma once
#include "CComponent.h"

enum PROJ_TYPE
{
    PERSPECTIVE,
    ORTHOGRAHPICS,
};

class CCamera :
    public CComponent
{
private:
    Matrix      m_matView;
    Matrix      m_matProj;

    PROJ_TYPE   m_eProjType;
    float       m_fAspectRatio;

    float       m_fFar;
    float       m_fScale;

    UINT                    m_iLayerMask;

    vector<CGameObject*>    m_vecQpaque;
    vector<CGameObject*>    m_vecMask;
    vector<CGameObject*>    m_vecTransparent;

private :
    void SortObject();
    void render_opaque();
    void render_mask();
    void render_transparent();
public :
    virtual void finaltick();
    void         render();
public :
    float        GetOrthographicScale() { return m_fScale; }
    float&       GetOrthographicScale_() { return m_fScale; }
    void         SetOrthographicScale(float _fScale) { m_fScale = _fScale; }

    void        SetProjType(PROJ_TYPE _eType) { m_eProjType = _eType; }
    PROJ_TYPE   GetProjType() { return m_eProjType; }

    void  SetAspectRatio(float _fRatio) { m_fAspectRatio = _fRatio; }
    float GetAspectRatio() { return m_fAspectRatio; }

    void  SetFar(float _fFar) { m_fFar = _fFar; }
    float GetFar() { return m_fFar; }

    /*
    * Matrix°´Ã¼´Â const ref·Î return
    */
    const Matrix& GetViewMat() { return m_matView; }
    const Matrix& GetProjMat() { return m_matProj; }

    void SetLayerMask(const wstring& _strLayerName);
    void SetLayerMask(int _iLayerIdx);
    void SetLayerMaskAll() { m_iLayerMask = 0xffffffff; }
    void SetLayerMaskZero() { m_iLayerMask = 0; }

public :
    CLONE(CCamera);
public :
    CCamera();
    ~CCamera();
};

