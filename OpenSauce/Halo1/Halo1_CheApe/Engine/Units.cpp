/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Engine/Objects.hpp"

#if PLATFORM_TYPE == PLATFORM_SAPIEN

#include <blamlib/Halo1/objects/objects.hpp>
#include <blamlib/Halo1/objects/damage.hpp>
#include <blamlib/Halo1/units/units.hpp>
#include <blamlib/Halo1/items/weapons.hpp>

#include "Engine/GrenadeTypesUpgrade.hpp"
#include "Engine/AI.hpp"

#include "Engine/Units.Animations.inl"
#include "Engine/Units.SeatBoarding.inl"
#include "Engine/Units.SeatDamage.inl"

namespace Yelo
{
	namespace Objects { namespace Units
	{
		FUNC_PTR(BIPED_UPDATE_CHECK_PARENT_UNIT_TYPE,			PTR_NULL, PTR_NULL, 0x657D6C);
		FUNC_PTR(BIPED_UPDATE_PARENT_UNIT_TYPE_MASK,			PTR_NULL, PTR_NULL, 0x657D75);
		FUNC_PTR(BIPED_VEHICLE_SPEECH_PARENT_UNIT_TYPE_MASK,	PTR_NULL, PTR_NULL, 0x651519);
		FUNC_PTR(BIPED_VEHICLE_SPEECH_PARENT_UNIT_TYPE_TAG,		PTR_NULL, PTR_NULL, 0x651526);
		FUNC_PTR(UNIT_DAMAGE_AFTERMATH_CALL,					PTR_NULL, PTR_NULL, 0x64A128);
		FUNC_PTR(UNIT_CAN_ENTER_SEAT_HOOK,						PTR_NULL, PTR_NULL, 0x586800);
		FUNC_PTR(BIPED_SEATED_MELEE_HOOK,						PTR_NULL, PTR_NULL, 0x6580FB);
		FUNC_PTR(BIPED_SEATED_MELEE_RETN,						PTR_NULL, PTR_NULL, 0x658101);
		FUNC_PTR(UNIT_THROW_GRENADE_RELEASE_HOOK,				PTR_NULL, PTR_NULL, 0x58EF76);
		FUNC_PTR(WEAPON_PREVENTS_GRENADE_THROWING_HOOK,			PTR_NULL, PTR_NULL, 0x59027A);
		FUNC_PTR(WEAPON_PREVENTS_GRENADE_THROWING_RETN,			PTR_NULL, PTR_NULL, 0x59027F);
		FUNC_PTR(INTER_VEHICLE_SEAT_ASSERT_JZ,					PTR_NULL, PTR_NULL, 0x5908AD);
		
		void PLATFORM_API UnitDamageAftermathHook(const datum_index unit_index
			, const s_damage_data* damage_data
			, const Flags::object_damage_flags damage_flags
			, const real shield_amount
			, const real body_amount
			, void* arg6
			, const int32 damage_part
			, const datum_index dead_unit_index)
		{
			blam::unit_damage_aftermath(unit_index
				, damage_data
				, damage_flags
				, shield_amount
				, body_amount
				, arg6
				, damage_part
				, dead_unit_index);

			AI::UnitDamageAftermath(unit_index, damage_data);
		}

		static bool PLATFORM_API UnitCanEnterSeat(datum_index unit_index
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

		void UnitCausePlayerSeatedMelee(const datum_index unit_index)
		{
			SeatDamage::UnitCauseSeatedPlayerMelee(unit_index);
		}

		API_FUNC_NAKED static void BipedSeatedMeleeHook()
		{
			static uintptr_t RETN_ADDRESS = GET_FUNC_PTR(BIPED_SEATED_MELEE_RETN);

			_asm
			{
				push	eax
				push	edx

				push	ebp
				call	BipedSeatedMelee
				add		esp, 4

				pop		edx
				pop		eax

				lea		ecx, [esp + 10h]
				push	ecx
				push	ebp

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
				push	ebp
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
			Memory::WriteMemory(GET_FUNC_VPTR(BIPED_UPDATE_CHECK_PARENT_UNIT_TYPE), Enums::_x86_opcode_nop, 2);
			*CAST_PTR(byte*, GET_FUNC_PTR(BIPED_UPDATE_PARENT_UNIT_TYPE_MASK)) = Enums::_object_type_mask_unit;
			*CAST_PTR(byte*, GET_FUNC_PTR(BIPED_VEHICLE_SPEECH_PARENT_UNIT_TYPE_MASK)) = Enums::_object_type_mask_unit;
			*CAST_PTR(tag*, GET_FUNC_PTR(BIPED_VEHICLE_SPEECH_PARENT_UNIT_TYPE_TAG)) = 'unit';
			
			// Stop an assertion when a unit with a parent tries to enter another seat
			Memory::WriteMemory(GET_FUNC_VPTR(INTER_VEHICLE_SEAT_ASSERT_JZ), Enums::_x86_opcode_jmp_short, 1);

			Memory::WriteRelativeCall(&UnitDamageAftermathHook, GET_FUNC_VPTR(UNIT_DAMAGE_AFTERMATH_CALL), true);
			Memory::WriteRelativeJmp(&UnitCanEnterSeat, GET_FUNC_VPTR(UNIT_CAN_ENTER_SEAT_HOOK), true);
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

		void InitializeForNewMap()
		{
			Items::InitializeGrenadesForNewMap();
		}

		void DisposeFromOldMap() { }
	};};
};
#endif