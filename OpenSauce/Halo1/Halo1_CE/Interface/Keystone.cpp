/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/Keystone.hpp"
#if !PLATFORM_IS_DEDI

#include "Memory/MemoryInterface.hpp"
#include "Settings/Settings.hpp"

namespace Yelo
{
	namespace Keystone
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_INTERFACE
#define __EL_INCLUDE_FILE_ID	__EL_INTERFACE_KEYSTONE
#include "Memory/_EngineLayout.inl"

		uint32 PLATFORM_API HandleMessage(void* arg0, HANDLE window_handle, void* arg2, const MSG* message);

		void OnChatAddString(wcstring string); // forward declare

		void SendMessageUpdateEAX()
		{
			static const uintptr_t SendMessageCallAddr = GET_FUNC_PTR(KEYSTONE_CHATLOG_ADD_ITEM_SEND_MESSAGE);

			wcstring SendMessageStrAddr;
			__asm mov SendMessageStrAddr, eax;

			OnChatAddString(SendMessageStrAddr);

			__asm push SendMessageStrAddr;
			__asm call SendMessageCallAddr;
		}

		void SendMessageUpdateECX()
		{
			static const uintptr_t SendMessageCallAddr = GET_FUNC_PTR(KEYSTONE_CHATLOG_ADD_ITEM_SEND_MESSAGE);

			wcstring SendMessageStrAddr;
			__asm mov SendMessageStrAddr, ecx;

			OnChatAddString(SendMessageStrAddr);

			__asm push SendMessageStrAddr;
			__asm call SendMessageCallAddr;
		}

		void SendMessageUpdateEDX()
		{
			static const uintptr_t SendMessageCallAddr = GET_FUNC_PTR(KEYSTONE_CHATLOG_ADD_ITEM_SEND_MESSAGE);

			wcstring SendMessageStrAddr;
			__asm mov SendMessageStrAddr, edx;

			OnChatAddString(SendMessageStrAddr);

			__asm push SendMessageStrAddr;
			__asm call SendMessageCallAddr;
		}

		void SendMessageInitialize()
		{
			// TODO: clean up this shitty macro to use WriteRelativeCall or something
#define CALL_SENDMSG_HOOK(reg, ce_addr)						\
	call = Enums::_x86_opcode_call_near;					\
	rel_call = _SendMessageUpdatePtr##reg - (ce_addr + 5);	\
	Memory::WriteMemory( CAST_PTR(void*,ce_addr),&call,1);	\
	Memory::WriteMemory( CAST_PTR(void*,ce_addr+1),&rel_call,4);

			byte call;
			uintptr_t rel_call;

#pragma warning( push )
#pragma warning( disable : 4311 ) // bitching about pointer truncation

			uintptr_t _SendMessageUpdatePtrEAX = CAST_PTR(uintptr_t, &SendMessageUpdateEAX);
			uintptr_t _SendMessageUpdatePtrECX = CAST_PTR(uintptr_t, &SendMessageUpdateECX);
			uintptr_t _SendMessageUpdatePtrEDX = CAST_PTR(uintptr_t, &SendMessageUpdateEDX);

			DOC_TODO_DEBUG("Update these if you change Client platform version")
#if PLATFORM_VERSION == 0x1080
			CALL_SENDMSG_HOOK(EDX, 0x45D1EA);
			CALL_SENDMSG_HOOK(EAX, 0x461229);
			CALL_SENDMSG_HOOK(EDX, 0x461332);
			CALL_SENDMSG_HOOK(EDX, 0x4614A0);
			CALL_SENDMSG_HOOK(EDX, 0x4615A2);
			CALL_SENDMSG_HOOK(ECX, 0x46187C);
			CALL_SENDMSG_HOOK(ECX, 0x4AE674);
			CALL_SENDMSG_HOOK(EDX, 0x4AE6F3);
			CALL_SENDMSG_HOOK(ECX, 0x4AE744);
#elif PLATFORM_VERSION == 0x1090
			CALL_SENDMSG_HOOK(EDX, 0x45D1FA);
			CALL_SENDMSG_HOOK(EAX, 0x461239);
			CALL_SENDMSG_HOOK(EDX, 0x461342);
			CALL_SENDMSG_HOOK(EDX, 0x4614B0);
			CALL_SENDMSG_HOOK(EDX, 0x4615B2);
			CALL_SENDMSG_HOOK(ECX, 0x46188C);
			CALL_SENDMSG_HOOK(ECX, 0x4AE6A4);
			CALL_SENDMSG_HOOK(EDX, 0x4AE723);
			CALL_SENDMSG_HOOK(ECX, 0x4AE774);
#elif PLATFORM_VERSION == 0x10A0
			CALL_SENDMSG_HOOK(EDX, 0x45CFEA);
			CALL_SENDMSG_HOOK(EAX, 0x461029);
			CALL_SENDMSG_HOOK(EDX, 0x461132);
			CALL_SENDMSG_HOOK(EDX, 0x4612A0);
			CALL_SENDMSG_HOOK(EDX, 0x4613A2);
			CALL_SENDMSG_HOOK(ECX, 0x46167C);
			CALL_SENDMSG_HOOK(ECX, 0x4AE474);
			CALL_SENDMSG_HOOK(EDX, 0x4AE4F3);
			CALL_SENDMSG_HOOK(ECX, 0x4AE544);
#endif

#pragma warning( pop )
		}

		struct keystone_globals_data {
			// chat that appears in the chatlog log file
			cstring chatlog_log;
			FILE* log;
		}keystone_globals;


		wcstring ChatLogName()	PTR_IMP_GET2(ChatLogName);
		void* MainWindow()		PTR_IMP_GET(MainWindow);

		void Initialize()
		{
			keystone_globals.chatlog_log = Settings::K_CHAT_LOG_FILENAME;
			keystone_globals.log = Settings::CreateReport(keystone_globals.chatlog_log, true, true);

			if(keystone_globals.log != nullptr)
				SendMessageInitialize();
			
			Memory::WriteRelativeCall(&HandleMessage, GET_FUNC_VPTR(KS_TRANSLATEACCELERATOR_CALL), true);
			CAST_PTR(byte*, GET_FUNC_VPTR(KS_TRANSLATEACCELERATOR_CALL))[5] = Enums::_x86_opcode_nop;
		}

		void Dispose()
		{
			if(keystone_globals.log != nullptr)
				fclose(keystone_globals.log);
		}

		static void WriteFirstLine()
		{
			static char format[] = "\n%s\n";

			tm* newtime;
			time_t aclock;	time( &aclock ); // Get time in seconds
			newtime = localtime( &aclock ); // Convert time to struct tm form

			fprintf(keystone_globals.log, format, asctime(newtime));
			fflush(keystone_globals.log);
		}

		// If the chat log fails to open, the system isn't initialize,
		// and this is never called
		void OnChatAddString(wcstring string)
		{
			static char format[] = "%s%s%c";

			static bool first_line = true;
			if(first_line)
			{
				first_line = false;
				WriteFirstLine();
			}

			tag_string time_str;
			GetTimeStampString(time_str);

			char time_buffer[16];
			memset(time_buffer, 0, sizeof(time_buffer));

			strncpy(time_buffer, time_str+11, 9); // copy the time only, leave out the date and year

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			wstring_to_string_lazy(buffer, sizeof(buffer), string);
			fprintf(keystone_globals.log, format, time_buffer, buffer, '\n');
			fflush(keystone_globals.log);
		}


		API_FUNC_NAKED void* GetWindow(void* keystone, wcstring child)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(KS_GETWINDOW);

			API_FUNC_NAKED_START()
				push	child
				push	keystone
				mov		eax, FUNCTION
				call	[eax]
			API_FUNC_NAKED_END_CDECL(2)
		}

		API_FUNC_NAKED void WindowRelease(void* handle)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(KW_RELEASE);

			API_FUNC_NAKED_START()
				push	handle
				mov		eax, FUNCTION
				call	[eax]
			API_FUNC_NAKED_END_CDECL(1)
		}

		API_FUNC_NAKED void* WindowGetControlByID(void* window, wcstring id)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(KW_GETCONTROLBYID);

			API_FUNC_NAKED_START()
				push	id
				push	window
				mov		eax, FUNCTION
				call	[eax]
			API_FUNC_NAKED_END_CDECL(2)
		}

		API_FUNC_NAKED LRESULT ControlSendMessage(void* control, uint32 msg, WPARAM wParam, LPARAM lParam)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(KC_SENDMESSAGE);

			API_FUNC_NAKED_START()
				push	lParam
				push	wParam
				push	msg
				push	control
				mov		eax, FUNCTION
				call	[eax]
			API_FUNC_NAKED_END_CDECL(4)
		}

		API_FUNC_NAKED uint32 KsTranslateAccelerator(void* arg0, HANDLE window_handle, void* arg2, const MSG* message)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(KS_TRANSLATEACCELERATOR);

			API_FUNC_NAKED_START()
				push	message
				push	arg2
				push	window_handle
				push	arg0
				mov		eax, FUNCTION
				call	[eax]
			API_FUNC_NAKED_END_CDECL(4)
		}
		
		// should only be called by the unhandled exception filter
		void ReleaseKeystone()
		{
			// release the keystone windows
			void* keystone_mainwindow = MainWindow();
			if(keystone_mainwindow)
			{
				void* keystone_childwindow;
				
				keystone_childwindow = GetWindow(keystone_mainwindow, L"KeystoneEditbox");
				if(keystone_childwindow)
				{
					WindowRelease(keystone_childwindow);
				}
				
				keystone_childwindow = GetWindow(keystone_mainwindow, L"KeystoneChatLog");
				if(keystone_childwindow)
				{
					WindowRelease(keystone_childwindow);
				}

				WindowRelease(keystone_mainwindow);
			}
		}

#pragma region Message Pump
		struct s_message_pump_globals
		{
			std::vector<i_windows_message_handler*> m_message_handlers;
		};
		static s_message_pump_globals g_message_pump_globals;

		static uint32 PLATFORM_API HandleMessage(void* arg0, HANDLE window_handle, void* arg2, const MSG* message)
		{
			uint32 value = KsTranslateAccelerator(arg0, window_handle, arg2, message);

			for(auto entry : g_message_pump_globals.m_message_handlers)
			{
				entry->HandleMessage(message);
			}

			return value;
		}

		void AttachWindowsMessageHandler(i_windows_message_handler* handler)
		{
			auto existing_entry = std::find(g_message_pump_globals.m_message_handlers.begin(), g_message_pump_globals.m_message_handlers.end(), handler);
			if(existing_entry == g_message_pump_globals.m_message_handlers.end())
			{
				g_message_pump_globals.m_message_handlers.push_back(handler);
			}
		}

		void DetachWindowsMessageHandler(i_windows_message_handler* handler)
		{
			auto existing_entry = std::find(g_message_pump_globals.m_message_handlers.begin(), g_message_pump_globals.m_message_handlers.end(), handler);
			if(existing_entry != g_message_pump_globals.m_message_handlers.end())
			{
				g_message_pump_globals.m_message_handlers.erase(existing_entry);
			}
		}
#pragma endregion
	};
};

#endif