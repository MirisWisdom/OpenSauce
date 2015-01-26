/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

#include <blamlib/Halo1/units/unit_structures.hpp>

#include <YeloLib/Halo1/units/units_yelo.hpp>

#include "Memory/MemoryInterface.hpp"
#include "Objects/Units.hpp"

namespace Yelo { namespace Objects { namespace Units {

namespace Animations
{
	static bool UnitAnimationStateInterruptableHook()
	{
		const s_unit_datum_animation_data* unit_animation;
		_enum next_animation_state;

		_asm mov	next_animation_state, dx;
		_asm mov	unit_animation, ecx;

		return blam::unit_animation_state_interruptable(*unit_animation, next_animation_state);
	}

	static bool UnitAnimationBusyHook()
	{
		const s_unit_datum_animation_data* unit_animation;

		_asm mov	unit_animation, ecx;

		return blam::unit_animation_busy(*unit_animation);
	}

	static bool UnitAnimationStateLoopsHook()
	{
		const s_unit_datum_animation_data* unit_animation;

		_asm mov	unit_animation, ecx;

		return blam::unit_animation_state_loops(*unit_animation);
	}

	static bool UnitAnimationWeaponIKHook()
	{
		const s_unit_datum_animation_data* unit_animation;

		_asm mov	unit_animation, ecx;

		return blam::unit_animation_weapon_ik(*unit_animation);
	}

	static bool UnitAnimationVehicleIKHook()
	{
		const s_unit_datum_animation_data* unit_animation;

		_asm mov	unit_animation, ecx;

		return blam::unit_animation_vehicle_ik(*unit_animation);
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
			Memory::WriteRelativeCall(&UnitAnimationStateInterruptableHook, ptr, true);
		for(auto ptr : K_UNIT_ANIMATION_BUSY_CALLS)
			Memory::WriteRelativeCall(&UnitAnimationBusyHook, ptr, true);
		Memory::WriteRelativeCall(&UnitAnimationStateLoopsHook, GET_FUNC_VPTR(UNIT_ANIMATION_STATE_LOOPS_CALL), true);
		Memory::WriteRelativeCall(&UnitAnimationWeaponIKHook, GET_FUNC_VPTR(UNIT_ANIMATION_WEAPON_IK_CALL), true);
		Memory::WriteRelativeCall(&UnitAnimationVehicleIKHook, GET_FUNC_VPTR(UNIT_ANIMATION_VEHICLE_IK_CALL), true);
	}

	void Dispose()
	{
	}
};

}; }; };