#pragma once

#include "CRes.h"

class CMesh : 
    public CRes
{
private:

    ComPtr<ID3D11Buffer>     m_readVB;
    ComPtr<ID3D11Buffer>     m_VB;
    D3D11_BUFFER_DESC        m_tVBDesc;

    ComPtr<ID3D11Buffer>     m_IB;
    D3D11_BUFFER_DESC        m_tIBDesc;
    UINT                     m_iIdxCount;

    std::unique_ptr<Vtx[]>   m_vertices;
    UINT                     m_iVtxCount;

    //Ray
    Vec3                     m_vDir;
    Vec3                     m_vOrig;
public :
    int Create(void* _pVtxSysmem, size_t _iVtxCount, void* _pIdxSysmem, size_t _iIdxCount);
    void UpdateData();
    void render();
    void render_particle(UINT _iCount);
    void Read();
    void Write();
    bool Collision(Vec3 _vPos);
    bool Picking(const Vec3& _vPos, UINT& i);
    bool IntersectTriangle(const Vec3& orig, const Vec3& dir, Vec3& v0, Vec3& v1, Vec3& v2, FLOAT* t, FLOAT* u, FLOAT* v);
    virtual void Save(const wstring _strRelativePath) {}
    virtual int Load(const wstring& _strFilePath) override { return S_OK; }

    CLONE_ASSERT(CMesh);
public:
    CMesh(bool _bEngineRes = false);
    virtual ~CMesh();
};