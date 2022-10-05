#pragma once

class CConstBuffer;

class CDevice
{
	SINGLE(CDevice);
private:
	HWND							m_hWnd;

	ComPtr<ID3D11Device>			m_pDevice;				// GPU �޸� ����
	ComPtr<ID3D11DeviceContext>		m_pDeviceContext;		// GPU Rendering

	ComPtr<ID3D11Texture2D>			m_pRenderTargetTex;		// Render Target
	ComPtr<ID3D11RenderTargetView>	m_pRenderTargetView;	// RenderTarget �� ����

	ComPtr<ID3D11Texture2D>			m_pDepthStencilTex;		// ���� ���� Ÿ��
	ComPtr<ID3D11DepthStencilView>	m_pDepthStencilView;	// DSTex �� ����	

	ComPtr<IDXGISwapChain>			m_pSwapChain;			// RenderTarget(FrontBuffer, BackBuffer) �� ���� �� ���� ��ü ����
	D3D11_VIEWPORT					m_tViewPort;			// ����۸� �����쿡 �׸� ����(��ġ, ũ��) ����

	Vec2							m_vRenderResolution;	// ������ ���� �ػ�

	ComPtr<ID3D11SamplerState>		m_arrSampler[(UINT)SAMPLER_TYPE::END];

	CConstBuffer* m_arrCB[(UINT)CB_TYPE::END];



public:
	int init(HWND _hWnd, Vec2 _vResolution);
	void TargetClear();
	void Present();

private:
	int CreateSwapchain();
	int CreateTarget();
	int CreateConstBuffer();
	int CreateSampler();

public:
	ID3D11Device* GetDevice() { return m_pDevice.Get(); }
	ID3D11DeviceContext* GetContext() { return m_pDeviceContext.Get(); }
	CConstBuffer* GetConstBuffer(CB_TYPE _eType) { return m_arrCB[(UINT)_eType]; }
};

