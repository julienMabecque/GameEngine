#pragma once
#include "Window.h"


class AppWindow: public Window
{
	public:
		AppWindow();
		~AppWindow();
		
		
		// Hérité via Window
		void onCreate() override;
		void onUpdate() override;
		void onDestroy() override;
};

