/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Networking/GameSpyApi.hpp"

#include "Memory/MemoryInterface.hpp"
#include "Game/GameState.hpp"

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_NETWORKING
#define __EL_INCLUDE_FILE_ID	__EL_NETWORKING_GAMESPY_API
#include "Memory/_EngineLayout.inl"

	namespace Networking
	{
		s_gamespy_socket* GsSocket()										DPTR_IMP_GET(gs_Socket);
		s_gamespy_socket* GsLoopbackSocket()								DPTR_IMP_GET(gs_LoopbackSocket);
		s_gamespy_config* GsConfig()										PTR_IMP_GET2(gamespy_config);
		s_gamespy_globals* GsGlobals()										PTR_IMP_GET2(gamespy_globals);
#if PLATFORM_IS_USER											// While the getter for this is only in User builds, I keep the definition around just for documentation's sake
		s_gamespy_server_browser_globals* GsServerBrowserGlobals()			PTR_IMP_GET2(gamespy_server_browser_globals);
		static bool* GsPatchCheckForUpdates()								PTR_IMP_GET2(g_gamespy_patch_check_for_updates);
#endif

		s_gamespy_product* GsProducts()										PTR_IMP_GET2(gamespy_products_list);

		API_FUNC_NAKED s_gamespy_client* GsGetClient(int32 client_id)
		{
			static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(GAMESPY_GET_CLIENT_KEY_HASH);

			API_FUNC_NAKED_START()
				push	ecx

				mov		eax, client_id
				call	TEMP_CALL_ADDR
				cmp		eax, GET_DATA_PTR(compiler_null_string)
				jnz		_return
				mov		eax, 4 // will cause us to return NULL. Yes, I r a hacka
				
_return:
				sub		eax, 4

				pop		ecx
			API_FUNC_NAKED_END(1)
		}

		static int32 gamespy_patch_check_for_updates_sans_check()
		{
			return GsConfig()->check_for_updates_status = Enums::_gamespy_update_status_no_update;
		}
		void GsTurnOffUpdateCheck()
		{
#if PLATFORM_IS_USER
			*GsPatchCheckForUpdates() = false; // pretend we've checked for updates already

			Memory::WriteRelativeCall(&gamespy_patch_check_for_updates_sans_check, 
				GET_DATA_VPTR(GAMESPY_PATCH_SPAWN_CHECK_FOR_UPDATES_THREAD_CALL));
#endif
		}
	};
};