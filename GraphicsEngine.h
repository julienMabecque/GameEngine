#pragma once
#include <d3d11.h>
#include "Prerequisites.h"
#include "RenderSystem.h"

class GraphicsEngine
{
	public:
		GraphicsEngine();
		// Initialize the graphics engine and directX11
		bool init();
		// Release the graphics engine ressources and directX11
		bool release();
		RenderSystem* getRenderSystem();
		~GraphicsEngine();
		
	public:
		static GraphicsEngine* get();
	private:
		RenderSystem* m_render_system = nullptr;
};

