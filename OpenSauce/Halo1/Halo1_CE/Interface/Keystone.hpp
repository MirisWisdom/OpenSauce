/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

namespace Yelo
{
	namespace Keystone
	{
		void Initialize();
		void Dispose();

		wcstring ChatLogName();
		void* MainWindow();

		// Keystone - Get Window
		// [keystone] - Parent Window ptr (can be NULL?)
		// [child] - name of the child window
		void* GetWindow(void* keystone, wcstring child);

		void WindowRelease(void* handle);

		// Keystone Window - Get Control By ID
		// [window] - parent of the control
		// [id] - name of the control
		void* WindowGetControlByID(void* window, wcstring id);

		LRESULT ControlSendMessage(void* control, uint32 msg, WPARAM wParam, LPARAM lParam);
	};
};
#endif