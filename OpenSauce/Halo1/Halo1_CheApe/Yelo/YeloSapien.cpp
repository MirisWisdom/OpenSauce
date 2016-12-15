/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"

#if PLATFORM_TYPE == PLATFORM_SAPIEN
#include <blamlib/Halo1/hs/hs_structures.hpp>
#include <blamlib/Halo1/interface/first_person_weapons.hpp>
#include <YeloLib/Halo1/open_sauce/blam_memory_upgrades.hpp>

#include "CheApeInterface.hpp" // for Memory functions
#include "Settings/Settings.hpp"
#include "Settings/SettingsSapien.hpp"
#include "Engine/EngineFunctions.hpp"
#include "Engine/Scripting.hpp"
#include "Engine/Objects.hpp"
#include "Engine/GameState.hpp"
#include "TagGroups/yelo_definitions.hpp"
#include "TagGroups/yelo_scenario_definitions.hpp"

namespace Yelo
{
	namespace Objects
	{
		static const uintptr_t OBJECT_DAMAGE_AFTERMATH__CALC_ACCEL_HOOK = 0x649EE6;
		static const uintptr_t OBJECT_DAMAGE_AFTERMATH__CALC_ACCEL_HOOK_RETURN = 0x649F17;

		static const uintptr_t OBJECT_DAMAGE_AFTERMATH__UNIT_VALIDATE_INST_ACCEL_HOOK = 0x649FFF;
		static const uintptr_t OBJECT_DAMAGE_AFTERMATH__UNIT_VALIDATE_INST_ACCEL_HOOK_RETURN = 0x64A010;
		static const uintptr_t OBJECT_DAMAGE_AFTERMATH__UNIT_VALIDATE_INST_ACCEL_HOOK_RETURN_INVALID = 0x64A07E;
	};
};
#include <YeloLib/Halo1/objects/object_damage_upgrades.inl>

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

			static uintptr_t GAMESPY_METRICS_DUMP_FUNCTIONS[] = {
				0x5D6CE0, 0x5D7390
			};

			static byte* player_ui_globals = CAST_PTR(byte*, 0xDC2918);
		};

		//////////////////////////////////////////////////////////////////////////
		// sapien modifications for loading player profiles
		// the idea is that this should allow people to control units in sapien...and thus record animations
		API_FUNC_NAKED static void PLATFORM_API player_ui_set_controls_from_profile(int32 local_player_index)
		{
			static const uintptr_t FUNCTION = 0x5DD060;

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED static void PLATFORM_API player_ui_set_current_player_profile(int32 local_player_index, int32 unknown, byte profile[Enums::k_player_profile_buffer_size])
		{
			static const uintptr_t FUNCTION = 0x5DD470;

			__asm	jmp	FUNCTION
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

			if(profile_load_def != nullptr)
			{
				byte* call_addr = CAST_PTR(byte*, profile_load_def->evaluate);
				call_addr += Enums::_hs_function_profile_load_call_offset;

				Memory::WriteRelativeCall(profile_load_override, call_addr);
			}
		}
		//////////////////////////////////////////////////////////////////////////

		void IntializeBeforeTagGroupsInitalize()
		{
		}

		void Initialize()
		{
			// Set Sapien's affinity mask
			SetProcessAffinityMask(GetCurrentProcess(), Settings::c_settings_sapien::Instance()->m_affinity_mask);

			for(auto ptr : AddressOf::MaxTagsCheck)
				*ptr = Enums::k_maximum_simultaneous_tag_instances_upgrade;

			// Cause the gamespy metrics dump code to NEVER execute
			for(auto ptr : AddressOf::GAMESPY_METRICS_DUMP_FUNCTIONS)
				*CAST_PTR(byte*,ptr) = Enums::_x86_opcode_ret;

			// replace sapien's call to first_person_weapons_initialize_for_new_map to our implementation
			Memory::WriteRelativeCall(blam::first_person_weapons_initialize_for_new_map,
				CAST_PTR(void*, 0x5E704A));

			TagGroups::ScenarioYeloLoadHookInitialize();

			InitializeProfileLoadOverride();

			GameState::Initialize();
			Objects::Initialize();
			Objects::ObjectDamageAftermath_UpgradesInitialize();
		}

		void DisposeBeforeCSeries()
		{
			// NOTE: call any dispose routines which allocate using the engine's debug memory manager here
			// WARNING: this code will never run if Sapien is executed with "SAPIEN_NO_RENDER"

		}
		void Dispose()
		{
			for(auto ptr : AddressOf::MaxTagsCheck)
				*ptr = Enums::k_maximum_simultaneous_tag_instances;
			
			GameState::Dispose();
			Objects::Dispose();
			TagGroups::ScenarioYeloLoadHookDispose();
		}
	};
};
#endif