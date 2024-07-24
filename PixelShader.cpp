#include "PixelShader.h"
#include "RenderSystem.h"
#include <exception>

PixelShader::PixelShader(RenderSystem* m_system) : m_system(m_system)
{
}

void PixelShader::release()
{
    m_ps->Release();
    delete this;

}


bool PixelShader::init(const void* shader_byte_code, size_t byte_code_size)
{
    if (!SUCCEEDED(m_system->m_d3d_device->CreatePixelShader(shader_byte_code, byte_code_size, nullptr, &m_ps)))
    {
        return false;
    }
    return true;
}


PixelShader::~PixelShader()
{
}
