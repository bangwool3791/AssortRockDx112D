#include "pch.h"
#include "CMesh.h"

#include "CDevice.h"

CMesh::CMesh()
	:CRes(RES_TYPE::MESH)
	, m_tVBDesc{}
	, m_tIBDesc{}
	, m_iIdxCount{0}
{

}

CMesh::~CMesh()
{

}

int CMesh::Create(void* _pVtxSysmem, size_t _iVtxCount, void* _pIdxSysmem, size_t _iIdxCount)
{
	m_tVBDesc.ByteWidth = UINT(sizeof(Vtx) * _iVtxCount); 
	m_tVBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_tVBDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tVBDesc.CPUAccessFlags = 0;
	
	D3D11_SUBRESOURCE_DATA tSubData = {};
	tSubData.pSysMem = _pVtxSysmem;

	HRESULT hr = DEVICE->CreateBuffer(&m_tVBDesc, &tSubData, m_VB.GetAddressOf());

	if (FAILED(hr))
	{
		return hr;
	}

	m_iIdxCount = (UINT)_iIdxCount;
	m_tIBDesc.ByteWidth = sizeof(UINT) * (UINT)_iIdxCount;
	m_tIBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	m_tIBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	m_tIBDesc.CPUAccessFlags = 0;

	tSubData = {};
	//memset(&tSubData, 0, sizeof(D3D11_SUBRESOURCE_DATA));

	tSubData.pSysMem = _pIdxSysmem;

	hr = DEVICE->CreateBuffer(&m_tIBDesc, &tSubData, &m_IB);

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