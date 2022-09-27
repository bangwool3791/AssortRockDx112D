#include "pch.h"
#include "CDevice.h"

CDevice::CDevice()
    :m_hWnd{}
    ,m_pDevice{nullptr}
    , m_pDeviceContext{ nullptr }
    , m_pRenderTargetTex{ nullptr }
    , m_pRenderTargetView{ nullptr }
    , m_pDepthStencilTex{ nullptr }
    , m_pDepthStencilView{ nullptr }
    , m_pSwapChain{ nullptr }
    , m_tViewPort{}
{

}

CDevice::~CDevice()
{
}

int CDevice::init(HWND _hWnd, Vec2 _vResolution)
{
    m_hWnd = _hWnd;
    m_vRenderResolution = _vResolution;

    UINT iDeviceFlag = D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL FeatureLevel = (D3D_FEATURE_LEVEL)0;

    HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr
        , iDeviceFlag, nullptr, 0
        , D3D11_SDK_VERSION
        , m_pDevice.GetAddressOf()
        , &FeatureLevel
        , m_pDeviceContext.GetAddressOf());

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

    m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());

    m_tViewPort.TopLeftX = 0;
    m_tViewPort.TopLeftY = 0;
    m_tViewPort.Width = _vResolution.x;
    m_tViewPort.Height = _vResolution.y;

    m_pDeviceContext->RSSetViewports(1, &m_tViewPort);

    return S_OK;
}

void CDevice::Present()
{
    m_pSwapChain->Present(0, 0);
}

int CDevice::CreateSwapchain()
{
    DXGI_SWAP_CHAIN_DESC desc = {};

    desc.OutputWindow = m_hWnd;
    desc.Windowed = true;
    desc.BufferCount = 1;
    desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

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

    IDXGIDevice* pDXGIDevice = nullptr;
    IDXGIAdapter* pAdapter = nullptr;
    IDXGIFactory* pFactory = nullptr;

    if (FAILED(m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDXGIDevice)))
    {
        return E_FAIL;
    }

    if (FAILED(pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pAdapter)))
    {
        return E_FAIL;
    }

    if (FAILED(pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory)))
    {
        return E_FAIL;
    }

    if (FAILED(pFactory->CreateSwapChain(m_pDevice.Get(), &desc, m_pSwapChain.GetAddressOf())))
    {
        return E_FAIL;
    }

    pDXGIDevice->Release();
    pAdapter->Release();
    pFactory->Release();

    return S_OK;
}

int CDevice::CreateTarget()
{
    HRESULT hr = S_OK;

    hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)m_pRenderTargetTex.GetAddressOf());

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

void CDevice::TargetClear()
{
    float arrColor[4]{ 0.f, 0.f, 0.f, 1.f };
    m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), arrColor);
    m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}
