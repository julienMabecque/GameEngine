#pragma once
#include "Window.h"


class AppWindow: public Window
{
	public:
		AppWindow();
		~AppWindow();
		
		
		// H�rit� via Window
		void onCreate() override;
		void onUpdate() override;
		void onDestroy() override;
};

