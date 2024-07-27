#include "PixelShader.h"
#include "RenderSystem.h"
#include <exception>

PixelShader::PixelShader(const void* shader_byte_code, size_t byte_code_size,RenderSystem* m_system) : m_system(m_system)
{
    if (!SUCCEEDED(m_system->m_d3d_device->CreatePixelShader(shader_byte_code, byte_code_size, nullptr, &m_ps)))
    {
        throw std::exception("PixelShader not created successfully");
    }

}


PixelShader::~PixelShader()
{
    m_ps->Release();
}
