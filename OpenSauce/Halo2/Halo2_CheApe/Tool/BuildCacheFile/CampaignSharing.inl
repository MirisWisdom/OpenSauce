/*
    Yelo: Open Sauce SDK
		Halo 2 (Editing Kit) Edition

	See license\OpenSauce\Halo2_CheApe for specific license information
*/

/* tag_sharing_load_multiplayer_shared changes to make it support campaign

.text:00581454                 push    offset aMapsShared_m_0 ; "maps\\shared.map"

.text:0058147F                 mov     eax, offset aMapsShared_m_0 ; "maps\\shared.map"
*/

namespace build_cache_file_ex_hacks
{
	namespace campaign_sharing
	{
		static long_enum _scenario_type;

		static void* BUILD_CACHE_FILE_FOR_SCENARIO__GET_SCENARIO_TYPE	= CAST_PTR(void*, 0x58830A);
		static byte* BUILD_CACHE_FILE_FOR_SCENARIO__CHECK_SCENARIO_TYPE	= CAST_PTR(byte*, 0x588320); // jz      short loc_588336
		API_FUNC_NAKED static void _build_cache_file_for_scenario__intercept_get_scenario_type()
		{
			static const uint32 INTERCEPTOR_EXIT = 0x588313;

			__asm {
				movsx	edx, word ptr [eax+0x1C]
				push	esi
				mov		[esp+0x58], edx			// mov     [esp+1F90h+scenario_type], edx
				mov		_scenario_type, edx
				jmp		INTERCEPTOR_EXIT
			}
		}

		static void* BUILD_CACHE_FILE_FOR_SCENARIO__TAG_SHARING_LOAD_SHARED = CAST_PTR(void*, 0x5883DE);
		API_FUNC_NAKED static void _tag_sharing_load_multiplayer_shared__interceptor()
		{
			static const uint32 TAG_SHARING_LOAD_MULTIPLAYER_SHARED = 0x5813C0;
			static wcstring k_campaign_shared_name = L"maps\\single_player_shared.map";
			static const uint32 TAG_SHARING_LOAD_SHARED_NAME1 = 0x581455;
			static const uint32 TAG_SHARING_LOAD_SHARED_NAME2 = 0x581480;

			__asm {
				cmp		_scenario_type, 0 // is this a campaign scenario?
				jnz		call_tag_sharing_load_multiplayer_shared
				// replace the shared.map string references with the campaign one
				push	ecx
				mov		ecx, k_campaign_shared_name
				mov		eax, TAG_SHARING_LOAD_SHARED_NAME1
				mov		[eax], ecx
				mov		eax, TAG_SHARING_LOAD_SHARED_NAME2
				mov		[eax], ecx
				pop		ecx
call_tag_sharing_load_multiplayer_shared:
				jmp		TAG_SHARING_LOAD_MULTIPLAYER_SHARED
			}
		}

		void Initialize()
		{
			Memory::WriteRelativeJmp(&_build_cache_file_for_scenario__intercept_get_scenario_type, 
				BUILD_CACHE_FILE_FOR_SCENARIO__GET_SCENARIO_TYPE, true);

			*BUILD_CACHE_FILE_FOR_SCENARIO__CHECK_SCENARIO_TYPE = 0xEB; // change from 'jz' to 'jmp'

			Memory::WriteRelativeCall(&_tag_sharing_load_multiplayer_shared__interceptor,
				BUILD_CACHE_FILE_FOR_SCENARIO__TAG_SHARING_LOAD_SHARED);
		}
	};
};