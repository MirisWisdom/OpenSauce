/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/Keystone.hpp"
#if !PLATFORM_IS_DEDI

#include "Memory/MemoryInterface.hpp"
#include "Common/YeloSettings.hpp"

namespace Yelo
{
	namespace Keystone
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_INTERFACE
#define __EL_INCLUDE_FILE_ID	__EL_INTERFACE_KEYSTONE
#include "Memory/_EngineLayout.inl"

		void OnChatAddString(wcstring string); // forward declare

		void SendMessageUpdateEAX()
		{
			static uint32 SendMessageCallAddr = GET_FUNC_PTR(KEYSTONE_CHATLOG_ADD_ITEM_SEND_MESSAGE);

			wcstring SendMessageStrAddr;
			__asm mov SendMessageStrAddr, eax;

			OnChatAddString(SendMessageStrAddr);

			__asm push SendMessageStrAddr;
			__asm call SendMessageCallAddr;
		}

		void SendMessageUpdateECX()
		{
			static uint32 SendMessageCallAddr = GET_FUNC_PTR(KEYSTONE_CHATLOG_ADD_ITEM_SEND_MESSAGE);

			wcstring SendMessageStrAddr;
			__asm mov SendMessageStrAddr, ecx;

			OnChatAddString(SendMessageStrAddr);

			__asm push SendMessageStrAddr;
			__asm call SendMessageCallAddr;
		}

		void SendMessageUpdateEDX()
		{
			static uint32 SendMessageCallAddr = GET_FUNC_PTR(KEYSTONE_CHATLOG_ADD_ITEM_SEND_MESSAGE);

			wcstring SendMessageStrAddr;
			__asm mov SendMessageStrAddr, edx;

			OnChatAddString(SendMessageStrAddr);

			__asm push SendMessageStrAddr;
			__asm call SendMessageCallAddr;
		}

		void SendMessageInitialize()
		{
#define CALL_SENDMSG_HOOK(reg, ce_addr)						\
	call = Enums::_x86_opcode_call_near;					\
	rel_call = _SendMessageUpdatePtr##reg - (ce_addr + 5);	\
	Memory::WriteMemory( CAST_PTR(void*,ce_addr),&call,1);	\
	Memory::WriteMemory( CAST_PTR(void*,ce_addr+1),&rel_call,4);

			byte call;
			uint32 rel_call;

#pragma warning( push )
#pragma warning( disable : 4311 ) // bitching about pointer truncation

			uint32 _SendMessageUpdatePtrEAX = CAST_PTR(uint32,&SendMessageUpdateEAX);
			uint32 _SendMessageUpdatePtrECX = CAST_PTR(uint32,&SendMessageUpdateECX);
			uint32 _SendMessageUpdatePtrEDX = CAST_PTR(uint32,&SendMessageUpdateEDX);

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
#endif

#pragma warning( pop )
		}

		struct keystone_globals_data {
			// chat that appears in the chatlog log file
			cstring chatlog_log;
			FILE* log;
		}keystone_globals;


		wcstring ChatLogName()	PTR_IMP_GET2(ChatLogName);
		void* MainWindow()		PTR_IMP_GET2(MainWindow);

		void Initialize()
		{
			keystone_globals.chatlog_log = Settings::K_CHAT_LOG_FILENAME;
			keystone_globals.log = Settings::CreateReport(keystone_globals.chatlog_log, true, true);

			if(keystone_globals.log != NULL)
				SendMessageInitialize();
		}

		void Dispose()
		{
			if(keystone_globals.log != NULL)
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
			static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(KS_GETWINDOW);

			API_FUNC_NAKED_START()
				push	child
				push	keystone
				call	TEMP_CALL_ADDR
			API_FUNC_NAKED_END_CDECL(2)
		}

		API_FUNC_NAKED void WindowRelease(void* handle)
		{
			static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(KW_RELEASE);

			API_FUNC_NAKED_START()
				push	handle
				call	TEMP_CALL_ADDR
			API_FUNC_NAKED_END_CDECL(1)
		}

		API_FUNC_NAKED void* WindowGetControlByID(void* window, wcstring id)
		{
			static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(KW_GETCONTROLBYID);

			API_FUNC_NAKED_START()
				push	id
				push	window
				call	TEMP_CALL_ADDR
			API_FUNC_NAKED_END_CDECL(2)
		}

		API_FUNC_NAKED LRESULT ControlSendMessage(void* control, uint32 msg, WPARAM wParam, LPARAM lParam)
		{
			static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(KC_SENDMESSAGE);

			API_FUNC_NAKED_START()
				push	lParam
				push	wParam
				push	msg
				push	control
				call	TEMP_CALL_ADDR
			API_FUNC_NAKED_END_CDECL(4)
		}
	};
};

#endif