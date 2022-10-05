#include "pch.h"
#include "CDevice.h"

#include "CConstBuffer.h"

CDevice::CDevice()
	: m_hWnd(nullptr)
	, m_pDevice(nullptr)
	, m_pDeviceContext(nullptr)
	, m_pRenderTargetTex(nullptr)
	, m_pRenderTargetView(nullptr)
	, m_pDepthStencilTex(nullptr)
	, m_pDepthStencilView(nullptr)
	, m_pSwapChain(nullptr)
	, m_tViewPort{}
	, m_arrCB{}
{

}

CDevice::~CDevice()
{
	for (UINT i = 0; i < (UINT)CB_TYPE::END; ++i)
	{
		if (nullptr != m_arrCB[i])
			delete m_arrCB[i];
	}
}

int CDevice::init(HWND _hWnd, Vec2 _vResolution)
{
	m_hWnd = _hWnd;
	m_vRenderResolution = _vResolution;

	UINT iDeviceFlag = D3D11_CREATE_DEVICE_DEBUG;
	D3D_FEATURE_LEVEL FeatureLevel = (D3D_FEATURE_LEVEL)0;


	ID3D11Device* pDevice = nullptr;
	HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr
		, iDeviceFlag, nullptr, 0
		, D3D11_SDK_VERSION
		, m_pDevice.GetAddressOf()
		, &FeatureLevel
		, m_pDeviceContext.GetAddressOf());


	// 스왚체인 만들기
	if (FAILED(CreateSwapchain()))
	{
		MessageBox(nullptr, L"스왚체인 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}

	// RenderTarget, DepthStencilTarget 만들기
	if (FAILED(CreateTarget()))
	{
		MessageBox(nullptr, L"타겟 생성 실패", L"Device 초기화 실패", MB_OK);
	}

	// 출력용 렌더 타겟 및 깊이 타겟 지정
	m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());


	// ViewPort
	m_tViewPort.TopLeftX = 0;
	m_tViewPort.TopLeftY = 0;
	m_tViewPort.Width = _vResolution.x;
	m_tViewPort.Height = _vResolution.y;

	m_pDeviceContext->RSSetViewports(1, &m_tViewPort);

	if (FAILED(CreateSampler()))
	{

	}

	// 상수버퍼 생성
	if (FAILED(CreateConstBuffer()))
	{

	}

	return S_OK;
}


int CDevice::CreateSwapchain()
{
	DXGI_SWAP_CHAIN_DESC desc = {};

	desc.OutputWindow = m_hWnd;	// Front Buffer 를 출력시킬 윈도우 핸들
	desc.Windowed = true;		// 윈도우, 전체화면 모드
	desc.BufferCount = 1;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // 이전 프레임 장면을 유지하지 않는다.

	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferDesc.Width = (UINT)m_vRenderResolution.x;
	desc.BufferDesc.Height = (UINT)m_vRenderResolution.y;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	ComPtr<IDXGIDevice> pDXGIDevice = nullptr;
	ComPtr<IDXGIAdapter> pAdapter = nullptr;
	ComPtr<IDXGIFactory> pFactory = nullptr;


	if (FAILED(m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)pDXGIDevice.GetAddressOf())))
	{
		return E_FAIL;
	}

	if (FAILED(pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pAdapter.GetAddressOf())))
	{
		return E_FAIL;
	}

	if (FAILED(pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf())))
	{
		return E_FAIL;
	}

	if (FAILED(pFactory->CreateSwapChain(m_pDevice.Get(), &desc, m_pSwapChain.GetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;
}

int CDevice::CreateTarget()
{
	HRESULT hr = S_OK;
	// RenderTargetTexture 를 스왚체인으로부터 참조하기
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)m_pRenderTargetTex.GetAddressOf());

	// RenderTargetView 만들기
	hr = m_pDevice->CreateRenderTargetView(m_pRenderTargetTex.Get(), nullptr, m_pRenderTargetView.GetAddressOf());


	// DepthStencilTexture
	D3D11_TEXTURE2D_DESC texdesc = {};

	texdesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;

	texdesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	texdesc.CPUAccessFlags = 0;

	texdesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	texdesc.Width = (UINT)m_vRenderResolution.x;
	texdesc.Height = (UINT)m_vRenderResolution.y;
	texdesc.ArraySize = 1;

	texdesc.SampleDesc.Count = 1;
	texdesc.SampleDesc.Quality = 0;

	texdesc.MipLevels = 0;
	texdesc.MiscFlags = 0;

	hr = m_pDevice->CreateTexture2D(&texdesc, nullptr, m_pDepthStencilTex.GetAddressOf());

	// DepthStencilView 제작
	hr = m_pDevice->CreateDepthStencilView(m_pDepthStencilTex.Get(), nullptr, m_pDepthStencilView.GetAddressOf());

	return hr;
}

int CDevice::CreateConstBuffer()
{
	m_arrCB[(UINT)CB_TYPE::TRANSFORM] = new CConstBuffer(CB_TYPE::TRANSFORM);
	m_arrCB[(UINT)CB_TYPE::TRANSFORM]->Create(sizeof(Vec4));


	m_arrCB[(UINT)CB_TYPE::MATERIAL] = new CConstBuffer(CB_TYPE::MATERIAL);
	m_arrCB[(UINT)CB_TYPE::MATERIAL]->Create(sizeof(tMtrlConst));

	return 0;
}

int CDevice::CreateSampler()
{
	HRESULT hr = S_OK;

	// 샘플러 스테이트 생성
	D3D11_SAMPLER_DESC desc = {};

	desc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	desc.Filter = D3D11_FILTER_ANISOTROPIC;

	hr = DEVICE->CreateSamplerState(&desc, m_arrSampler[(UINT)SAMPLER_TYPE::ANISOTROPIC].GetAddressOf());


	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	hr = DEVICE->CreateSamplerState(&desc, m_arrSampler[(UINT)SAMPLER_TYPE::POINT].GetAddressOf());


	// 샘플러 바인딩
	CONTEXT->VSSetSamplers((UINT)SAMPLER_TYPE::ANISOTROPIC, 1, m_arrSampler[(UINT)SAMPLER_TYPE::ANISOTROPIC].GetAddressOf());
	CONTEXT->HSSetSamplers((UINT)SAMPLER_TYPE::ANISOTROPIC, 1, m_arrSampler[(UINT)SAMPLER_TYPE::ANISOTROPIC].GetAddressOf());
	CONTEXT->DSSetSamplers((UINT)SAMPLER_TYPE::ANISOTROPIC, 1, m_arrSampler[(UINT)SAMPLER_TYPE::ANISOTROPIC].GetAddressOf());
	CONTEXT->GSSetSamplers((UINT)SAMPLER_TYPE::ANISOTROPIC, 1, m_arrSampler[(UINT)SAMPLER_TYPE::ANISOTROPIC].GetAddressOf());
	CONTEXT->PSSetSamplers((UINT)SAMPLER_TYPE::ANISOTROPIC, 1, m_arrSampler[(UINT)SAMPLER_TYPE::ANISOTROPIC].GetAddressOf());
	CONTEXT->CSSetSamplers((UINT)SAMPLER_TYPE::ANISOTROPIC, 1, m_arrSampler[(UINT)SAMPLER_TYPE::ANISOTROPIC].GetAddressOf());

	CONTEXT->VSSetSamplers((UINT)SAMPLER_TYPE::POINT, 1, m_arrSampler[(UINT)SAMPLER_TYPE::POINT].GetAddressOf());
	CONTEXT->HSSetSamplers((UINT)SAMPLER_TYPE::POINT, 1, m_arrSampler[(UINT)SAMPLER_TYPE::POINT].GetAddressOf());
	CONTEXT->DSSetSamplers((UINT)SAMPLER_TYPE::POINT, 1, m_arrSampler[(UINT)SAMPLER_TYPE::POINT].GetAddressOf());
	CONTEXT->GSSetSamplers((UINT)SAMPLER_TYPE::POINT, 1, m_arrSampler[(UINT)SAMPLER_TYPE::POINT].GetAddressOf());
	CONTEXT->PSSetSamplers((UINT)SAMPLER_TYPE::POINT, 1, m_arrSampler[(UINT)SAMPLER_TYPE::POINT].GetAddressOf());
	CONTEXT->CSSetSamplers((UINT)SAMPLER_TYPE::POINT, 1, m_arrSampler[(UINT)SAMPLER_TYPE::POINT].GetAddressOf());

	return hr;
}

void CDevice::TargetClear()
{
	float arrColor[4] = { 0.f, 0.f, 0.f, 1.f };
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), arrColor);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}


void CDevice::Present()
{
	m_pSwapChain->Present(0, 0);
}