#include "AppWindow.h"
#include <Windows.h>
#include "Vector3D.h"
#include "Matrix4x4.h"
#include "InputSystem.h"

struct vertex
{
	Vector3D position;
	Vector3D color;
	Vector3D color1;
	
};

_declspec(align(16)) 
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	unsigned int m_time;

};

AppWindow::AppWindow()
{
}
void AppWindow::updateQuadPosition()
{
	constant cc;
	cc.m_time = ::GetTickCount();//use ptetre GetTickCount64

	m_delta_pos+=m_delta_time/4.0f;

	if (m_delta_pos >= 1.0f)
	{
		m_delta_pos = 0;
	}

	Matrix4x4 temp;

	//cc.m_world.setTranslation(Vector3D::lerp(Vector3D(-2,-2,0),Vector3D(2,2,0), m_delta_pos));

	m_delta_scale += m_delta_time / 0.55f;

	//cc.m_world.setScale(Vector3D::lerp(Vector3D(0.5, 0.5, 0), Vector3D(100.0f, 100.0f, 0), (sin(m_delta_scale)+1.0f)/2.0f));

	//temp.setTranslation(Vector3D::lerp(Vector3D(-1.5f, -1.5f, 0), Vector3D(1.5f, 1.5f, 0), m_delta_pos));

	//cc.m_world *= temp;
	cc.m_world.setScale(Vector3D(m_scale_cube, m_scale_cube, m_scale_cube));

	temp.setIdentity();
	temp.setRotationZ(0.0f);
	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationY(m_rot_y);
	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationX(m_rot_x);
	cc.m_world *= temp;


	cc.m_view.setIdentity();
	cc.m_proj.setOrthoLH
	(
		this->getClientWindowRect().right - this->getClientWindowRect().left,//400.0f
		this->getClientWindowRect().bottom - this->getClientWindowRect().top,//400.0f
		-4.0f,
		4.0f
	);

	m_cb->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);

}


AppWindow::~AppWindow()
{
}


void AppWindow::onCreate()
{
	Window::onCreate();

	InputSystem::get()->addListener(this); 

	GraphicsEngine::get()->init();

	m_swap_chain = GraphicsEngine::get()->createSwapChain();

	RECT rc = this->getClientWindowRect();
	m_swap_chain->init(this->m_hwnd, rc.right-rc.left,rc.bottom-rc.top);

	vertex vertex_list[] =
	{
		//front face
		{Vector3D (-0.5f,-0.5f,-0.5f),Vector3D(0,0,0),Vector3D(0,1,0)},
		{Vector3D (-0.5f, 0.5f,-0.5f),Vector3D(1,1,0),Vector3D(0,1,1)},
		{Vector3D ( 0.5f, 0.5f,-0.5f),Vector3D(0,0,1),Vector3D(1,0,0)},
		{Vector3D ( 0.5f,-0.5f,-0.5f),Vector3D(1,1,1),Vector3D(0,0,1)},

		//back face
		{Vector3D( 0.5f,-0.5f,0.5f),Vector3D(0,0,0),Vector3D(0,1,0)},
		{Vector3D( 0.5f, 0.5f,0.5f),Vector3D(1,1,0),Vector3D(0,1,1)},
		{Vector3D(-0.5f, 0.5f,0.5f),Vector3D(0,0,1),Vector3D(1,0,0)},
		{Vector3D(-0.5f,-0.5f,0.5f),Vector3D(1,1,1),Vector3D(0,0,1)}
	};
	

	m_vb=GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(vertex_list);

	unsigned int index_list[] =
	{
		//front side
		0,1,2,
		2,3,0,
		//back side
		4,5,6,
		6,7,4,
		//top side
		1,6,5,
		5,2,1,
		//bottom side
		7,0,3,
		3,4,7,
		//right side
		3,2,5,
		5,4,3,
		//left side
		7,6,1,
		1,0,7
	};


	m_ib=GraphicsEngine::get()->createIndexBuffer();
	UINT size_index_list = ARRAYSIZE(index_list);
	m_ib->load(index_list, size_index_list);





	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	
	m_vs=GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
	m_vb->load(vertex_list, sizeof(vertex), size_list, shader_byte_code, size_shader);

	GraphicsEngine::get()->releaseCompiledShader();

	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();

	constant cc;
	cc.m_time = 0;

	m_cb = GraphicsEngine::get()->createConstantBuffer();
	m_cb->load(&cc, sizeof(constant));


}

void AppWindow::onUpdate()
{
	Window::onUpdate();


	InputSystem::get()->update();

	//Clear the render target
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0,0.3f,0.4f,1);
	//Set viewport of render target in which we have to draw	
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);
	//Set default shader in the graphics pipeline

	
	updateQuadPosition();


	//Set the constant buffer to the graphics pipeline
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);
	//Set the shader
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(m_ps);

	//Set the vertex buffer
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
	//Set the incices of the triangle to draw
	GraphicsEngine::get()->getImmediateDeviceContext()->setIndexBuffer(m_ib);


	//Draw the vertex buffer
	GraphicsEngine::get()->getImmediateDeviceContext()->drawIndexedTriangleList(m_ib->getSizeIndexList(),0,0);
	m_swap_chain->present(true);

	m_old_delta = m_new_delta;
	m_new_delta = ::GetTickCount();
	m_delta_time = (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.0f):0;
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_vb->release();
	m_swap_chain->release();
	m_vs->release();
	m_ps->release();
	m_ib->release();
	m_cb->release();
	GraphicsEngine::get()->release();
}

void AppWindow::onFocus()
{
	InputSystem::get()->addListener(this);
}

void AppWindow::onKillFocus()
{
	InputSystem::get()->removeListener(this);
}

void AppWindow::onKeyDown(int key)
{
	if (key == 'W')
	{
		m_rot_x += 3.14f * m_delta_time;
	}
	else if (key == 'S')
	{
		m_rot_x -= 3.14f * m_delta_time;
	}
	else if (key == 'A')
	{
		m_rot_y += 3.14f * m_delta_time;
	}
	else if (key == 'D')
	{
		m_rot_y -= 3.14f * m_delta_time;
	}
}

void AppWindow::onKeyUp(int key)
{
	
}

void AppWindow::onMouseMove(const Point& delta_mouse_pos)
{
	m_rot_x -= delta_mouse_pos.m_y*m_delta_time;
	m_rot_y -= delta_mouse_pos.m_x*m_delta_time;
}

void AppWindow::onLeftMouseDown(const Point& mouse_pos)
{
	m_scale_cube = 0.5f;
}

void AppWindow::onLeftMouseUp(const Point& mouse_pos)
{
	m_scale_cube = 1.0f;
}

void AppWindow::onRightMouseDown(const Point& mouse_pos)
{
	m_scale_cube = 2.0f;
}

void AppWindow::onRightMouseUp(const Point& mouse_pos)
{
	m_scale_cube = 100.0f;
}
