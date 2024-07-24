#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class SwapChain
{
	public:
		SwapChain(RenderSystem* m_system);

		bool init(HWND hwnd, UINT width, UINT height); //initialize the swap chain

		bool present(bool vsync); //present the swap chain

		bool release();


		~SwapChain();

	private:
		IDXGISwapChain* m_swap_chain;
		ID3D11RenderTargetView* m_rtv;
		ID3D11DepthStencilView* m_dsv;// verifier ou le mettre ! check vidéo
		RenderSystem* m_system = nullptr;
		

	private:
		friend class GraphicsEngine;
		friend class DeviceContext;

};

