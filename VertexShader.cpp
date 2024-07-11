#include "VertexShader.h"
#include "GraphicsEngine.h"
#include <iostream>

VertexShader::VertexShader()
{
}

void VertexShader::release()
{
    if (m_vs) // V�rifiez si m_vs n'est pas nullptr avant de l'utiliser
    {
        m_vs->Release();
        m_vs = nullptr; // Bonne pratique : mettre le pointeur � nullptr apr�s la lib�ration
        std::cout << "VertexShader released successfully." << std::endl;
    }
    else
    {
        std::cout << "VertexShader::release called, but m_vs is already nullptr." << std::endl;
    }
    delete this;
   
}

VertexShader::~VertexShader()
{
}

bool VertexShader::init(const void* shader_byte_code, size_t byte_code_size)
{
    if (!SUCCEEDED(GraphicsEngine::get()->m_d3d_device->CreateVertexShader(shader_byte_code, byte_code_size, nullptr, &m_vs)))
    {
		return false;
	}
    return true;
}
