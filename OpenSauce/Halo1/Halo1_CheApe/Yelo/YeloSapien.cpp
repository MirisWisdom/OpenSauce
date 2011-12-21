/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"

#if PLATFORM_ID == PLATFORM_SAPIEN
#include <Blam/Halo1/BlamMemoryUpgrades.hpp>
#include "CheApeInterface.hpp" // for Memory functions
#include "Common/YeloSettings.hpp"
#include "Engine/Scripting.hpp"
#include "TagGroups/yelo_definitions.hpp"
#include "TagGroups/yelo_scenario_definitions.hpp"

namespace Yelo
{
	namespace Sapien
	{
		namespace AddressOf
		{
			// Addresses which check a counter against the max tag limit
			static uint16* MaxTagsCheck[] = {
				CAST_PTR(uint16*, 0x4FC9AB),
			};

			static uint32 GAMESPY_METRICS_DUMP_FUNCTIONS[] = {
				0x5D6CE0, 0x5D7390
			};

			static byte* player_ui_globals = CAST_PTR(byte*, 0xDC2918);
		};

		//////////////////////////////////////////////////////////////////////////
		// sapien modifications for loading player profiles
		// the idea is that this should allow people to control units in sapien...and thus record animations
		API_FUNC_NAKED static void player_ui_set_controls_from_profile(int32 local_player_index)
		{
			static const uint32 TEMP_CALL_ADDR = 0x5DD060;

			NAKED_FUNC_START()
				push	local_player_index
				call	TEMP_CALL_ADDR
			NAKED_FUNC_END_CDECL(1)
		}
		API_FUNC_NAKED static void player_ui_set_current_player_profile(int32 local_player_index, int32 unknown, byte profile[Enums::k_player_profile_buffer_size])
		{
			static const uint32 TEMP_CALL_ADDR = 0x5DD470;

			NAKED_FUNC_START()
				push	profile
				push	unknown
				push	local_player_index
				call	TEMP_CALL_ADDR
			NAKED_FUNC_END_CDECL(3)
		}

		static void PLATFORM_API profile_load_override(cstring profile_name)
		{
			byte profile[Enums::k_player_profile_buffer_size];
			if(Settings::PlayerProfileRead(profile_name, profile))
			{
				//memcpy_s(AddressOf::player_ui_globals, Enums::k_player_profile_buffer_size,
				//	profile, Enums::k_player_profile_buffer_size);

				//player_ui_set_controls_from_profile(0);
				player_ui_set_current_player_profile(0, -1, profile);

				YELO_WARN("CheApe: loaded profile '%s'", profile_name);
			}
			else
				YELO_WARN("CheApe: couldn't load profile '%s'", profile_name);
		}
		static void InitializeProfileLoadOverride()
		{
			Scripting::hs_function_definition* profile_load_def = Scripting::GetFunctionDefintion(Enums::_hs_function_profile_load);

			if(profile_load_def != NULL)
			{
				byte* call_addr = CAST_PTR(byte*, profile_load_def->evaluate);
				call_addr += Enums::_hs_function_profile_load_call_offset;

				Memory::WriteRelativeCall(profile_load_override, call_addr);
			}
		}
		//////////////////////////////////////////////////////////////////////////

		void Initialize()
		{
			// Set Sapien's affinity mask if there is one specified
			const Settings::s_profile& k_profile = Settings::Get().active_profile;
			if(!k_profile.IsIgnored() && k_profile.has_affinity_mask)
			{
				SetProcessAffinityMask(GetCurrentProcess(), k_profile.affinity_mask);
			}

			for(int32 x = 0; x < NUMBEROF(AddressOf::MaxTagsCheck); x++)
				*AddressOf::MaxTagsCheck[x] = CAST(uint16, Konstants::MaximumTagCountUpgrade());

			// Cause the gamespy metrics dump code to NEVER execute
			for(int32 x = 0; x < NUMBEROF(AddressOf::GAMESPY_METRICS_DUMP_FUNCTIONS); x++)
				*CAST_PTR(byte*, AddressOf::GAMESPY_METRICS_DUMP_FUNCTIONS[x]) = 0xC3;

			TagGroups::ScenarioYeloLoadHookInitialize();

			InitializeProfileLoadOverride();
		}

		void Dispose()
		{
			for(int32 x = 0; x < NUMBEROF(AddressOf::MaxTagsCheck); x++)
				*AddressOf::MaxTagsCheck[x] = Enums::k_maximum_tag_count;

			TagGroups::ScenarioYeloLoadHookDispose();
		}
	};
};
#endif