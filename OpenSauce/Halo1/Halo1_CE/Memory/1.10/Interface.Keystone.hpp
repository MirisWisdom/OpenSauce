/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Memory/1.10/Pointers/HaloCE_110_Runtime.Interface.inl"
#include "Memory/1.10/Pointers/HaloCE_110_Runtime_Manual.Interface.inl"
#endif

#include <YeloLib/memory/memory_interface_base.hpp>
#include <Memory/MemoryInterface.hpp>

namespace Yelo
{
	namespace Keystone
	{
		FUNC_PTR(KEYSTONE_CHATLOG_ADD_ITEM_SEND_MESSAGE, K_KEYSTONE_CHATLOG_ADD_ITEM_SEND_MESSAGE, FUNC_PTR_NULL);

		ENGINE_PTR(wchar_t, ChatLogName, K_CHAT_LOG_NAME, PTR_NULL);
		ENGINE_PTR(void**, MainWindow, K_MAIN_WINDOW, PTR_NULL);

		FUNC_PTR(KS_TRANSLATEACCELERATOR_CALL, K_KS_TRANSLATEACCELERATOR_CALL, FUNC_PTR_NULL);
		FUNC_PTR(KS_TRANSLATEACCELERATOR, K_KS_TRANSLATEACCELERATOR, FUNC_PTR_NULL);
		FUNC_PTR(KS_GETWINDOW, K_KS_GETWINDOW, FUNC_PTR_NULL);
		FUNC_PTR(KW_RELEASE, K_KW_RELEASE, FUNC_PTR_NULL);
		FUNC_PTR(KW_GETCONTROLBYID, K_KW_GETCONTROLBYID, FUNC_PTR_NULL);
		FUNC_PTR(KC_SENDMESSAGE, K_KC_SENDMESSAGE, FUNC_PTR_NULL);
	}
}
