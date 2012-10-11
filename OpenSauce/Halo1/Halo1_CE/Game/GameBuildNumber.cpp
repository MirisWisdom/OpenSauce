/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Game/GameBuildNumber.hpp"

#include "Memory/MemoryInterface.hpp"

#include "Common/YeloSettings.hpp"
#include "Game/GameState.hpp"

namespace Yelo
{
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

	namespace BuildNumber
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_GAME
#define __EL_INCLUDE_FILE_ID	__EL_GAME_BUILD_NUMBER
#include "Memory/_EngineLayout.inl"

		char* GameBuildString()					PTR_IMP_GET2(game_build_version);
		char* GamespyGameBuildString()			PTR_IMP_GET2(game_build_version_gamespy);


		DOC_TODO_DEBUG("Update these if you change Client platform version")
		// These are the same build strings the stock game checks when verifying a game save
		static cstring k_binary_compatible_build_numbers_stock[] = {
#if PLATFORM_VERSION == 0x1080
			"01.00.08.0616",
#elif PLATFORM_VERSION == 0x1090
			"01.00.09.0620",
#endif
			"01.00.00.0564",
			"01.00.01.0580",
			"01.00.02.0581",
		};

		static cstring k_build_number_yelo_current = K_OPENSAUCE_VERSION_HCE_BUILD_NUMBER(PLATFORM_VERSION_BUILD);
		static cstring k_binary_compatible_build_numbers_yelo[] = {
			k_build_number_yelo_current
		};


		static bool GameStateHeaderIsValid_BuildNumberImpl(const GameState::s_header_data* header)
		{
			const tag_string& build_number = header->version;
			if(GameState::YeloGameStateEnabled())
			{
				for(int x = 0; x < NUMBEROF(k_binary_compatible_build_numbers_yelo); x++)
					if(strcmp(build_number, k_binary_compatible_build_numbers_yelo[x])==0)
						return true;
			}
			else
			{
				for(int x = 0; x < NUMBEROF(k_binary_compatible_build_numbers_stock); x++)
					if(strcmp(build_number, k_binary_compatible_build_numbers_stock[x])==0)
						return true;
			}

			return false;
		}
		static API_FUNC_NAKED void GameStateHeaderIsValidHook()
		{
			static const uintptr_t RET_ADDRESS = GET_FUNC_PTR(GAME_STATE_HEADER_IS_VALID_HOOK_RET);

			__asm {
				push	ebp
				call	GameStateHeaderIsValid_BuildNumberImpl
				jmp		RET_ADDRESS
			}
		}
		static API_FUNC_NAKED void GameStateTryAndLoadHook()
		{
			static const uintptr_t RET_ADDRESS_TRUE = GET_FUNC_PTR(GAME_STATE_HEADER_TRY_AND_LOAD_HOOK_RET_TRUE);
			static const uintptr_t RET_ADDRESS_FALSE = GET_FUNC_PTR(GAME_STATE_HEADER_TRY_AND_LOAD_HOOK_RET_FALSE);

			__asm {
				//lea		ebx, [esp+0x158-0x14C]
				push	ebx
				call	GameStateHeaderIsValid_BuildNumberImpl
				test	al, al
				jz		is_invalid
				jmp		RET_ADDRESS_TRUE
is_invalid:
				jmp		RET_ADDRESS_FALSE
			}
		}

		void Initialize()
		{
			Memory::WriteRelativeJmp(&GameStateHeaderIsValidHook, GET_FUNC_VPTR(GAME_STATE_HEADER_IS_VALID_HOOK), true);
			Memory::WriteRelativeJmp(&GameStateTryAndLoadHook, GET_FUNC_VPTR(GAME_STATE_HEADER_TRY_AND_LOAD_HOOK), true);
		}
		void Dispose()
		{
		}

		void InitializeForNewMap()
		{
			if(GameState::YeloGameStateEnabled())
			{
				GameState::s_header_data* header = GameState::GameStateGlobals()->header;
				strcpy_s(header->version, k_build_number_yelo_current);
			}
		}

		void InitializeForYeloGameState(bool enabled)
		{
		}


		cstring k_game_build_numbers[Enums::k_max_game_build_number_index] = {
			"01.00.00.0609",
			"01.00.07.0613",
			"01.00.08.0616",
			"01.00.09.0620",
		};
		static cstring k_game_build_numbers_yelo[] = {
			k_build_number_yelo_current
		};

		bool StringIsValid(cstring build_number)
		{
			for(int i = 0; i < NUMBEROF(k_game_build_numbers); i++)
				if(strcmp(k_game_build_numbers[i], build_number) == 0)
					return true;

			return false;
		}

		cstring VersionToBuildNumberString(cstring maj_minor_str)
		{
			using namespace Enums;

				 if( strstr(maj_minor_str, "1.00") ) return k_game_build_numbers[_game_build_number_index_100];
			else if( strstr(maj_minor_str, "1.07") ) return k_game_build_numbers[_game_build_number_index_107];
			else if( strstr(maj_minor_str, "1.08") ) return k_game_build_numbers[_game_build_number_index_108];
			else if( strstr(maj_minor_str, "1.09") ) return k_game_build_numbers[_game_build_number_index_109];
			else return NULL;
		}

		static void ChangeAdvertisedVersionId(long_enum version_id, bool and_game_build)
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

						 if(version_id == _game_version_id_100)	build_str = k_game_build_numbers[_game_build_number_index_100];
					else if(version_id == _game_version_id_107) build_str = k_game_build_numbers[_game_build_number_index_107];
					else if(version_id == _game_version_id_108) build_str = k_game_build_numbers[_game_build_number_index_108];
					else if(version_id == _game_version_id_109) build_str = k_game_build_numbers[_game_build_number_index_109];

					if(build_str != NULL)
					{
						strcpy(GameBuildString(), build_str);
						strcpy(GamespyGameBuildString(), build_str);
					}
				}
			}
		}
		bool ChangeAdvertisedVersion(cstring version_str, bool and_game_build)
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
				ChangeAdvertisedVersionId(version_id, and_game_build);

			return result;
		}
	};
};