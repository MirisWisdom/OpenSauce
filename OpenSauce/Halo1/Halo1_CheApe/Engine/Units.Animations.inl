/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

#if PLATFORM_TYPE == PLATFORM_SAPIEN

#include <YeloLib/Halo1/units/units_yelo.hpp>

namespace Yelo
{
	namespace Objects { namespace Units { namespace Animations
	{
		FUNC_PTR(ANIMATION_STATE_DEFINED_KEYFRAME_HOOK,			PTR_NULL, PTR_NULL, 0x58EF52);
		FUNC_PTR(ANIMATION_STATE_DEFINED_KEYFRAME_STOCK_RETN,	PTR_NULL, PTR_NULL, 0x58EF59);
		FUNC_PTR(ANIMATION_STATE_DEFINED_KEYFRAME_SKIP_RETN,	PTR_NULL, PTR_NULL, 0x58F161);
		FUNC_PTR(ANIMATION_STATE_FINAL_KEYFRAME_HOOK,			PTR_NULL, PTR_NULL, 0x58EFA7);
		FUNC_PTR(ANIMATION_STATE_FINAL_KEYFRAME_STOCK_RETN,		PTR_NULL, PTR_NULL, 0x58EFAE);
		FUNC_PTR(ANIMATION_STATE_FINAL_KEYFRAME_SKIP_RETN,		PTR_NULL, PTR_NULL, 0x58F14D);
		
		void* K_UNIT_ANIMATION_STATE_INTERRUPTABLE_CALLS[] =
		{
			CAST_PTR(void*, 0x58EED5),
			CAST_PTR(void*, 0x58F212),
			CAST_PTR(void*, 0x58F760),
		};
		void* K_UNIT_ANIMATION_BUSY_CALLS[] =
		{
			CAST_PTR(void*, 0x5923CD),
			CAST_PTR(void*, 0x5931B5),
			CAST_PTR(void*, 0x593213),
			CAST_PTR(void*, 0x593A1A),
			CAST_PTR(void*, 0x595430),
		};

		FUNC_PTR(UNIT_ANIMATION_STATE_LOOPS_CALL,				PTR_NULL, PTR_NULL, 0x58F153);
		FUNC_PTR(UNIT_ANIMATION_WEAPON_IK_CALL,					PTR_NULL, PTR_NULL, 0x58EC63);
		FUNC_PTR(UNIT_ANIMATION_VEHICLE_IK_CALL,				PTR_NULL, PTR_NULL, 0x58EC02);

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
				movsx	eax, byte ptr [edi + 2A3h]
				cmp		eax, STATE_COUNT_STOCK
				jl		stock_animation
			
				cmp		eax, STATE_COUNT_YELO
				jge		skip_animation
			
				push	eax

				push	eax
				push	esi
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
				movsx	eax, byte ptr [edi + 2A3h]
				cmp		eax, STATE_COUNT_STOCK
				jl		stock_animation
			
				cmp		eax, STATE_COUNT_YELO
				jge		skip_animation
			
				push	eax

				push	eax
				push	esi
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
	};};};
};
#endif