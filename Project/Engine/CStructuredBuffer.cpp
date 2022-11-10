#include "pch.h"
#include "CStructuredBuffer.h"

#include "CDevice.h"

CStructuredBuffer::CStructuredBuffer()
	:m_tDesc{}
	,m_eType{SB_TYPE::NONE}
	,m_iElementSize{}
	,m_iElementCount{}
{
}

CStructuredBuffer::CStructuredBuffer(const CStructuredBuffer& _origin)
{

}

CStructuredBuffer::~CStructuredBuffer()
{

}


int CStructuredBuffer::Create(UINT _iElementSize, UINT _iElementCount, SB_TYPE _eType, void* _pInitial)
{
	m_SB = nullptr;
	m_SRV = nullptr;

	m_eType = _eType;
	m_iElementSize = _iElementSize;
	m_iElementCount = _iElementCount;

	m_tDesc.ByteWidth = _iElementCount * _iElementSize;
	m_tDesc.StructureByteStride = _iElementSize;

	m_tDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	m_tDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	HRESULT hr = S_OK;

	if (nullptr == _pInitial)
	{
		hr = DEVICE->CreateBuffer(&m_tDesc, nullptr, m_SB.GetAddressOf());
	}
	else
	{
		D3D11_SUBRESOURCE_DATA tSub{};
		tSub.pSysMem = _pInitial;

		hr = DEVICE->CreateBuffer(&m_tDesc, &tSub, m_SB.GetAddressOf());
	}

	// 버퍼 생성 실패
	if (FAILED(hr))
		return E_FAIL;


	D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc{};

	tSRVDesc.BufferEx.NumElements = _iElementCount;
	tSRVDesc.ViewDimension = D3D_SRV_DIMENSION_BUFFEREX;
	
	if (FAILED(DEVICE->CreateShaderResourceView(m_SB.Get(), &tSRVDesc, m_SRV.GetAddressOf())))
	{
		return E_FAIL;
	}
	return 0;
}

void CStructuredBuffer::SetData(void* _pSrc, UINT _iElementCount)
{
	if (m_iElementCount < _iElementCount)
	{
		Create(m_iElementSize, _iElementCount, m_eType, _pSrc);
	}
	else
	{
		D3D11_MAPPED_SUBRESOURCE tMapSub{};
		CONTEXT->Map(m_SB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tMapSub);
		memcpy(tMapSub.pData, _pSrc, m_iElementSize * _iElementCount);
		CONTEXT->Unmap(m_SB.Get(), 0);
	}
}

void CStructuredBuffer::UpdateData(UINT _iRegisterNum, UINT _PipelineStage)
{
	if ((UINT)PIPELINE_STAGE::VS & _PipelineStage)
		CONTEXT->VSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());

	if ((UINT)PIPELINE_STAGE::HS & _PipelineStage)
		CONTEXT->HSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());

	if ((UINT)PIPELINE_STAGE::DS & _PipelineStage)
		CONTEXT->DSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());

	if ((UINT)PIPELINE_STAGE::GS & _PipelineStage)
		CONTEXT->GSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());

	if ((UINT)PIPELINE_STAGE::PS & _PipelineStage)
		CONTEXT->PSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());
}
