#include "Window.h"


Window* window = nullptr;

Window::Window()
{
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_CREATE:
		// event fired when the window is created
		window->onCreate();
		break;

	case WM_DESTROY:
		// event fired when the window is destroyed
		window->onDestroy();
		::PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	return NULL;
}


bool Window::init()
{
	WNDCLASSEX wc;
	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = NULL;
	wc.lpszClassName = "MyWindowClass";
	wc.lpszMenuName = "";
	wc.style = NULL;
	wc.lpfnWndProc = &WndProc;

	if(!::RegisterClassEx(&wc))//register the class; if fail return false
		return false;

	if (!window)
		window = this;

	//create window
	m_hwnd=::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, "MyWindowClass", "DirectX Application", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768, NULL, NULL, NULL, NULL);

	//creation failed
	if (!m_hwnd)
		return false;

	//show up the window
	::ShowWindow(m_hwnd, SW_SHOW);
	::UpdateWindow(m_hwnd);

	

	//set the flag to indicate the window is initialized and running
	m_is_run = true;
	return true;
}

bool Window::broadcast()
{
	MSG msg;


	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	window->onUpdate();

	Sleep(0);

	return true;
}

bool Window::release()
{

	if (m_hwnd)
	{
		::DestroyWindow(m_hwnd);
		//m_hwnd = NULL;
		return false;
	}	
	return true;
}

bool Window::isRun()
{
	return m_is_run;
}

Window::~Window()
{
}

void Window::onDestroy()
{
	m_is_run = false;
}
