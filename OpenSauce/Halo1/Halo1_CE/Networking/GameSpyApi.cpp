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
#endif

		s_gamespy_product* GsProducts()										PTR_IMP_GET2(gamespy_products_list);

		API_FUNC_NAKED s_gamespy_client* GsGetClient(int32 client_id)
		{
			static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(GAMESPY_GET_CLIENT_KEY_HASH);

			__asm {
				push	ebp
				mov		ebp, esp
				push	ecx

				mov		eax, client_id
				call	TEMP_CALL_ADDR
				cmp		eax, GET_DATA_PTR(compiler_null_string)
				jnz		_return
				mov		eax, 4 // will cause us to return NULL. Yes, I r a hacka
				
_return:
				sub		eax, 4

				pop		ecx
				pop		ebp
				retn	4 * 1
			}
		}
	};


	namespace Enums
	{
		enum game_version_id : long_enum {
			//_game_version_id_ = ,
			_game_version_id_100 = 0x94ECF,	//  609999
			_game_version_id_107 = 0x5BC42F,// 6013999
			_game_version_id_108 = 0x5BCFE7,// 6016999
			_game_version_id_109 = 0x96A27,	//  616999
		};
	};

	namespace Networking
	{
		static void ChangeAdvertisedGameVersionId(long_enum version_id, bool and_game_build)
		{
			using namespace Enums;

			if(version_id)
			{
				GET_PTR(game_version_id1) = version_id;
				GET_PTR(game_version_id2) = version_id;
				GET_PTR(game_version_id3) = version_id;

				if(and_game_build)
				{
					cstring build_str = NULL;

						 if(version_id == _game_version_id_100)	build_str = "01.00.00.0609";
					else if(version_id == _game_version_id_107) build_str = "01.00.07.0613";
					else if(version_id == _game_version_id_108) build_str = "01.00.08.0616";
					else if(version_id == _game_version_id_109) build_str = "01.00.09.0620";

					if(build_str != NULL)
					{
						strcpy(GameState::GameBuildString(), build_str);
						strcpy(GameState::GamespyGameBuildString(), build_str);
					}
				}
			}
		}

		bool ChangeAdvertisedGameVersion(cstring version_str, bool and_game_build)
		{
			using namespace Enums;

			bool result = true;
			long_enum version_id = 0;

				 if( strstr(version_str, "1.00") ) version_id = _game_version_id_100;
			else if( strstr(version_str, "1.07") ) version_id = _game_version_id_107;
			else if( strstr(version_str, "1.08") ) version_id = _game_version_id_108;
			else if( strstr(version_str, "1.09") ) version_id = _game_version_id_109;
			else result = false;

			if(result)
				ChangeAdvertisedGameVersionId(version_id, and_game_build);

			return result;
		}
	};
};