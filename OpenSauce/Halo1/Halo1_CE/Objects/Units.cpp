/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Objects/Units.hpp"

#include <blamlib/Halo1/objects/damage.hpp>
#include <blamlib/Halo1/game/game_globals.hpp>
#include <blamlib/Halo1/game/game_globals_definitions.hpp>
#include <blamlib/Halo1/saved_games/game_state_structures.hpp>
#include <blamlib/Halo1/units/unit_definitions.hpp>
#include <blamlib/Halo1/items/weapons.hpp>

#include <blamlib/Halo1/units/biped_structures.hpp>
#include <blamlib/Halo1/units/vehicle_structures.hpp>
#include <blamlib/Halo1/units/unit_structures.hpp>

#include <YeloLib/Halo1/units/units_yelo.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario_definitions.hpp>

#include "Game/Camera.hpp"
#include "Game/EngineFunctions.hpp"
#include "Game/GameState.hpp"
#include "Game/AI.hpp"
#include "Memory/MemoryInterface.hpp"

#include "Objects/Objects.hpp"

namespace Yelo
{
	namespace Objects { namespace Units {
#define __EL_INCLUDE_ID			__EL_INCLUDE_OBJECTS
#define __EL_INCLUDE_FILE_ID	__EL_OBJECTS_UNITS
#include "Memory/_EngineLayout.inl"
	}; };
};

#include "Objects/Units.SeatBoarding.inl"
#include "Objects/Units.SeatDamage.inl"
#include "Objects/Units.Animations.inl"
#include "Objects/Units.GrenadeCounts.inl"

namespace Yelo
{
	namespace Objects { namespace Units
	{
		static void PLATFORM_API UnitDamageAftermathHook(const datum_index unit_index
			, const s_damage_data* damage_data
			, const Flags::object_damage_flags damage_flags
			, const real shield_amount
			, const real body_amount
			, void* arg6
			, const int32 damage_part)
		{
			blam::unit_damage_aftermath(unit_index, damage_data, damage_flags, shield_amount, body_amount, arg6, damage_part);

			AI::UnitDamageAftermath(unit_index, damage_data);
		}

		bool PLATFORM_API UnitCanEnterSeat(datum_index unit_index
			, datum_index target_unit_index
			, int16 target_seat_index,
			_Out_opt_ datum_index* return_unit_in_seat)
		{
			bool result = true;

			Objects::UnitCanEnterSeatMultiteam(unit_index, target_unit_index, target_seat_index, return_unit_in_seat, result);
			SeatBoarding::UnitCanEnterBoardingSeat(unit_index, target_unit_index, target_seat_index, result);
			SeatBoarding::UnitCanEnterTargetSeat(unit_index, target_unit_index, target_seat_index, result);

			return result;
		}

		API_FUNC_NAKED static void PLATFORM_API UnitCanEnterSeatHook()
		{
			API_FUNC_NAKED_START_()
				push	ebx
				push	ecx
				push	edx

				mov		ecx, [esp+24]
				mov		ebx, [esp+20]
				push	ecx		// datum_index* unit_in_seat_index
				push	ebx		// int16 target_seat_index
				push	edx		// datum_index target_unit_index
				push	eax		// datum_index unit_index
				call	UnitCanEnterSeat
				add		esp, 10h

				pop		edx
				pop		ecx
				pop		ebx
			API_FUNC_NAKED_END_()
		}

		void UnitCausePlayerSeatedMelee(const datum_index unit_index)
		{
			SeatDamage::UnitCauseSeatedPlayerMelee(unit_index);
		}

		API_FUNC_NAKED static void BipedSeatedMeleeHook()
		{
			static uintptr_t RETN_ADDRESS = GET_FUNC_PTR(BIPED_SEATED_MELEE_RETN);

			_asm
			{
				push	esi
				pushad

				push	esi
				call	BipedSeatedMelee
				add		esp, 4

				popad
				pop		esi

				mov		esi, [ebp+8]
				push	esi
				lea		ecx, [ebp-4]
				jmp		RETN_ADDRESS
			}
		}

		void PLATFORM_API UnitThrowGrenadeReleaseHook(const datum_index unit_index, const sbyte keyframe)
		{
			auto* unit_datum = blam::object_get_and_verify_type<s_unit_datum>(unit_index);
			if(unit_datum->unit.vehicle_seat_index == NONE)
			{
				blam::unit_throw_grenade_release(unit_index, keyframe);
			}
			else
			{
				SeatDamage::UnitThrowSeatedPlayerGrenade(unit_index);
			}
		}

		bool PLATFORM_API WeaponPreventsGrenadeThrowing(const datum_index unit_index, const datum_index weapon_index)
		{
			auto* unit_datum = blam::object_get_and_verify_type<s_unit_datum>(unit_index);
			if(unit_datum->unit.vehicle_seat_index == NONE)
			{
				return blam::weapon_prevents_grenade_throwing(weapon_index);
			}
			else
			{
				return SeatDamage::PreventSeatedPlayerGrenade(unit_index, weapon_index);
			}
		}

		API_FUNC_NAKED bool WeaponPreventsGrenadeThrowingHook()
		{
			static uintptr_t RETN_ADDRESS = GET_FUNC_PTR(WEAPON_PREVENTS_GRENADE_THROWING_RETN);

			_asm
			{
				push	ecx
				push	edi
				call	WeaponPreventsGrenadeThrowing
				add		esp, 8

				jmp		RETN_ADDRESS
			};
		}

		void Initialize()
		{
			Animations::Initialize();
			SeatBoarding::Initialize();

			// Enables biped seats
			Memory::WriteMemory(GET_FUNC_VPTR(BIPED_UPDATE_CHECK_PARENT_UNIT_TYPE), Enums::_x86_opcode_nop, 6);

			Memory::WriteRelativeCall(&UnitDamageAftermathHook, GET_FUNC_VPTR(UNIT_DAMAGE_AFTERMATH_CALL), true);
			Memory::CreateHookRelativeCall(&UnitCanEnterSeatHook, GET_FUNC_VPTR(UNIT_CAN_ENTER_SEAT_HOOK), Enums::_x86_opcode_ret);
			Memory::WriteRelativeJmp(&BipedSeatedMeleeHook, GET_FUNC_VPTR(BIPED_SEATED_MELEE_HOOK), true);
			Memory::WriteRelativeCall(&UnitThrowGrenadeReleaseHook, GET_FUNC_VPTR(UNIT_THROW_GRENADE_RELEASE_HOOK), true);
			Memory::WriteRelativeJmp(&WeaponPreventsGrenadeThrowingHook, GET_FUNC_VPTR(WEAPON_PREVENTS_GRENADE_THROWING_HOOK), true);
		}

		void Dispose() { }

		void ObjectsUpdate()
		{
			Objects::c_object_iterator iter(Enums::_object_type_mask_unit);

			for(auto object_index : iter)
			{
				SeatBoarding::UnitUpdate(object_index.index);
			}
		}

		static void InitializeUnitGrenadeTypesForNewMap(GameState::s_yelo_header_data& yelo_header)
		{
			static bool g_already_enabled = false;

			if (!yelo_header.flags.update_unit_grenade_types_count)
				return;

			uint32 count = yelo_header.unit_grenade_types_count;

			bool enabled = count > Enums::k_unit_grenade_types_count;
			// HACK: if the grenade count changes are already in place, don't repeat or undo them
			// There was a problem with clients getting disconnected from the server during a join attempt.
			// It puts you back at the mainmenu, but when you try to join again, this would executed again
			// without the changes from last time being undo'd, resulting in a ASSERT() failing on code bytes mismatch
			if (enabled != g_already_enabled)
			{
				InitializeGrenadeCounts_UnitZoomLevelRefs(enabled);
				InitializeGrenadeCounts_UnitDesiredZoomLevelRefs(enabled);
				InitializeGrenadeCounts_NumberOfUnitGrenadeTypes(count);
				InitializeGrenadeCounts_UnitGrenadeCounts(enabled);
				InitializeGrenadeCounts_MessageDeltaGrenadeCounts(enabled);

				g_already_enabled = enabled;
			}

			yelo_header.flags.update_unit_grenade_types_count = false;
		}

		void InitializeForNewMap()
		{
			if(GameState::YeloGameStateEnabled())
			{
				GameState::s_yelo_header_data& yelo_header = GameState::GameStateGlobals()->header->yelo;

				// handle the grenade types upgrading, if needed
				InitializeUnitGrenadeTypesForNewMap(yelo_header);
			}
		}

		void DisposeFromOldMap() { }

		void InitializeForNewMapPrologue()
		{
			const TagGroups::s_game_globals* game_globals = GameState::GlobalGameGlobals();
			GameState::s_yelo_header_data& yelo_header = GameState::GameStateGlobals()->header->yelo;

			// check this map's grenade count vs the previous map's
			if (game_globals->grenades.Count != yelo_header.unit_grenade_types_count)
			{
				// inform the unit grenade counts code to run an assembly update
				yelo_header.flags.update_unit_grenade_types_count = true;
				// figure out the new grenade count
				// defaulting to the stock count if there is suspicious number
				if (game_globals->grenades.Count <= Enums::k_unit_grenade_types_count_yelo)
					yelo_header.unit_grenade_types_count = CAST(byte, game_globals->grenades.Count);
				else
					yelo_header.unit_grenade_types_count = Enums::k_unit_grenade_types_count;
			}
		}

		void InitializeForYeloGameState(bool enabled)
		{
		}
	}; };
};