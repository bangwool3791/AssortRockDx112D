#pragma once
#include "CComponent.h"

class CStructuredBuffer;
enum PROJ_TYPE
{
    PERSPECTIVE,
    ORTHOGRAHPICS,
};

class CCamera :
    public CComponent

{
private:
    Ray                    m_ray;

    Matrix                  m_matView;
    Matrix                  m_matProj;

    PROJ_TYPE               m_eProjType;
    float                   m_fAspectRatio;

    float                   m_fFar;
    float                   m_fNear;
    float                   m_fScale;

    UINT                    m_iLayerMask;
    int                     m_iCamIdx;

    vector<CGameObject* >                    m_vecOpaque;
    vector<CGameObject* >                    m_vecMask;
    vector<CGameObject* >                    m_vecTransparent;

    map<const wstring, vector<CGameObject*>> m_mapOpaqueVec;
    map<const wstring, vector<CGameObject*>> m_mapMaskVec;
    map<const wstring, vector<CGameObject*>> m_mapTransparentVec;
    vector<CGameObject* >                    m_vecPostProcess;

    CStructuredBuffer*                       m_pObjectRenderBuffer;
private :
    void SortObject();
    void render_opaque();
    void render_mask();
    void render_transparent();
    void render_postprocess();
    //void update_render(Ptr<CMesh> p);
public :
    virtual void finaltick();
    void         render();
protected:
    void CalcViewMat();
    void CalcProjMat();
public :

    float        GetOrthographicScale() { return m_fScale; }
    float&       GetOrthographicScale_() { return m_fScale; }
    void         SetOrthographicScale(float _fScale) { m_fScale = _fScale; }

    void        SetProjType(PROJ_TYPE _eType) { m_eProjType = _eType; }
    PROJ_TYPE   GetProjType() { return m_eProjType; }

    void        SetAspectRatio(float _fRatio) { m_fAspectRatio = _fRatio; }
    float       GetAspectRatio() { return m_fAspectRatio; }

    void        SetNear(float _fNear) { m_fNear = _fNear; }
    float       GetNear() { return m_fNear; }
    void        SetFar(float _fFar) { m_fFar = _fFar; }
    float       GetFar() { return m_fFar; }

    Ray        CalRay();
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

public:
    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);
public :
    CCamera();
    CCamera(const CCamera& rhs);
    ~CCamera();
};

