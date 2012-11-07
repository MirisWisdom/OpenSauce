/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Game/EngineFunctions.hpp"

#include "Game/Console.hpp"
#include "Game/GameState.hpp"
#include "Game/ScriptLibrary.hpp"
#include "Networking/MDP.hpp"
#include "Objects/Objects.hpp"

// comment the following to just use the parameters as-is 
// instead of copying to the stack. I believe there was some 
// concurrency issues originally which caused me to use local 
// buffers instead...
#define ENGINE_FUNCTIONS_USE_LOCAL
enum { k_engine_function_string_buffer_size = 512, };

namespace Yelo
{
	namespace Engine
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_GAME
#define __EL_INCLUDE_FILE_ID	__EL_GAME_ENGINE_FUNCTIONS
#include "Memory/_EngineLayout.inl"

		#include "Game/EngineFunctions._Misc.inl"		

		namespace AI
		{
			void API_FUNC_NAKED Delete(datum_index actor_index, bool is_dead)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(ACTOR_DELETE);

				API_FUNC_NAKED_START()
					movzx	eax, is_dead
					push	eax
					mov		ebx, actor_index
					call	TEMP_CALL_ADDR
					add		esp, 4 * 1
				API_FUNC_NAKED_END(2)
			}

			void API_FUNC_NAKED AttachFree(datum_index unit_index, datum_index actor_variant_definition)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(AI_SCRIPTING_ATTACH_FREE);

				API_FUNC_NAKED_START()
					mov		eax, actor_variant_definition
					push	unit_index
					call	TEMP_CALL_ADDR
					add		esp, 4 * 1
				API_FUNC_NAKED_END(2)
			}
		};

		namespace Cheats
		{
			#include "Game/EngineFunctions.Cheats.inl"
		};

		namespace Console
		{
			#include "Game/EngineFunctions.Console.inl"
		};

		namespace Effects
		{
			void API_FUNC_NAKED NewOnObjectMarker(datum_index effect_definition_index, datum_index object_index, cstring marker_name)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(EFFECT_NEW_ON_OBJECT_MARKER);

				API_FUNC_NAKED_START()
					push	esi
					push	edi

					mov		ecx, marker_name
					push	ecx
					mov		esi, object_index
					mov		edi, effect_definition_index
					call	TEMP_CALL_ADDR
					add		esp, 4 * 1

					pop		edi
					pop		esi
				API_FUNC_NAKED_END(3)
			}
		};

		namespace Game
		{
			#include "Game/EngineFunctions.Game.inl"
		};

		namespace HS
		{
			void ObjectListAdd(datum_index object_list, datum_index object_index)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(OBJECT_LIST_ADD);

				if(object_list.IsNull()) return;

				__asm {
					push	object_index
					mov		eax, object_list
					call	TEMP_CALL_ADDR
					add		esp, 4 * 1
				}
			}
		};

		namespace Input
		{
			bool KeyIsDown(int16 key)
			{
#if !PLATFORM_IS_DEDI
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(INPUT_KEY_IS_DOWN);

				__asm {
					mov		cx, key
					call	TEMP_CALL_ADDR
				}
#else
				return false;
#endif
			}
		};

		namespace Interface
		{
			#include "Game/EngineFunctions.Interface.inl"
		};

		namespace Math
		{
			double PeriodicFunctionEvaluate(Enums::periodic_function function_type, double input)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(PERIODIC_FUNCTION_EVALUATE);

				__asm {
					fld		input
					sub		esp, 4 * 2			// allocate space for the 'input' parameter
					fstp	qword ptr [esp]		// store the input on the stack
					movzx	eax, function_type
					call	TEMP_CALL_ADDR
					add		esp, 4 * 2			// deallocate. double type consumes two DWORDs of stack
				}
			}

			float TransitionFunctionEvaluate(Enums::transition_function function_type, float input)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(TRANSITION_FUNCTION_EVALUATE);

				__asm {
					push	input
					movzx	ecx, function_type
					call	TEMP_CALL_ADDR
					add		esp, 4 * 1
				}
			}
		};

		namespace Memory
		{
			#include "Game/EngineFunctions.Memory.inl"
		};

		namespace Networking
		{
			#include "Game/EngineFunctions.Networking.inl"
		};

		namespace Objects
		{
			#include "Game/EngineFunctions.Objects.inl"
		};

		namespace Physics
		{
			API_FUNC_NAKED bool PLATFORM_API CollisionTestVector(long_flags flags, real_point3d& location, real_vector3d& vector, 
				datum_index object_index, Yelo::Physics::s_collision_result& collision)
			{
				static const uintptr_t JMP_ADDR = GET_FUNC_PTR(COLLISION_TEST_VECTOR);

				__asm jmp	JMP_ADDR
			}
		};

		namespace Players
		{
			#include "Game/EngineFunctions.Players.inl"
		};

		namespace Scenario
		{
			bool TriggerVolumeTestPoint(int32 trigger_volume_index, const real_point3d& point)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(SCENARIO_TRIGGER_VOLUME_TEST_POINT);

				__asm {
					mov		ecx, point
					mov		eax, trigger_volume_index
					call	TEMP_CALL_ADDR
				}
			}

			bool TriggerVolumeTestObject(int32 trigger_volume_index, datum_index object)
			{
				if(object.IsNull())
					return false;

				Yelo::Objects::s_object_data* obj = (*Yelo::Objects::ObjectHeader())[object]->_object;

				return TriggerVolumeTestPoint(trigger_volume_index, obj->center);
			}
		};

		namespace TagGroups
		{
			void PredictResources(TagBlock<Yelo::TagGroups::predicted_resource>& resources_block)
			{
#if !PLATFORM_IS_DEDI // dedi doesn't need to predict resources since it doesn't render bitmaps or play sounds
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(PHYSICAL_MEMORY_MAP_PREDICT_RESOURCES);

				__asm {
					push	esi

					mov		esi, resources_block
					call	TEMP_CALL_ADDR

					pop		esi
				}
#endif
			}

			datum_index TagLoaded(tag group_tag, cstring name)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(TAG_LOADED);

#if defined(ENGINE_FUNCTIONS_USE_LOCAL)
				char local[256];
				memset(local, 0, sizeof(local));
				strcpy_s(local, name);
#else
				cstring local = name;
#endif

				__asm {
					push	edi

#if defined(ENGINE_FUNCTIONS_USE_LOCAL)
					lea		eax, local
#else
					mov		eax, local
#endif
					push	eax
					mov		edi, group_tag
					call	TEMP_CALL_ADDR
					add		esp, 4 * 1

					pop		edi
				}
			}

			wcstring UnicodeStringListGetString(datum_index ustr, int32 index)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(UNICODE_STRING_LIST_GET_STRING);

				__asm {
					mov		edx, index
					mov		ecx, ustr
					call	TEMP_CALL_ADDR
				}
			}
		};
	};
};