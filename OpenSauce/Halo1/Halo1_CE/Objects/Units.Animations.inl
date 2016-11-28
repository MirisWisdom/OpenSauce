/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

#include <blamlib/Halo1/units/unit_structures.hpp>

#include <YeloLib/Halo1/units/units_yelo.hpp>

#include "Memory/MemoryInterface.hpp"
#include "Objects/Units.hpp"

namespace Yelo
{
	namespace Objects { namespace Units { namespace Animations
	{
		API_FUNC_NAKED bool PLATFORM_API UnitAnimationStateInterruptable()
		{
			API_FUNC_NAKED_START()
				push	edx
				push	ecx

				push	edx
				push	ecx
				call	blam::unit_animation_state_interruptable
				add		esp, 8

				pop		ecx
				pop		edx
				pop		ebp
			API_FUNC_NAKED_END_()
		}

		API_FUNC_NAKED bool PLATFORM_API UnitAnimationBusy()
		{
			API_FUNC_NAKED_START()
				push	edx
				push	ecx

				push	ecx
				call	blam::unit_animation_busy
				add		esp, 4

				pop		ecx
				pop		edx
				pop		ebp
			API_FUNC_NAKED_END_()
		}

		API_FUNC_NAKED bool PLATFORM_API UnitAnimationStateLoops()
		{
			API_FUNC_NAKED_START()
				push	edx
				push	ecx

				push	ecx
				call	blam::unit_animation_state_loops
				add		esp, 4

				pop		ecx
				pop		edx
				pop		ebp
			API_FUNC_NAKED_END_()
		}

		API_FUNC_NAKED bool PLATFORM_API UnitAnimationWeaponIK()
		{
			API_FUNC_NAKED_START()
				push	edx
				push	ecx

				push	ecx
				call	blam::unit_animation_weapon_ik
				add		esp, 4

				pop		ecx
				pop		edx
				pop		ebp
			API_FUNC_NAKED_END_()
		}

		API_FUNC_NAKED bool PLATFORM_API UnitAnimationVehicleIK()
		{
			API_FUNC_NAKED_START()
				push	edx
				push	ecx

				push	ecx
				call	blam::unit_animation_vehicle_ik
				add		esp, 4

				pop		ecx
				pop		edx
				pop		ebp
			API_FUNC_NAKED_END_()
		}

		API_FUNC_NAKED void AnimationStateDefinedKeyframeHook()
		{
			static uintptr_t STOCK_ANIMATION_RETN = GET_FUNC_PTR(ANIMATION_STATE_DEFINED_KEYFRAME_STOCK_RETN);
			static uintptr_t SKIP_ANIMATION_RETN = GET_FUNC_PTR(ANIMATION_STATE_DEFINED_KEYFRAME_SKIP_RETN);
		
			static int32 STATE_COUNT_STOCK = Enums::_unit_animation_state;
			static int32 STATE_COUNT_YELO = Enums::_unit_animation_state_yelo;

			_asm
			{
				movsx	eax, byte ptr [esi + 2A3h]
				cmp		eax, STATE_COUNT_STOCK
				jl		stock_animation
			
				cmp		eax, STATE_COUNT_YELO
				jge		skip_animation
			
				push	eax
				push	eax
				push	ebx
				call	AnimationStateDefinedKeyframe
				add		esp, 8
				pop		eax

			skip_animation:
				jmp		SKIP_ANIMATION_RETN
			stock_animation:
				jmp		STOCK_ANIMATION_RETN
			};
		}
	
		API_FUNC_NAKED void AnimationStateFinalKeyframeHook()
		{
			static uintptr_t STOCK_ANIMATION_RETN = GET_FUNC_PTR(ANIMATION_STATE_FINAL_KEYFRAME_STOCK_RETN);
			static uintptr_t SKIP_ANIMATION_RETN = GET_FUNC_PTR(ANIMATION_STATE_FINAL_KEYFRAME_SKIP_RETN);
		
			static int32 STATE_COUNT_STOCK = Enums::_unit_animation_state;
			static int32 STATE_COUNT_YELO = Enums::_unit_animation_state_yelo;

			_asm
			{
				movsx	eax, byte ptr [esi + 2A3h]
				cmp		eax, STATE_COUNT_STOCK
				jl		stock_animation
			
				cmp		eax, STATE_COUNT_YELO
				jge		skip_animation
			
				push	eax
				push	eax
				push	ebx
				call	AnimationStateFinalKeyframe
				add		esp, 8
				pop		eax

			skip_animation:
				jmp		SKIP_ANIMATION_RETN
			stock_animation:
				jmp		STOCK_ANIMATION_RETN
			};
		}

		void Initialize()
		{
			Memory::WriteRelativeJmp(&AnimationStateDefinedKeyframeHook, GET_FUNC_VPTR(ANIMATION_STATE_DEFINED_KEYFRAME_HOOK), true);
			Memory::WriteRelativeJmp(&AnimationStateFinalKeyframeHook, GET_FUNC_VPTR(ANIMATION_STATE_FINAL_KEYFRAME_HOOK), true);

			for(auto ptr : K_UNIT_ANIMATION_STATE_INTERRUPTABLE_CALLS)
			{
				Memory::WriteRelativeCall(&UnitAnimationStateInterruptable, ptr, true);
			}
			for(auto ptr : K_UNIT_ANIMATION_BUSY_CALLS)
			{
				Memory::WriteRelativeCall(&UnitAnimationBusy, ptr, true);
			}

			Memory::WriteRelativeCall(&UnitAnimationStateLoops, GET_FUNC_VPTR(UNIT_ANIMATION_STATE_LOOPS_CALL), true);
			Memory::WriteRelativeCall(&UnitAnimationWeaponIK, GET_FUNC_VPTR(UNIT_ANIMATION_WEAPON_IK_CALL), true);
			Memory::WriteRelativeCall(&UnitAnimationVehicleIK, GET_FUNC_VPTR(UNIT_ANIMATION_VEHICLE_IK_CALL), true);
		}

		void Dispose() { }
	};};};
};