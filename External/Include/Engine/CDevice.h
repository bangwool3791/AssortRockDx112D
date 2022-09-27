#pragma once
#include <wrl.h>

class CDevice
{
	SINGLE(CDevice);

private :
	HWND m_hWnd;
	
	ComPtr<ID3D11Device> m_pDevice;
	ComPtr<ID3D11DeviceContext> m_pDeviceContext;

	ComPtr<ID3D11Texture2D> m_pRenderTargetTex;
	//A rendertarget is a resource that can be written by the output-merger stage at the end of a render pass. 
	//Each render-target should also have a corresponding depth-stencil view.
	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;

	ComPtr<ID3D11Texture2D>		m_pDepthStencilTex;
	ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;
	
	ComPtr<IDXGISwapChain>			m_pSwapChain;
	D3D11_VIEWPORT			m_tViewPort;

	Vec2					m_vRenderResolution;

public :
	int init(HWND _hWnd, Vec2 _vResolution);
	void Present();
	void TargetClear();
private:
	int CreateSwapchain();
	int CreateTarget();
public :
	ID3D11Device* GetDevice() { return m_pDevice.Get(); }
	ID3D11DeviceContext* GetContext() { return m_pDeviceContext.Get(); }
};

