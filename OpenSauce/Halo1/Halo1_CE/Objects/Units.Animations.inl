/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

#include <blamlib/Halo1/units/unit_structures.hpp>

#include "Memory/MemoryInterface.hpp"
#include "Objects/Units.hpp"

namespace Yelo { namespace Objects { namespace Units {

namespace Animations
{
	typedef void (*animation_state_primary_keyframe_handler_t)(const datum_index);
	typedef void (*animation_state_final_keyframe_handler_t)(const datum_index);

	struct s_animation_state_handler
	{
		animation_state_primary_keyframe_handler_t m_primary_keyframe_handler;
		animation_state_final_keyframe_handler_t m_final_keyframe_handler;
	};

	static s_animation_state_handler g_animation_state_handlers[] =
	{
		//_unit_animation_state_yelo_seat_board
		{
			&Units::SeatBoarding::AnimationStateBoardPrimaryKeyframeHandler,
			&Units::SeatBoarding::AnimationStateBoardFinalKeyframeHandler,
		},
		//_unit_animation_state_yelo_seat_ejection
		{
			&Units::SeatBoarding::AnimationStateBoardPrimaryKeyframeHandler,
			&Units::SeatBoarding::AnimationStateBoardFinalKeyframeHandler,
		}
	}; BOOST_STATIC_ASSERT(NUMBEROF(g_animation_state_handlers) == (Enums::_unit_animation_state_yelo - Enums::_unit_animation_state));

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

	void PLATFORM_API AnimationStatePrimaryKeyframe(const datum_index unit_index, const Enums::unit_animation_state state)
	{
		auto& handler = g_animation_state_handlers[state - Enums::_unit_animation_state];

		if(handler.m_primary_keyframe_handler)
		{
			handler.m_primary_keyframe_handler(unit_index);
		}
	}
	
	API_FUNC_NAKED void AnimationStatePrimaryKeyframeHook()
	{
		static uintptr_t STOCK_ANIMATION_RETN = 0x568A56;
		static uintptr_t SKIP_ANIMATION_RETN = 0x568C77;
		
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
			call	AnimationStatePrimaryKeyframe
			add		esp, 8
			pop		eax

		skip_animation:
			jmp		SKIP_ANIMATION_RETN
		stock_animation:
			jmp		STOCK_ANIMATION_RETN
		};
	}

	void PLATFORM_API AnimationStateFinalKeyframe(const datum_index unit_index, const Enums::unit_animation_state state)
	{
		auto& handler = g_animation_state_handlers[state - Enums::_unit_animation_state];

		if(handler.m_final_keyframe_handler)
		{
			handler.m_final_keyframe_handler(unit_index);
		}
	}
	
	API_FUNC_NAKED void AnimationStateFinalKeyframeHook()
	{
		static uintptr_t STOCK_ANIMATION_RETN = 0x568AAB;
		static uintptr_t SKIP_ANIMATION_RETN = 0x568C64;
		
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
		Memory::WriteRelativeJmp(&AnimationStatePrimaryKeyframeHook, CAST_PTR(void*, 0x568A4F), true);
		Memory::WriteRelativeJmp(&AnimationStateFinalKeyframeHook, CAST_PTR(void*, 0x568AA4), true);

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