#pragma once
#include <d3d11.h>
#include "Prerequisites.h"
#include "RenderSystem.h"

class GraphicsEngine
{
	private:
		GraphicsEngine();// Initialize the graphics engine and directX11
		~GraphicsEngine();// Release the graphics engine ressources and directX11
	public:
		RenderSystem* getRenderSystem();
	public:
		static GraphicsEngine* get();
		static void create();
		static void release();
	private:
		RenderSystem* m_render_system = nullptr;
		static GraphicsEngine* m_engine;
};

