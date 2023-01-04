#include "pch.h"
#include "CMesh.h"

#include "CDevice.h"
#include <unordered_set>

CMesh::CMesh(bool _bEngineRes)
	: CRes(RES_TYPE::MESH, _bEngineRes)
	, m_tVBDesc{}
	, m_tIBDesc{}
	, m_iIdxCount(0)
	, m_iVtxCount{0}
{
}


CMesh::~CMesh()
{
    m_readVB = nullptr;
}

int CMesh::Create(void* _pVtxSysmem, size_t _iVtxCount, void* _pIdxSysmem, size_t _iIdxCount)
{
	m_tVBDesc.ByteWidth = UINT(sizeof(Vtx) * _iVtxCount); 
	m_iVtxCount = _iVtxCount;
	m_tVBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_tVBDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tVBDesc.CPUAccessFlags = 0;
	
	D3D11_SUBRESOURCE_DATA tSubData = {};
	tSubData.pSysMem = _pVtxSysmem;

	HRESULT hr = DEVICE->CreateBuffer(&m_tVBDesc, &tSubData, m_VB.GetAddressOf());
	//m_VB->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("CComputeShader::m_VB") - 1, "CComputeShader::m_VB");

	if (FAILED(hr))
	{
		return hr;
	}

	m_iIdxCount = (UINT)_iIdxCount;
	m_tIBDesc.ByteWidth = sizeof(UINT) * (UINT)_iIdxCount;
	m_tIBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	m_tIBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	m_tIBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	tSubData = {};
	//memset(&tSubData, 0, sizeof(D3D11_SUBRESOURCE_DATA));

	tSubData.pSysMem = _pIdxSysmem;

	hr = DEVICE->CreateBuffer(&m_tIBDesc, &tSubData, &m_IB);
	//m_IB->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("CComputeShader::m_IB") - 1, "CComputeShader::m_IB");
	return hr;
}

void CMesh::UpdateData()
{
	UINT iStride = sizeof(Vtx);
	UINT iOffset = 0;

	CONTEXT->IASetVertexBuffers(0, 1, m_VB.GetAddressOf(), &iStride, &iOffset);
	CONTEXT->IASetIndexBuffer(m_IB.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void CMesh::render()
{
	UpdateData();
	CONTEXT->DrawIndexed(m_iIdxCount, 0, 0);
}

void CMesh::render_particle(UINT _iCount)
{
	UpdateData();
	CONTEXT->DrawIndexedInstanced(m_iIdxCount, _iCount, 0, 0, 0);
}

void CMesh::Read()
{
    if (nullptr != m_readVB)
    {
        m_readVB = nullptr;
    }

    D3D11_BUFFER_DESC VbufferDesc;

    VbufferDesc.Usage = D3D11_USAGE_STAGING;
    VbufferDesc.ByteWidth = m_tVBDesc.ByteWidth;
    VbufferDesc.BindFlags = 0;
    VbufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    VbufferDesc.MiscFlags = 0;

    //Create the Vertex staging buffer.
    HRESULT hr = DEVICE->CreateBuffer(&VbufferDesc, NULL, &m_readVB);
    assert(hr == S_OK);
    //Copy veBuffer into staging buffer so  it can be mapped/read
    CONTEXT->CopyResource(m_readVB.Get(), m_VB.Get());

    size_t nVerts = m_tVBDesc.ByteWidth / sizeof(Vtx);
    m_vertices = std::make_unique<Vtx[]>(nVerts);

    //map vertex buffer on GPU
    D3D11_MAPPED_SUBRESOURCE msV = {};
    HRESULT hrm = CONTEXT->Map(m_readVB.Get(), NULL, D3D11_MAP_READ, NULL, &msV);
    assert(hrm == S_OK);

    //loop through buffer
    if (!nVerts)
        cout << "Failed computing nVerts" << endl;
    else
    {
        memcpy(m_vertices.get(), msV.pData, nVerts * sizeof(Vtx));
       //cout << m_VB.Get() << endl; //Current Buffer veing processed
        //for (size_t i = 0; i < nVerts; i++)
        //{
        //   cout << vertices[i].vPos.x << ", "
        //       << vertices[i].vPos.y << ", "
        //       << vertices[i].vPos.z << endl; //
        //} 
        //cout << "buffer end" << endl; //
        //cout << "" << endl; // 
    }

    CONTEXT->Unmap(m_readVB.Get(), 0);
}

void CMesh::Write()
{
    Read();
    ComPtr<ID3D11Buffer> WriteBuffer;
    D3D11_BUFFER_DESC desc;
    desc.Usage = D3D11_USAGE_DYNAMIC;
    // Not sure if the temp buffer needs to have D3D11_BIND_UNORDERED_ACCESS
    // like the Unity buffer does.
    // We're only writing into the temp buffer from a single thread.
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.ByteWidth = m_tVBDesc.ByteWidth;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    // Using MiscFlags and StructureByteStride from Unity's buffer doesn't seem to work
    desc.MiscFlags = 0; // unityDesc.MiscFlags;
    //desc.StructureByteStride = unityDesc.StructureByteStride;
    HRESULT hr = DEVICE->CreateBuffer(&desc, NULL, &WriteBuffer);

    D3D11_MAPPED_SUBRESOURCE mapped;

    CONTEXT->CopyResource(WriteBuffer.Get(), m_VB.Get());

    size_t nVerts = m_tVBDesc.ByteWidth / sizeof(Vtx);

    hr = CONTEXT->Map(WriteBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, NULL, &mapped);
    memcpy(mapped.pData, m_vertices.get(), m_tVBDesc.ByteWidth);
    CONTEXT->Unmap(WriteBuffer.Get(), 0);
}

bool CMesh::Collision(Vec3 _vPos)
{
	Read();

    size_t nVerts = m_tVBDesc.ByteWidth / sizeof(Vtx);

    for (UINT i = 0; i < nVerts / 4; i+= 4)
    {
        if (Picking(_vPos, i))
            return true;
    } 
	return false;
}

bool CMesh::Picking(const Vec3& _vPos, UINT& i)
{
    Vec3	vPoint[4] = {
    Vec3(m_vertices[i + 1].vPos.x, 0.f, m_vertices[i + 1].vPos.z),
    Vec3(m_vertices[i + 2].vPos.x, 0.f, m_vertices[i + 2].vPos.z),
    Vec3(m_vertices[i + 3].vPos.x, 0.f, m_vertices[i + 3].vPos.z),
    Vec3(m_vertices[i + 0].vPos.x, 0.f, m_vertices[i + 0].vPos.z),
    };
    // 시계방향으로 방향벡터를 만들자
    Vec3		vDir[4] = {
        vPoint[1] - vPoint[0],
        vPoint[2] - vPoint[1],
        vPoint[3] - vPoint[2],
        vPoint[0] - vPoint[3]
    };
    // 각 방향벡터의 법선을 구함
    Vec3		vNormal[4] = {
        Vec3(-vDir[0].z, 0.f, vDir[0].x),
        Vec3(-vDir[1].z, 0.f, vDir[1].x),
        Vec3(-vDir[2].z, 0.f, vDir[2].x),
        Vec3(-vDir[3].z, 0.f, vDir[3].x)
    };

    Vec3	vMouseDir[4] = {
        _vPos - vPoint[0],
        _vPos - vPoint[1],
        _vPos - vPoint[2],
        _vPos - vPoint[3]
    };
    // 구한 법선 벡터들을 정규화(단위벡터) 시켜준다.
    for (int i = 0; i < 4; ++i)
        vNormal[i].Normalize();
    for (int i = 0; i < 4; ++i)
    {
        // 예각이 나왔다
        if (0.f < vNormal[i].Dot(vMouseDir[i]))
            return false;
    }

    cout << "[x] " << m_vertices[i + 1].vPos.x << "[z] " << m_vertices[i + 1].vPos.z << endl;
    return true;
}
// 정점과 충돌 체크
// vOrig는 카메라 원점, vDir는 Ray 방향
// v0, v1, v2 는 삼각형의 정점
// t 는 vOrig에서 v0 까지의 거리
// u 는 v0에서 v1 이고, v 는 v0에서 v2 이다.
bool CMesh::IntersectTriangle(const Vec3& orig, const Vec3& dir, Vec3& v0, Vec3& v1, Vec3& v2, FLOAT* t, FLOAT* u, FLOAT* v)
{     // Find vectors for two edges sharing vert0     
    Vec3 edge1 = v1 - v0;
    Vec3 edge2 = v2 - v0;
    // Begin calculating determinant - also used to calculate U parameter     
    Vec3 pvec;    
    pvec = dir.Cross(edge2);
    // If determinant is near zero, ray lies in plane of triangle     
    FLOAT det = edge1.Dot(pvec);
    Vec3 tvec;
    if( det > 0 )     
    {         tvec = orig - v0;     }     
    else     {         tvec = v0 - orig;         det = -det;     }    
    if( det < 0.0001f )         
        return FALSE;     
    // Calculate U parameter and test bounds     
    *u = tvec.Dot(pvec);     
    if( *u < 0.0f || *u > det )         
        return FALSE;    
    // Prepare to test V parameter    
    Vec3 qvec;     
    qvec = tvec.Cross(edge1);    
    // Calculate V parameter and test bounds    
    *v = dir.Dot(qvec);     
    if( *v < 0.0f || *u + *v > det )         
        return FALSE;     
    // Calculate t, scale parameters, ray intersects triangle     
    *t = edge2.Dot(qvec);     
    FLOAT fInvDet = 1.0f / det;     
    *t *= fInvDet;     
    *u *= fInvDet;     
    *v *= fInvDet;     
    return TRUE; 
}
