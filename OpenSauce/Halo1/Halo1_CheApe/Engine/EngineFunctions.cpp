/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Engine/EngineFunctions.hpp"

#include <blamlib/Halo1/cache/physical_memory_map.hpp>
#include <blamlib/Halo1/cseries/profile.hpp>
#include <blamlib/Halo1/math/periodic_functions.hpp>
#include <blamlib/Halo1/memory/byte_swapping.hpp>
#include <blamlib/Halo1/memory/memory_pool.hpp>
#include <blamlib/Halo1/saved_games/game_state.hpp>
#include <blamlib/Halo1/tag_files/files.hpp>
#include <blamlib/Halo1/tag_files/tag_files.hpp>
#include <blamlib/Halo1/cryptography/md5.hpp>
#include <blamlib/Halo1/objects/objects.hpp>
#include <blamlib/Halo1/objects/damage.hpp>
#include <blamlib/Halo1/objects/object_structures.hpp>
#include <blamlib/Halo1/ai/actors.hpp>
#include <blamlib/Halo1/ai/actor_structures.hpp>
#include <blamlib/Halo1/ai/ai_structures.hpp>
#include <blamlib/Halo1/game/game_globals.hpp>
#include <blamlib/Halo1/game/game_globals_structures.hpp>

#include <YeloLib/tag_files/tag_groups_base_yelo.hpp>
#include <YeloLib/Halo1/saved_games/game_state_yelo.hpp>

#include "Engine/Objects.hpp"

namespace Yelo
{
	namespace Enums
	{
		enum unit_animation_state : _enum;
		enum first_person_weapon_animation : _enum;
		enum unit_replacement_animation_state : _enum;
	};
	namespace GameState
	{
		struct s_main_globals;
	};
	namespace GameUI
	{
		struct s_first_person_weapons;
	};
	namespace Scenario
	{
		struct s_scenario_globals;
	};
	namespace TagGroups
	{
		struct predicted_resource;

		struct s_game_globals;
		struct scenario;

		struct collision_bsp;
		struct structure_bsp;
	};
#define __EL_INCLUDE_ID			__EL_INCLUDE_GAME
#define __EL_INCLUDE_FILE_ID	__EL_GAME_ENGINE_FUNCTIONS
#include "Memory/_EngineLayout.inl"
	
	//////////////////////////////////////////////////////////////////////////
	// ai
	namespace blam
	{
#if PLATFORM_TYPE == PLATFORM_SAPIEN
		//////////////////////////////////////////////////////////////////////////
		// actors.c
		API_FUNC_NAKED void PLATFORM_API actor_delete(datum_index actor_index, bool is_dead)
		{
			static uintptr_t FUNCTION = GET_FUNC_PTR(ACTOR_DELETE);

			_asm jmp	FUNCTION;
		}

		API_FUNC_NAKED void PLATFORM_API actor_customize_unit(const datum_index actor_variant, const datum_index unit_index)
		{
			static uintptr_t FUNCTION = GET_FUNC_PTR(ACTOR_CUSTOMIZE_UNIT);

			_asm jmp	FUNCTION;
		}

		API_FUNC_NAKED void PLATFORM_API actor_braindead(const datum_index actor_index, const bool braindead)
		{
			static uintptr_t FUNCTION = GET_FUNC_PTR(ACTOR_BRAINDEAD);

			_asm jmp	FUNCTION;
		}

		API_FUNC_NAKED datum_index PLATFORM_API actor_create_for_unit(const bool is_swarm
			, const datum_index unit_index
			, const datum_index actor_variant
			, const datum_index encounter_index
			, const int32 squad_index
			, const int32 arg7
			, const int32 arg6
			, const bool magic_sight_after_timer
			, const Enums::actor_default_state initial_state
			, const Enums::actor_default_state return_state
			, const int32 command_list_index
			, const int32 sequence_id)
		{
			static uintptr_t FUNCTION = GET_FUNC_PTR(ACTOR_CREATE_FOR_UNIT);

			_asm jmp	FUNCTION;
		}
#endif
	};
	//////////////////////////////////////////////////////////////////////////
	// cseries
	namespace blam
	{
		//////////////////////////////////////////////////////////////////////////
		// cseries.c
		API_FUNC_NAKED void PLATFORM_API display_assert(cstring reason, cstring file, const uint32 line, bool halt)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DISPLAY_ASSERT);

			__asm	jmp	FUNCTION
		}
		//////////////////////////////////////////////////////////////////////////
		// debug_memory.c
		API_FUNC_NAKED void PLATFORM_API debug_dump_memory()
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DEBUG_DUMP_MEMORY);

			__asm	jmp	FUNCTION
		}

		API_FUNC_NAKED void* PLATFORM_API debug_malloc(const size_t ptr_size, const bool fill_with_garbage, cstring file, const uint32 line)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DEBUG_MALLOC);

			__asm	jmp	FUNCTION
		}

		API_FUNC_NAKED void PLATFORM_API debug_free(void* pointer, cstring file, const uint32 line)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DEBUG_FREE);

			__asm	jmp	FUNCTION
		}

		API_FUNC_NAKED void* PLATFORM_API debug_realloc(void* pointer, const size_t new_size, cstring file, const uint32 line)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DEBUG_REALLOC);

			__asm	jmp	FUNCTION
		}
		//////////////////////////////////////////////////////////////////////////
		// errrors.c
		API_FUNC_NAKED bool PLATFORM_API errors_handle()
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(ERRORS_HANDLE);

			__asm	jmp	FUNCTION
		}
		//////////////////////////////////////////////////////////////////////////
		// cseries/profile
		bool profiling_enabled()	PTR_IMP_GET(g_profiling_enabled);
		API_FUNC_NAKED void PLATFORM_API profile_enter_private(Debug::s_profile_section& section)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DEBUG_FREE);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED void PLATFORM_API profile_exit_private(Debug::s_profile_section& section)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DEBUG_FREE);

			__asm	jmp	FUNCTION
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// cache
	namespace blam
	{
		//////////////////////////////////////////////////////////////////////////
		// cache_file_builder.c
#if PLATFORM_TYPE == PLATFORM_TOOL
		API_FUNC_NAKED bool PLATFORM_API scenario_load_all_structure_bsps()
		{
			static const uintptr_t FUNCTION = 0x454090;

			__asm	jmp	FUNCTION
		}
#endif
		//////////////////////////////////////////////////////////////////////////
		// predicted_resources.c
#if PLATFORM_TYPE == PLATFORM_TOOL
		void predicted_resources_add_resource(TagBlock<TagGroups::predicted_resource>& predicted_resources,
			long_enum resource_type, datum_index tag_index, int32 resource_index)
		{
			static void* PREDICTED_RESOURCES_ADD_RESOURCE = CAST_PTR(void*, 0x4B94E0);

			typedef void (PLATFORM_API* call_proc)(TagBlock<TagGroups::predicted_resource>&, long_enum, datum_index, int32);
			static call_proc add_predicted_resource = CAST_PTR(call_proc, PREDICTED_RESOURCES_ADD_RESOURCE);

			add_predicted_resource(predicted_resources, resource_type, tag_index, resource_index);
		}
#endif
	};
	//////////////////////////////////////////////////////////////////////////
	// game
	namespace blam
	{
#if PLATFORM_TYPE == PLATFORM_SAPIEN
		//////////////////////////////////////////////////////////////////////////
		// game_allegiance.c
		API_FUNC_NAKED bool PLATFORM_API game_team_is_enemy(long_enum team, long_enum team_to_test)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(GAME_TEAM_IS_ENEMY);

			__asm	jmp	FUNCTION
		}
#endif
	};
	//////////////////////////////////////////////////////////////////////////
	// hs
	namespace blam
	{
		//////////////////////////////////////////////////////////////////////////
		// hs/hs_scenario_definitions.c
		API_FUNC_NAKED bool PLATFORM_API hs_scenario_postprocess(bool for_runtime)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(HS_SCENARIO_POSTPROCESS);

			__asm	jmp	FUNCTION
		}
		
#if PLATFORM_TYPE == PLATFORM_SAPIEN
		API_FUNC_NAKED void PLATFORM_API hs_effect_new_from_object_marker(datum_index effect_definition_index, datum_index object_index, cstring marker_name)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(HS_EFFECT_NEW_FROM_OBJECT_MARKER);

			__asm	jmp	FUNCTION
		}
#endif
	};
	//////////////////////////////////////////////////////////////////////////
	// interface
	namespace blam
	{
		//////////////////////////////////////////////////////////////////////////
		// interface/ui_widget_group.c
		API_FUNC_NAKED void PLATFORM_API ui_load_tags_for_scenario(datum_index scenario_index)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(UI_LOAD_TAGS_FOR_SCENARIO);

			__asm	jmp	FUNCTION
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// items
	namespace blam
	{
#if PLATFORM_TYPE == PLATFORM_SAPIEN
		//////////////////////////////////////////////////////////////////////////
		// items/weapons.c
		API_FUNC_NAKED bool PLATFORM_API weapon_prevents_melee_attack(const datum_index weapon_index)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(WEAPON_PREVENTS_MELEE_ATTACK);

			__asm	jmp	FUNCTION
		}

		API_FUNC_NAKED bool PLATFORM_API weapon_prevents_grenade_throwing(const datum_index weapon_index)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(WEAPON_PREVENTS_GRENADE_THROWING);

			__asm	jmp	FUNCTION
		}

		API_FUNC_NAKED void PLATFORM_API weapon_stop_reload(const datum_index weapon_index)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(WEAPON_STOP_RELOAD);

			__asm	jmp	FUNCTION
		}

		API_FUNC_NAKED void PLATFORM_API first_person_weapon_message_from_unit(const datum_index unit_index, const int32 weapon_message_type)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FIRST_PERSON_WEAPON_MESSAGE_FROM_UNIT);

			__asm	jmp	FUNCTION
		}

		API_FUNC_NAKED int16 PLATFORM_API weapon_get_first_person_animation_time(const datum_index weapon_index
			, const int16 frame_type
			, Enums::first_person_weapon_animation animation
			, const int32 arg3)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(WEAPON_GET_FIRST_PERSON_ANIMATION_TIME);

			__asm	jmp	FUNCTION
		}
#endif
	}
	//////////////////////////////////////////////////////////////////////////
	// main
	namespace blam
	{
#if PLATFORM_TYPE != PLATFORM_GUERILLA
		API_FUNC_NAKED void PLATFORM_API console_printf(bool clear_screen, cstring format, ...)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(CONSOLE_PRINTF);

			__asm	jmp	FUNCTION
		}

		API_FUNC_NAKED void PLATFORM_API console_warning(cstring format, ...)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(CONSOLE_WARNING);

			__asm	jmp	FUNCTION
		}
#endif
	};
	//////////////////////////////////////////////////////////////////////////
	// math
	namespace blam
	{
		//////////////////////////////////////////////////////////////////////////
		// periodic_functions.c
		real PLATFORM_API periodic_function_evaluate(Enums::periodic_function function_type, real input)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(PERIODIC_FUNCTION_EVALUATE);

			__asm {
				fld		input
				sub		esp, 4 * 2			// allocate space for the 'input' parameter
				fstp	qword ptr [esp]		// store the input on the stack (as a double)
				push	function_type
				call	FUNCTION
				add		esp, 4 * (1+2)		// deallocate. double type consumes two DWORDs of stack
			}
		}
		real PLATFORM_API transition_function_evaluate(Enums::transition_function function_type, real input)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TRANSITION_FUNCTION_EVALUATE);

			__asm {
				push	input
				push	function_type
				call	FUNCTION
				add		esp, 4 * 2
			}
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// memory
	namespace blam
	{
		using namespace Yelo::Memory;

		//////////////////////////////////////////////////////////////////////////
		// byte_swapping.c
		void PLATFORM_API byte_swap_data(Memory::s_byte_swap_definition* definition, void* address,
			int32 data_count)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(BYTE_SWAP_DATA);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED void PLATFORM_API byte_swap_data_explicit(cstring name, int32 size, 
			byte_swap_code_t* codes, int data_count, void *address)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(BYTE_SWAP_DATA_EXPLICIT);

			__asm	jmp	FUNCTION
		}

		//////////////////////////////////////////////////////////////////////////
		// data.c
		API_FUNC_NAKED void* PLATFORM_API datum_try_and_get(s_data_array* data, datum_index index)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DATUM_TRY_AND_GET);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED void* PLATFORM_API datum_get(s_data_array* data, datum_index index)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DATUM_GET);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED void PLATFORM_API data_verify(const s_data_array* data)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DATA_VERIFY);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED s_data_array* PLATFORM_API data_new(cstring name, int32 maximum_count, size_t datum_size)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DATA_NEW);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED void PLATFORM_API data_dispose(s_data_array* data)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DATA_DISPOSE);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED datum_index PLATFORM_API datum_new_at_index(s_data_array* data, datum_index index)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DATUM_NEW_AT_INDEX);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED datum_index PLATFORM_API datum_new(s_data_array* data)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DATUM_NEW);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED void PLATFORM_API datum_delete(s_data_array* data, datum_index index)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DATUM_DELETE);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED void PLATFORM_API data_delete_all(s_data_array* data)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DATA_DELETE_ALL);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED datum_index PLATFORM_API data_next_index(const s_data_array* data, datum_index cursor)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DATA_NEXT_INDEX);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED void PLATFORM_API data_make_valid(s_data_array* data)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(DATA_MAKE_VALID);

			__asm	jmp	FUNCTION
		}

		//////////////////////////////////////////////////////////////////////////
		// memory_pool.c
		API_FUNC_NAKED void PLATFORM_API memory_pool_initialize(Memory::s_memory_pool* pool, cstring name, int32 pool_size)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(MEMORY_POOL_INITIALIZE);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED void PLATFORM_API memory_pool_verify(const Memory::s_memory_pool* pool)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(MEMORY_POOL_VERIFY);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED Memory::s_memory_pool* PLATFORM_API memory_pool_new(cstring name, int32 pool_size)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(MEMORY_POOL_NEW);

			__asm	jmp	FUNCTION
		}
		PLATFORM_VALUE(API_FUNC_NAKED, , API_FUNC_NAKED) void PLATFORM_API memory_pool_delete(Memory::s_memory_pool* pool)
		{
#if PLATFORM_TYPE == PLATFORM_TOOL
			memory_pool_verify(pool);
			memset(pool, 0, sizeof(*pool));
			YELO_FREE(pool); // FREE is preferred here over DELETE since the type is a POD engine type
#else
			static const uintptr_t FUNCTION = GET_FUNC_PTR(MEMORY_POOL_DELETE);

			__asm	jmp	FUNCTION
#endif
		}
		API_FUNC_NAKED bool PLATFORM_API memory_pool_block_allocate(Memory::s_memory_pool* pool, _Inout_ s_memory_pool_block::reference_t reference, int32 size)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(MEMORY_POOL_BLOCK_ALLOCATE);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED void PLATFORM_API memory_pool_block_free(Memory::s_memory_pool* pool, _Inout_ s_memory_pool_block::reference_t reference)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(MEMORY_POOL_BLOCK_FREE);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED void PLATFORM_API memory_pool_defragment(Memory::s_memory_pool* pool)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(MEMORY_POOL_DEFRAGMENT);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED bool PLATFORM_API memory_pool_block_reallocate(Memory::s_memory_pool* pool, _Inout_ s_memory_pool_block::reference_t reference, int32 new_size)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(MEMORY_POOL_BLOCK_REALLOCATE);

			__asm	jmp	FUNCTION
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// models
	namespace blam
	{
#if PLATFORM_TYPE == PLATFORM_SAPIEN
		API_FUNC_NAKED int16 PLATFORM_API model_find_marker(const datum_index render_model_definition_index, cstring marker_name)
		{
			static uintptr_t FUNCTION = GET_FUNC_PTR(MODEL_FIND_MARKER);

			_asm jmp	FUNCTION;
		}

		API_FUNC_NAKED int16 PLATFORM_API animation_choose_random_permutation_internal(long_enum render_or_affects_game_state
			, datum_index animation_graph_index
			, int32 animation_index)
		{
			static uintptr_t FUNCTION = GET_FUNC_PTR(ANIMATION_CHOOSE_RANDOM_PERMUTATION_INTERNAL);

			_asm jmp	FUNCTION;
		}
#endif
	};
	//////////////////////////////////////////////////////////////////////////
	// objects
	namespace blam
	{
#if PLATFORM_TYPE == PLATFORM_SAPIEN
		API_FUNC_NAKED datum_index PLATFORM_API object_new(s_object_placement_data& data)
		{
			static uintptr_t FUNCTION = GET_FUNC_PTR(OBJECT_NEW);

			_asm jmp	FUNCTION;
		}

		API_FUNC_NAKED void PLATFORM_API object_delete(datum_index object_index)
		{
			static uintptr_t FUNCTION = GET_FUNC_PTR(OBJECT_DELETE);

			_asm jmp	FUNCTION;
		}
		
		s_object_data* object_get(datum_index object_index)
		{
			if(!Objects::ObjectHeader().Header.is_valid)
			{
				return nullptr;
			}

			return Objects::ObjectHeader()[object_index]->_object;
		}

		API_FUNC_NAKED void PLATFORM_API object_detach(datum_index object_index)
		{
			static uintptr_t FUNCTION = GET_FUNC_PTR(OBJECT_DETACH);

			_asm jmp	FUNCTION;
		}

		API_FUNC_NAKED void PLATFORM_API object_attach_to_marker(datum_index target_object_index, cstring target_marker_name, datum_index object_index, cstring marker_name)
		{
			static uintptr_t FUNCTION = GET_FUNC_PTR(OBJECT_ATTACH_TO_MARKER);

			_asm jmp	FUNCTION;
		}

		API_FUNC_NAKED void PLATFORM_API objects_update()
		{
			static uintptr_t FUNCTION = GET_FUNC_PTR(OBJECTS_UPDATE);

			_asm jmp	FUNCTION;
		}

		API_FUNC_NAKED void PLATFORM_API object_placement_data_new(s_object_placement_data& data, datum_index object_definition_index, datum_index owner_object_index)
		{
			static uintptr_t FUNCTION = GET_FUNC_PTR(OBJECT_PLACEMENT_DATA_NEW);

			_asm jmp	FUNCTION;
		}

		API_FUNC_NAKED s_object_data* PLATFORM_API object_iterator_next(s_object_iterator& iter)
		{
			static uintptr_t FUNCTION = GET_FUNC_PTR(OBJECT_ITERATOR_NEXT);

			_asm jmp	FUNCTION;
		}

		API_FUNC_NAKED real_point3d& PLATFORM_API object_get_origin(datum_index object_index, __out real_point3d& return_origin)
		{
			static uintptr_t FUNCTION = GET_FUNC_PTR(OBJECT_GET_ORIGIN);

			_asm jmp	FUNCTION;
		}

		API_FUNC_NAKED void PLATFORM_API object_cause_damage(s_damage_data& data
			, datum_index damaged_object_index
			, int16 node_index
			, int16 region_index
			, int16 damage_materials_element_index
			, real_vector3d* normal)
		{
			static uintptr_t FUNCTION = GET_FUNC_PTR(OBJECT_CAUSE_DAMAGE);

			_asm jmp	FUNCTION;
		}

		API_FUNC_NAKED int16 PLATFORM_API object_get_marker_by_name(const datum_index object_index
			, cstring marker_name
			, s_object_marker* markers
			, const int16 maximum_marker_count)
		{
			static uintptr_t FUNCTION = GET_FUNC_PTR(OBJECT_GET_MARKER_BY_NAME);

			_asm jmp	FUNCTION;
		}

		API_FUNC_NAKED void PLATFORM_API object_destroy(datum_index object_index)
		{
			static uintptr_t FUNCTION = GET_FUNC_PTR(OBJECT_DESTROY);

			_asm jmp	FUNCTION;
		}
#endif
	}
	//////////////////////////////////////////////////////////////////////////
	// save_games
	namespace blam
	{
#if PLATFORM_TYPE == PLATFORM_SAPIEN
		//////////////////////////////////////////////////////////////////////////
		// game_state.c
		API_FUNC_NAKED void* PLATFORM_API game_state_malloc(cstring name, cstring type, size_t size)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(GAME_STATE_MALLOC);

			__asm	jmp	FUNCTION
		}
#endif
	};
	//////////////////////////////////////////////////////////////////////////
	// scenario
	namespace blam
	{
		//////////////////////////////////////////////////////////////////////////
		// scenario/scenario
		API_FUNC_NAKED bool PLATFORM_API scenario_switch_structure_bsp(int16 bsp_index)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(SCENARIO_SWITCH_STRUCTURE_BSP);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED bool PLATFORM_API scenario_load(cstring scenario_name)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(SCENARIO_LOAD);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED void PLATFORM_API scenario_unload()
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(SCENARIO_UNLOAD);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED datum_index PLATFORM_API scenario_tags_load(cstring scenario_name)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(SCENARIO_TAGS_LOAD);

			__asm	jmp	FUNCTION
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// shell
	namespace blam
	{
		//////////////////////////////////////////////////////////////////////////
		// shell_windows.c
		API_FUNC_NAKED bool PLATFORM_API shell_get_command_line_argument(cstring param, _Out_opt_ cstring* value = nullptr)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(SHELL_GET_COMMAND_LINE_ARGUMENT);

			__asm	jmp	FUNCTION
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// tag_files
	namespace blam
	{
		//////////////////////////////////////////////////////////////////////////
		// files.c
		API_FUNC_NAKED void PLATFORM_API file_reference_create(s_file_reference& reference, long_enum location)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FILE_REFERENCE_CREATE);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED s_file_reference& PLATFORM_API file_reference_add_directory(s_file_reference& reference, cstring directory)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FILE_REFERENCE_ADD_DIRECTORY);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED s_file_reference& PLATFORM_API file_reference_set_name(s_file_reference& reference, cstring name)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FILE_REFERENCE_SET_NAME);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED char* PLATFORM_API file_reference_get_name(const s_file_reference& reference, long_flags flags, __out char name[Enums::k_maximum_filename_length+1])
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FILE_REFERENCE_GET_NAME);

			__asm	jmp	FUNCTION
		}

		API_FUNC_NAKED int16 PLATFORM_API find_files(long_flags flags, const s_file_reference& directory, int32 maximum_count, s_file_reference references[])
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FIND_FILES);

			__asm	jmp	FUNCTION
		}
		//////////////////////////////////////////////////////////////////////////
		// files_windows.c
		API_FUNC_NAKED void PLATFORM_API file_error(cstring operation, const s_file_reference& reference)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FILE_ERROR);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED bool PLATFORM_API file_create(const s_file_reference& reference)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FILE_CREATE);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED bool PLATFORM_API file_delete(const s_file_reference& reference)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FILE_DELETE);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED bool PLATFORM_API file_exists(const s_file_reference& reference)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FILE_EXISTS);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED bool PLATFORM_API file_open(s_file_reference& reference, long_flags flags)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FILE_OPEN);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED bool PLATFORM_API file_close(s_file_reference& reference)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FILE_CLOSE);

			__asm	jmp	FUNCTION
		}

		API_FUNC_NAKED bool PLATFORM_API file_set_position(s_file_reference& reference, uint32 position)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FILE_SET_POSITION);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED uint32 PLATFORM_API file_get_eof(const s_file_reference& reference)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FILE_GET_EOF);

			__asm	jmp	FUNCTION
		}

		API_FUNC_NAKED bool PLATFORM_API file_read(s_file_reference& reference, size_t buffer_size, void* buffer)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FILE_READ);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED bool PLATFORM_API file_write(s_file_reference& reference, size_t buffer_size, const void* buffer)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FILE_WRITE);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED bool PLATFORM_API file_read_from_position(s_file_reference& reference, uint32 position,
			size_t buffer_size, void* buffer)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FILE_READ_FROM_POSITION);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED bool PLATFORM_API file_write_to_position(const s_file_reference& reference, uint32 position,
			size_t buffer_size, const void* buffer)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FILE_WRITE_TO_POSITION);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED bool PLATFORM_API file_read_only(const s_file_reference& reference)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(FILE_READ_ONLY_);

			__asm	jmp	FUNCTION
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// units
	namespace blam
	{
#if PLATFORM_TYPE == PLATFORM_SAPIEN
		API_FUNC_NAKED void PLATFORM_API unit_kill(const datum_index unit_index)
		{
			static uintptr_t FUNCTION = GET_FUNC_PTR(UNIT_KILL);

			_asm jmp	FUNCTION;
		}
		
		int16 unit_get_custom_animation_time(datum_index unit_index)
		{
			return CAST_PTR(int16 (PLATFORM_API*)(const datum_index), GET_FUNC_PTR(UNIT_GET_CUSTOM_ANIMATION_TIME))(unit_index);
		}

		API_FUNC_NAKED bool PLATFORM_API unit_start_user_animation(const datum_index unit_index
			, const datum_index animation_definition_index
			, cstring animation_name
			, const bool interpolate)
		{
			static uintptr_t FUNCTION = GET_FUNC_PTR(UNIT_START_USER_ANIMATION);

			_asm jmp	FUNCTION;
		}

		API_FUNC_NAKED void PLATFORM_API unit_drop_current_weapon(const datum_index unit_index, const bool force)
		{
			static uintptr_t FUNCTION = GET_FUNC_PTR(UNIT_DROP_CURRENT_WEAPON);

			_asm jmp	FUNCTION;
		}

		API_FUNC_NAKED void PLATFORM_API unit_damage_aftermath(const datum_index unit_index
			, const Objects::s_damage_data* damage_data
			, const _enum damage_flags
			, const real shield_amount
			, const real body_amount
			, void* arg6
			, const int32 damage_part
			, const datum_index dead_unit_index)
		{
			static uintptr_t FUNCTION = GET_FUNC_PTR(UNIT_DAMAGE_AFTERMATH);

			_asm jmp	FUNCTION;
		}

		API_FUNC_NAKED bool PLATFORM_API unit_enter_seat(datum_index unit_index
			, datum_index target_unit_index
			, int32 seat_index)
		{
			static uintptr_t FUNCTION = GET_FUNC_PTR(UNIT_ENTER_SEAT);

			_asm jmp	FUNCTION;
		}

		API_FUNC_NAKED void PLATFORM_API unit_exit_seat_end(datum_index unit_index
			, bool no_network_message
			, bool can_run_on_client_side
			, bool )
		{
			static uintptr_t FUNCTION = GET_FUNC_PTR(UNIT_EXIT_SEAT_END);

			_asm jmp	FUNCTION;
		}

		API_FUNC_NAKED void PLATFORM_API unit_animation_set_state(const datum_index unit_index, const Enums::unit_animation_state state)
		{
			static uintptr_t FUNCTION = GET_FUNC_PTR(UNIT_ANIMATION_SET_STATE);

			_asm jmp	FUNCTION;
		}

		API_FUNC_NAKED void PLATFORM_API unit_ready_desired_weapon(const datum_index unit_index, const bool force)
		{
			static uintptr_t FUNCTION = GET_FUNC_PTR(UNIT_READY_DESIRED_WEAPON);

			_asm jmp	FUNCTION;
		}

		API_FUNC_NAKED void PLATFORM_API object_start_interpolation(datum_index object_index, int32 interpolation_frame_count)
		{
			static uintptr_t FUNCTION = GET_FUNC_PTR(OBJECT_START_INTERPOLATION);

			_asm jmp	FUNCTION;
		}

		API_FUNC_NAKED void PLATFORM_API unit_animation_start_action(const datum_index unit_index, const Enums::unit_replacement_animation_state action_type)
		{
			static uintptr_t FUNCTION = GET_FUNC_PTR(UNIT_ANIMATION_START_ACTION);

			_asm jmp	FUNCTION;
		}

		API_FUNC_NAKED datum_index PLATFORM_API unit_inventory_get_weapon(const datum_index unit_index, const int16 index)
		{
			static uintptr_t FUNCTION = GET_FUNC_PTR(UNIT_INVENTORY_GET_WEAPON);

			_asm jmp	FUNCTION;
		}

		API_FUNC_NAKED void PLATFORM_API unit_throw_grenade_release(const datum_index unit_index, const sbyte keyframe)
		{
			static uintptr_t FUNCTION = GET_FUNC_PTR(UNIT_THROW_GRENADE_RELEASE);

			_asm jmp	FUNCTION;
		}

		API_FUNC_NAKED void PLATFORM_API unit_set_animation(datum_index unit_index
			, datum_index animation_graph_index
			, int32 animation_index)
		{
			static uintptr_t FUNCTION = GET_FUNC_PTR(UNIT_SET_ANIMATION);

			API_FUNC_NAKED_START()
				pushad
				push	edi

				mov		eax, unit_index
				mov		edi, animation_graph_index
				mov		ebx, animation_index
				call	FUNCTION

				pop		edi
				popad
				pop		ebp
			API_FUNC_NAKED_END_()
		}

		API_FUNC_NAKED void PLATFORM_API unit_open(datum_index unit_index)
		{
			static uintptr_t FUNCTION = GET_FUNC_PTR(UNIT_OPEN);

			_asm jmp	FUNCTION;
		}

		API_FUNC_NAKED void PLATFORM_API unit_close(datum_index unit_index)
		{
			static uintptr_t FUNCTION = GET_FUNC_PTR(UNIT_CLOSE);

			_asm jmp	FUNCTION;
		}

		API_FUNC_NAKED bool PLATFORM_API unit_try_and_exit_seat(const datum_index unit_index, bool can_run_on_client_side)
		{
			static uintptr_t FUNCTION = GET_FUNC_PTR(UNIT_TRY_AND_EXIT_SEAT);

			_asm jmp	FUNCTION;
		}

		API_FUNC_NAKED bool PLATFORM_API unit_can_see_point(const datum_index unit_index
			, const real_point3d* point
			, const real view_radians)
		{
			static uintptr_t FUNCTION = GET_FUNC_PTR(UNIT_CAN_SEE_POINT);

			_asm jmp	FUNCTION;
		}

		API_FUNC_NAKED void PLATFORM_API unit_cause_player_melee_damage(const datum_index unit_index)
		{
			static uintptr_t FUNCTION = GET_FUNC_PTR(UNIT_CAUSE_PLAYER_MELEE_DAMAGE);

			_asm jmp	FUNCTION;
		}
#endif
	};
	//////////////////////////////////////////////////////////////////////////
	// cryptography
	namespace blam
	{
		void PLATFORM_API generate_md5(char* data, DWORD data_size, char* md5_out)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(GENERATE_MD5);

			_asm {
				push	md5_out
				push	data_size
				push	data
				call	FUNCTION
				add		esp, 4 * 3
			};
		}
	};
};