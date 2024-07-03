#pragma once
#include <Windows.h>


class Window
{
public:
	Window();

	bool init();

	bool broadcast();

	bool release();

	bool isRun();

	~Window();

	RECT getClientWindowRect();
	void setHWND(HWND hwnd);

	//EVENT

	virtual void onCreate() = 0;
	virtual void onUpdate() = 0;
	virtual void onDestroy();
	
protected:
	HWND m_hwnd;
	bool m_is_run;
};

