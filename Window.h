#pragma once
#include <Windows.h>


class Window
{
public:
	Window();

	bool init();

	bool release();

	~Window();

	//EVENT

	virtual void onCreate() = 0;
	virtual void onUpdate() = 0;
	virtual void onDestroy() = 0;
	
protected:
	HWND m_hwnd;
};

