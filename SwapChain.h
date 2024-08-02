#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class SwapChain
{
	public:
		SwapChain(HWND hwnd, UINT width, UINT height,RenderSystem* m_system);
		void setFullScreen(bool fullscreen, unsigned int width, unsigned int height);
		void resize(unsigned int width, unsigned int height); //resize the swap chain
		bool present(bool vsync); //present the swap chain
		~SwapChain();

	private: 
		void reloadBuffers(unsigned int width, unsigned int height);
	private:
		IDXGISwapChain* m_swap_chain = nullptr;
		ID3D11RenderTargetView* m_rtv = nullptr;
		ID3D11DepthStencilView* m_dsv = nullptr;
		RenderSystem* m_system = nullptr;
		

	private:
		friend class DeviceContext;

};

