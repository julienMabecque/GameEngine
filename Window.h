#pragma once
#include <Windows.h>


class Window
{
public:
	Window();

	//initailize window
	bool init();
	bool broadcast();
	//release window
	bool release();
	bool isRun();

	

	RECT getClientWindowRect();
	void setHWND(HWND hwnd);

	//EVENT

	virtual void onCreate();
	virtual void onUpdate();
	virtual void onDestroy();
	virtual void onFocus();
	virtual void onKillFocus();

	~Window();

protected:
	HWND m_hwnd;
	bool m_is_run;
};

