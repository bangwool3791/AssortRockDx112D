#include "pch.h"
#include "CGraphicsShader.h"

#include "CPathMgr.h"
#include "CDevice.h"
#include "CResMgr.h"

CGraphicsShader::CGraphicsShader()
	:CShader(RES_TYPE::GRAPHICS_SHADER)
	, m_eTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{

}

CGraphicsShader::~CGraphicsShader()
{

}

void CGraphicsShader::CreateVertexShader(const wstring& _strRelativePath, const string& _strFuncName)
{
	HRESULT hr = S_OK;

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath() +  _strRelativePath;

	hr = D3DCompileFromFile(strFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), "vs_5_0", 0, 0, m_VSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf());

	assert(!FAILED(hr));

	hr = DEVICE->CreateVertexShader(m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize(), nullptr, m_VS.GetAddressOf());

	assert(!FAILED(hr));

	const vector<D3D11_INPUT_ELEMENT_DESC>& vecLayout = CResMgr::GetInst()->GetInputLayoutInfo();

	hr = DEVICE->CreateInputLayout(vecLayout.data(), (UINT)vecLayout.size()
		, m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize()
		, m_Layout.GetAddressOf());

	assert(!FAILED(hr));
	
}

void CGraphicsShader::CreatePixelShader(const wstring& _strRelativePath, const string& _strFuncName)
{
	HRESULT hr = S_OK;

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + _strRelativePath;

	hr = D3DCompileFromFile(strFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), "ps_5_0", 0, 0, m_PSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf());

	ID3DBlob* temp = m_ErrBlob.Get();
	assert(!FAILED(hr));

	hr = DEVICE->CreatePixelShader(m_PSBlob->GetBufferPointer(), m_PSBlob->GetBufferSize(), nullptr, m_PS.GetAddressOf());

	assert(!FAILED(hr));
}

void CGraphicsShader::UpdateDate()
{
	CONTEXT->VSSetShader(m_VS.Get(), 0, 0);
	CONTEXT->PSSetShader(m_PS.Get(), 0, 0);

	ComPtr<ID3D11RasterizerState> RSState = CDevice::GetInst()->GetRasterizerState(m_eRSType);
	CONTEXT->RSSetState(RSState.Get());
	
	ComPtr<ID3D11DepthStencilState> DSState = CDevice::GetInst()->GetDepthStencilState(m_eDSType);
	CONTEXT->OMSetDepthStencilState(DSState.Get(), 0);

	ComPtr<ID3D11BlendState> BSState = CDevice::GetInst()->GetBlendState(m_eBSType);
	CONTEXT->OMSetBlendState(BSState.Get(), nullptr, 0xffffff);

	CONTEXT->IASetInputLayout(m_Layout.Get());
	CONTEXT->IASetPrimitiveTopology(m_eTopology);
}