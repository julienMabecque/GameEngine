#include "VertexShader.h"
#include "RenderSystem.h"
#include <exception>

VertexShader::VertexShader(const void* shader_byte_code, size_t byte_code_size,RenderSystem* m_system) : m_system(m_system)
{
    if (!SUCCEEDED(m_system->m_d3d_device->CreateVertexShader(shader_byte_code, byte_code_size, nullptr, &m_vs)))
    {
		throw std::exception("VertexShader not created successfully");
	}
}

VertexShader::~VertexShader()
{
    m_vs->Release();
}

