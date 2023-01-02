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
    if (m_readVB != NULL) 
    { 
        m_readVB->Release(); 
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
    auto vertices = std::make_unique<Vtx[]>(nVerts);

    //map vertex buffer on GPU
    D3D11_MAPPED_SUBRESOURCE msV = {};
    HRESULT hrm = CONTEXT->Map(m_readVB.Get(), NULL, D3D11_MAP_READ, NULL, &msV);
    assert(hrm == S_OK);

    //loop through buffer
    if (!nVerts)
        cout << "Failed computing nVerts" << endl;
    else
    {
        memcpy(vertices.get(), msV.pData, nVerts * sizeof(Vtx));
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
    auto vertices = new Vtx[nVerts];

    hr = CONTEXT->Map(WriteBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, NULL, &mapped);
    memcpy(mapped.pData, vertices, m_tVBDesc.ByteWidth);
    CONTEXT->Unmap(WriteBuffer.Get(), 0);
}