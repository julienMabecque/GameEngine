#pragma once
#include <d3d11.h>
#include "Prerequisites.h"
#include "RenderSystem.h"
#include "TextureManager.h"
#include "MeshManager.h"
#include "Material.h"

class GraphicsEngine
{
	private:
		GraphicsEngine();// Initialize the graphics engine and directX11
		~GraphicsEngine();// Release the graphics engine ressources and directX11
	public:
		RenderSystem* getRenderSystem();
		TextureManager* getTextureManager();
		MeshManager* getMeshManager();
	public:
		MaterialPtr createMaterial(const wchar_t* vertex_shader_path, const wchar_t* pixel_shader_path);
		MaterialPtr createMaterial(const MaterialPtr& material);
		void setMaterial(const MaterialPtr& material);
		void getVertexMeshLayoutShaderByteCodeAndSize(void** byte_code, size_t* size);
	public:
		static GraphicsEngine* get();
		static void create();
		static void release();
	private:
		RenderSystem* m_render_system = nullptr;
		TextureManager* m_tex_manager = nullptr;
		MeshManager* m_mesh_manager = nullptr;
		static GraphicsEngine* m_engine;

		unsigned char m_mesh_layout_byte_code[1024];
		size_t m_mesh_layout_size = 0;
};

