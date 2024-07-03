#pragma once
#include <d3d11.h>


class SwapChain
{
	public:
		SwapChain();

		bool init(HWND hwnd, UINT width, UINT height); 

		bool release();


		~SwapChain();

	private:
		IDXGISwapChain* m_swap_chain;
		ID3D11RenderTargetView* m_rtv;
		ID3D11DepthStencilView* m_dsv;
		ID3D11Texture2D* m_depth_stencil_buffer;
		D3D11_VIEWPORT m_viewport;

};

