#pragma once
#include <Windows.h>


class Window
{
	public:
		Window();//initailize window
		bool isRun();
		RECT getClientWindowRect();

		//EVENT

		virtual void onCreate();
		virtual void onUpdate();
		virtual void onDestroy();
		virtual void onFocus();
		virtual void onKillFocus();

		~Window();//release window
	private:
		bool broadcast();
	protected:
		HWND m_hwnd;
		bool m_is_run;
		bool m_is_init=false;
};

