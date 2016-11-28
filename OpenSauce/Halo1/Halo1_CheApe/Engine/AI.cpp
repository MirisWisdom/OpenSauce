/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Engine/AI.hpp"

#include <blamlib/Halo1/objects/objects.hpp>
#include <blamlib/Halo1/objects/damage.hpp>

#if PLATFORM_TYPE == PLATFORM_SAPIEN
#include <YeloLib/Halo1/ai/ai_yelo.hpp>
#endif

#include "Engine/AI.Transform.inl"

namespace Yelo
{
	namespace AI
	{
#if PLATFORM_TYPE == PLATFORM_SAPIEN
		FUNC_PTR(ACTOR_ACTION_HANDLE_VEHICLE_EXIT_HOOK,		PTR_NULL, PTR_NULL, 0x701C7C);
		FUNC_PTR(ACTOR_ACTION_HANDLE_VEHICLE_EXIT_RETN,		PTR_NULL, PTR_NULL, 0x701C81);
		FUNC_PTR(PROP_STATUS_REFRESH_HOOK,					PTR_NULL, PTR_NULL, 0x6A21A2);
		FUNC_PTR(PROP_STATUS_REFRESH_RETN,					PTR_NULL, PTR_NULL, 0x6A21A8);
		FUNC_PTR(ACTOR_INPUT_UPDATE_HOOK,					PTR_NULL, PTR_NULL, 0x698405);
		FUNC_PTR(ACTOR_INPUT_UPDATE_RETN,					PTR_NULL, PTR_NULL, 0x69840C);

		API_FUNC_NAKED void ActorActionHandleVehicleExitHook()
		{
			static uintptr_t RETN_ADDRESS = GET_FUNC_PTR(ACTOR_ACTION_HANDLE_VEHICLE_EXIT_RETN);

			_asm
			{
				push	eax
				push	edx
				xor		ecx, ecx
				mov		cl, [eax + 60h]

				push	ecx
				push	eax
				push	esi
				call	AI::ActorPropShouldCauseExitVehicle
				add		esp, 0Ch

				mov		cl, al
				pop		edx
				pop		eax

				test    cl, cl

				jmp		RETN_ADDRESS
			};
		}

		API_FUNC_NAKED void PropStatusRefreshHook()
		{
			static uintptr_t RETN_ADDRESS = GET_FUNC_PTR(PROP_STATUS_REFRESH_RETN);

			_asm
			{
				push	eax
				push	ecx
				push	edx

				push	esi
				call	AI::ActorShouldIgnoreSeatedProp
				add		esp, 4
				
				pop		edx
				pop		ecx

				or		cl, al
				mov		[esi + 133h], cl

				pop		eax
				jmp		RETN_ADDRESS
			}
		}

		API_FUNC_NAKED void ActorInputUpdateHook()
		{
			static uintptr_t RETN_ADDRESS = GET_FUNC_PTR(ACTOR_INPUT_UPDATE_RETN);

			_asm
			{
				push	eax

				push	edi
				call	AI::ActorShouldPanicAboutMountedUnit
				add		esp, 4

				mov		byte ptr [esi + 1B4h], al

				pop		eax
				jmp		RETN_ADDRESS
			}
		}
#endif

		void Initialize()
		{
#if PLATFORM_TYPE != PLATFORM_GUERILLA
			Transform::InitialiseTagBlockProcedures();
#endif

#if PLATFORM_TYPE == PLATFORM_SAPIEN
			Transform::Initialize();

			Memory::WriteRelativeJmp(&ActorActionHandleVehicleExitHook, GET_FUNC_VPTR(ACTOR_ACTION_HANDLE_VEHICLE_EXIT_HOOK), true);
			Memory::WriteRelativeJmp(&PropStatusRefreshHook, GET_FUNC_VPTR(PROP_STATUS_REFRESH_HOOK), true);
			Memory::WriteRelativeJmp(&ActorInputUpdateHook, GET_FUNC_VPTR(ACTOR_INPUT_UPDATE_HOOK), true);
#endif
		}

		void Dispose() { }
		
#if PLATFORM_TYPE == PLATFORM_SAPIEN
		void InitializeForNewGameState()
		{
			Transform::InitializeForNewGameState();
		}

		void InitializeForNewMap()
		{
			Transform::InitializeForNewMap();
		}

		void DisposeFromOldMap()
		{
			Transform::DisposeFromOldMap();
		}

		void ObjectsUpdate()
		{
			Objects::c_object_iterator iter(Enums::_object_type_mask_unit);

			for(auto object_index : iter)
			{
				Transform::UnitUpdate(object_index.index);
			}
		}

		void UnitDamageAftermath(const datum_index object_index, const Objects::s_damage_data* damage_data)
		{
			Transform::UnitDamaged(object_index, damage_data);
		}
#endif
	};
};