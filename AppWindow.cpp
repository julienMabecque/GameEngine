#include "AppWindow.h"
#include <Windows.h>
#include "Vector3D.h"
#include "Vector2D.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include "Mesh.h"


struct vertex
{
	Vector3D position;
	Vector2D texcoord;
	
};

_declspec(align(16)) 
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	Vector4D m_light_direction;
	Vector4D m_camera_position;
};

AppWindow::AppWindow()
{
}

void AppWindow::update()
{
	updateCamera();
	updateModel();
	updateSkyBox();
}

void AppWindow::updateModel()
{
	constant cc;

	Matrix4x4 m_light_rot_matrix;
	m_light_rot_matrix.setIdentity();
	m_light_rot_matrix.setRotationY(m_light_rot_y);

	m_light_rot_y += 0.785f * m_delta_time;

	cc.m_world.setIdentity();
	cc.m_view = m_view_cam;
	cc.m_proj = m_proj_cam;
	cc.m_camera_position = m_world_cam.getTranslation();
	cc.m_light_direction = m_light_rot_matrix.getZDirection();
	m_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);
}

void AppWindow::updateCamera()
{
	Matrix4x4 world_cam, temp;
	world_cam.setIdentity();

	temp.setIdentity();
	temp.setRotationX(m_rot_x);
	world_cam *= temp;

	temp.setIdentity();
	temp.setRotationY(m_rot_y);
	world_cam *= temp;

	Vector3D new_pos = m_world_cam.getTranslation() + world_cam.getZDirection() * (m_forward * 0.05f);

	new_pos = new_pos + world_cam.getXDirection() * (m_rightward * 0.05f);

	world_cam.setTranslation(new_pos);

	m_world_cam = world_cam;
	world_cam.inverse();


	m_view_cam = world_cam;

	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	m_proj_cam.setPerspectiveFovLH(1.57f, ((float)width / (float)height), 0.1f, 100.0f);
}

void AppWindow::updateSkyBox()
{
	constant cc;

	cc.m_world.setIdentity();
	cc.m_world.setScale(Vector3D(100.0f, 100.0f, 100.0f));
	cc.m_world.setTranslation(m_world_cam.getTranslation());
	cc.m_view = m_view_cam;
	cc.m_proj = m_proj_cam;

	m_sky_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);
}

void AppWindow::drawMesh(const MeshPtr& mesh, const VertexShaderPtr& vs, const PixelShaderPtr& ps, const ConstantBufferPtr& cb, const TexturePtr& tex)
{
	//Set the constant buffer to the graphics pipeline
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(vs, cb);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(ps, cb);
	//Set the shader
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(vs);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(ps);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(ps, tex);

	//Set the vertex buffer
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(mesh->getVertexBuffer());
	//Set the incices of the triangle to draw
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(mesh->getIndexBuffer());


	//Draw the vertex buffer
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(mesh->getIndexBuffer()->getSizeIndexList(), 0, 0);
}


AppWindow::~AppWindow()
{
}





void AppWindow::onCreate()
{
	Window::onCreate();

	InputSystem::get()->addListener(this); 
	InputSystem::get()->showCursor(false);

	m_wood_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\brick.png");
	m_sky_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\sky.jpg");

	m_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\suzanne.obj");
	m_sky_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\sphere.obj");

	RECT rc = this->getClientWindowRect();
	m_swap_chain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->m_hwnd, rc.right-rc.left,rc.bottom-rc.top);

	

	m_world_cam.setTranslation(Vector3D(0, 0, -1));


	Vector3D position_list[] =
	{
		{Vector3D(-0.5f,-0.5f,-0.5f)},
		{Vector3D(-0.5f, 0.5f,-0.5f)},
		{Vector3D(0.5f, 0.5f,-0.5f)},
		{Vector3D(0.5f,-0.5f,-0.5f)},

		//back face
		{Vector3D(0.5f,-0.5f,0.5f)},
		{Vector3D(0.5f, 0.5f,0.5f)},
		{Vector3D(-0.5f, 0.5f,0.5f)},
		{Vector3D(-0.5f,-0.5f,0.5f)}

	};

	Vector2D texcoord_list[] =
	{
		{Vector2D(0.0f,0.0f)},
		{Vector2D(0.0f,1.0f)},
		{Vector2D(1.0f,0.0f)},
		{Vector2D(1.0f,1.0f)}
	};


	vertex vertex_list[] =
	{
		//front face
		{position_list[0],texcoord_list[1]},
		{position_list[1],texcoord_list[0]},
		{position_list[2],texcoord_list[2]},
		{position_list[3],texcoord_list[3]},

		//back face
		{position_list[4],texcoord_list[1]},
		{position_list[5],texcoord_list[0]},
		{position_list[6],texcoord_list[2]},
		{position_list[7],texcoord_list[3]},

		{position_list[1],texcoord_list[1]},
		{position_list[6],texcoord_list[0]},
		{position_list[5],texcoord_list[2]},
		{position_list[2],texcoord_list[3]},

		{position_list[7],texcoord_list[1]},
		{position_list[0],texcoord_list[0]},
		{position_list[3],texcoord_list[2]},
		{position_list[4],texcoord_list[3]},

		{position_list[3],texcoord_list[1]},
		{position_list[2],texcoord_list[0]},
		{position_list[5],texcoord_list[2]},
		{position_list[4],texcoord_list[3]},

		{position_list[7],texcoord_list[1]},
		{position_list[6],texcoord_list[0]},
		{position_list[1],texcoord_list[2]},
		{position_list[0],texcoord_list[3]}


	};
	

	
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
		8,9,10,
		10,11,8,
		//bottom side
		12,13,14,
		14,15,12,
		//right side
		16,17,18,
		18,19,16,
		//left side
		20,21,22,
		22,23,20
	};

	UINT size_index_list = ARRAYSIZE(index_list);
	m_ib=GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(index_list, size_index_list);
	





	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	
	m_vs=GraphicsEngine::get()->getRenderSystem()->createVertexShader(shader_byte_code, size_shader);
	m_vb = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(vertex_list, sizeof(vertex), size_list, shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();


	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"SkyBoxShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_sky_ps = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	constant cc;

	m_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));
	m_sky_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));


}

void AppWindow::onUpdate()
{
	Window::onUpdate();


	InputSystem::get()->update();

	//Clear the render target
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0,0.3f,0.4f,1);
	//Set viewport of render target in which we have to draw	
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);
	//Set default shader in the graphics pipeline

	//compute transform matrices
	update();

	//render model
	GraphicsEngine::get()->getRenderSystem()->setRasterizerState(false);
	drawMesh(m_mesh, m_vs, m_ps, m_cb, m_wood_tex);

	//render skybox
	GraphicsEngine::get()->getRenderSystem()->setRasterizerState(true);
	drawMesh(m_sky_mesh, m_vs, m_sky_ps, m_sky_cb, m_sky_tex);

	m_swap_chain->present(true);

	m_old_delta = m_new_delta;
	m_new_delta = ::GetTickCount();
	m_delta_time = (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.0f):0;
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
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
		//m_rot_x += 3.14f * m_delta_time;
		m_forward = 1.0f;
	}
	else if (key == 'S')
	{
		//m_rot_x -= 3.14f * m_delta_time;
		m_forward = -1.0f;
	}
	else if (key == 'A')
	{
		//m_rot_y += 3.14f * m_delta_time;
		m_rightward = -1.0f;
	}
	else if (key == 'D')
	{
		//m_rot_y -= 3.14f * m_delta_time;
		m_rightward = 1.0f;
	}
}

void AppWindow::onKeyUp(int key)
{
	m_forward = 0.0f;
	m_rightward = 0.0f;
	
}

void AppWindow::onMouseMove(const Point& mouse_pos)
{
	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);


	m_rot_x += (mouse_pos.m_y-(height/2.0f))*m_delta_time*0.1f;
	m_rot_y += (mouse_pos.m_x-(width / 2.0f)) *m_delta_time*0.1f;

	InputSystem::get()->setCursorPosition(Point(width / 2, height / 2));
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
