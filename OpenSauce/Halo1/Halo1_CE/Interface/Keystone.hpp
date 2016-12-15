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

		// should only be called by the unhandled exception filter
		void ReleaseKeystone();

#pragma region Message Pump
		/// <summary>	The windows message handler interface. </summary>
		class i_windows_message_handler
			abstract
		{
		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Handles the message described by message. </summary>
			///
			/// <param name="message">	   	The windows message. </param>
			///
			/// <returns>	true if the message was handled, false if not. </returns>
			virtual void HandleMessage(const MSG* message) = 0;
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Attaches a windows message handler to the message pump. </summary>
		///
		/// <param name="handler">	[in] If non-null, the handler to attach. </param>
		void AttachWindowsMessageHandler(i_windows_message_handler* handler);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Detaches a windows message handler from the message pump. </summary>
		///
		/// <param name="handler">	[in] If non-null, the handler to detach. </param>
		void DetachWindowsMessageHandler(i_windows_message_handler* handler);
#pragma endregion
	};
};
#endif